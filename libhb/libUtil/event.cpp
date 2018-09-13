
#include "stdafx.h"
#include "event.h"

#pragma comment( lib , "libSceSysmodule_stub_weak.a" )

using namespace common::Service;
namespace sss = sce::SampleUtil::System;

int EventDispatcher::initialize(void) { return SCE_OK; }

int EventDispatcher::finalize(void) { return SCE_OK; }

int EventDispatcher::registerListener(EventListener *listener) {
	printf("[EventDispatcher] REGISTER %p\n", listener);
	_listeners.push_back(listener);
	return SCE_OK;
}

int EventDispatcher::unregisterListener(EventListener *listener) {
	printf("[EventDispatcher] UNREGISTER %p\n", listener);
	std::vector<EventListener*>::iterator it;
	for (it = _listeners.begin(); it != _listeners.end(); it++) {
		if (*it == listener) {
			_listeners.erase(it);
			return SCE_OK;
		}
	}
	return -1;
}

/*static const char* getEventTypeAsString(common::Service::GameEventType eventType) {
	switch (eventType) {
		case kEventClick:					return "kEventClick";
		case kEventUp:				        return "kEventUp";
		case kEventDown:					return "kEventDown";
		case kEventSecret:				    return "kEventSecret";
		case kEventLoginUser:				return "kEventLoginUser";
		case kEventLogoutUser:				return "kEventLogoutUser";
		case kEventInvalid:					return "kEventInvalid";

		default:							return "UNKNOWN EVENT";
	}
}*/

void EventDispatcher::dispatchEvent(const Event *event, const void *userData, int userFlag) {
	//m_debugWindow->debugPrintf("Disptch event : %s\n", getEventTypeAsString((common::Service::GameEventType)event->type));
	for (int i = 0; i < _listeners.size(); i++) {
		_listeners.at(i)->onEventArrived(event, userData, userFlag);
	}
}

void EventDispatcher::dispatchEvent(uint32_t eventType, const void *userData, int userFlag) {
	//m_debugWindow->debugPrintf("Disptch event : %s\n", getEventTypeAsString((common::Service::GameEventType)eventType));
	Event event;
	event.type = eventType;
	for (int i = 0; i < _listeners.size(); i++) {
		EventListener* listener = _listeners.at(i);
		listener->onEventArrived(&event, userData, userFlag);
	}
}

namespace np = sce::Toolkit::NP;

void NpToolkitManager::VerifiedAccountChecker::startCheckingVerifiedAccountLocal(const sce::SampleUtil::System::UserId &userId) {
	int ret;
	sce::Toolkit::NP::UserProfileRequest request;

	request.userInfo.userId = userId;
	request.flag = SCE_TOOLKIT_NP_USER_PROFILE_UPDATED_INFORMATION;

	ret = sce::Toolkit::NP::UserProfile::Interface::getNpUserInformation(&m_localNpUserInformationFuture, &request);
	if (ret < 0) {
		printf("[%s] getNpUserInformation failed: 0x%x\n", __func__, ret);
	}

	return;
}

NpToolkitManager::VerifiedAccountCheckerResult NpToolkitManager::VerifiedAccountChecker::getCheckingVerifiedAccountResultLocal(const sce::SampleUtil::System::UserId &userId) {
	if (m_localNpUserInformationFuture.hasResult()) {
		if (m_localNpUserInformationFuture.get()->isOfficiallyVerified) {
			printf("[%s] User: %d: kVerifiedAccountCheckerResultVerified\n", __func__, userId);
			return kVerifiedAccountCheckerResultVerified;
		}
		else {
			printf("[%s] User: %d: kVerifiedAccountCheckerResultNotVerified\n", __func__, userId);
			return kVerifiedAccountCheckerResultNotVerified;
		}

	}
	else if (m_localNpUserInformationFuture.hasError()) {
		printf("[%s] getNpUserInformation failed: 0x%x\n", __func__, m_localNpUserInformationFuture.getError());

		bool bret = m_localNpUserInformationFuture.reset();
		SCE_SAMPLE_UTIL_ASSERT(bret);
	}

	printf("[%s] User: %d: kVerifiedAccountCheckerResultUnknown\n", __func__, userId);
	return kVerifiedAccountCheckerResultUnknown;
}

void NpToolkitManager::VerifiedAccountChecker::startCheckingVerifiedAccountRemote(const std::vector<std::string> &onlineIdInRemoteList, const sce::SampleUtil::System::UserId &userIdInLocal) {
	SCE_SAMPLE_UTIL_ASSERT(onlineIdInRemoteList.size() > 0);

	int ret;
	sce::Toolkit::NP::UserProfileRequest request;

	request.userInfo.userId = userIdInLocal;
	request.numRemoteUserRequested = onlineIdInRemoteList.size();

	SceNpOnlineId *tmpId = new SceNpOnlineId[onlineIdInRemoteList.size()];
	memset(tmpId, 0, sizeof(*tmpId) * onlineIdInRemoteList.size());

	for (int i = 0; i < onlineIdInRemoteList.size(); i++) {
		strncpy(tmpId[i].data, onlineIdInRemoteList.at(i).c_str(), onlineIdInRemoteList.at(i).size());
	}

	request.remoteUsersOnlineIds = tmpId;

	ret = sce::Toolkit::NP::UserProfile::Interface::getNpUsersInformation(&m_remoteNpUsersInformationFuture, &request);
	if (ret < 0) {
		printf("[%s] getNpUsersInformation failed: 0x%x\n", __func__, ret);
	}

	delete[] tmpId;

	return;
}

void NpToolkitManager::VerifiedAccountChecker::getCheckingVerifiedAccountResultRemote(NpToolkitManager::VerifiedAccountCheckerResultMap *outMap) {
	SCE_SAMPLE_UTIL_ASSERT(outMap != NULL);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(outMap->size(), 0);

	if (m_remoteNpUsersInformationFuture.hasResult()) {
		for (int i = 0; i < m_remoteNpUsersInformationFuture.get()->totalReturnedProfiles; i++)
		{
			if (m_remoteNpUsersInformationFuture.get()->userProfiles[i].isOfficiallyVerified) {
				outMap->insert(std::map<NpOnlineId, VerifiedAccountCheckerResult>::value_type(m_remoteNpUsersInformationFuture.get()->userProfiles[i].npid.handle.data, kVerifiedAccountCheckerResultVerified));
			}
			else {
				outMap->insert(std::map<NpOnlineId, VerifiedAccountCheckerResult>::value_type(m_remoteNpUsersInformationFuture.get()->userProfiles[i].npid.handle.data, kVerifiedAccountCheckerResultNotVerified));
			}
		}
	}
	else if (m_remoteNpUsersInformationFuture.hasError()) {
		printf("[%s] getNpUsersInformation failed: 0x%x\n", __func__, m_localNpUserInformationFuture.getError());

		bool bret = m_remoteNpUsersInformationFuture.reset();
		SCE_SAMPLE_UTIL_ASSERT(bret);
	}

}

int NpToolkitManager::initialize(void) {
	if (m_npToolkitAllocator == NULL) {
		m_npToolkitAllocator = new NPToolkitStlAllocator;
		SCE_SAMPLE_UTIL_ASSERT(m_npToolkitAllocator != NULL);
	}

	m_signedInUserMap.clear();
	m_remoteNpUserInfoList.clear();
	m_sessionInformation.reset();

	np::NpTitleId	npToolkitTitleId;
	//NpTitleId will be read from nptitle.dat if npToolkitTitleId is zero
	np::Parameters params(npToolkitCallback, npToolkitTitleId, (void*)this, 0, 0, m_npToolkitAllocator);
	params.m_pushNotificationFlag = SCE_TOOLKIT_NP_PUSH_NOTIFICATION_SERVICE_DISABLE_ALL;

	int ret = np::Interface::init(params);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	return SCE_OK;
}

int NpToolkitManager::finalize(void) {
	int ret = np::Interface::terminate();
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	m_signedInUserMap.clear();
	m_remoteNpUserInfoList.clear();
	delete  m_npToolkitAllocator;
	m_npToolkitAllocator = NULL;


	return SCE_OK;
}

int NpToolkitManager::registerNpEventListener(NpEventListener *listener) {
	printf("[NpToolkitManager] REGISTER %p\n", listener);
	_listeners.push_back(listener);
	return SCE_OK;
}

int NpToolkitManager::unregisterNpEventListener(NpEventListener *listener) {
	printf("[NpToolkitManager] UNREGISTER %p\n", listener);
	std::vector<NpEventListener*>::iterator it;
	for (it = _listeners.begin(); it != _listeners.end(); it++) {
		if (*it == listener) {
			_listeners.erase(it);
			return SCE_OK;
		}
	}
	return -1;
}

sce::SampleUtil::System::UserId NpToolkitManager::getUserIdFromNpOnlineId(const std::string &npOnlineId) {
	if (m_signedInUserMap.empty()) {
		return sss::kInvalidUserId;
	}

	SignedInUserMap::iterator it;
	for (it = m_signedInUserMap.begin(); it != m_signedInUserMap.end(); it++) {
		if (it->second.npOnlineId == npOnlineId) {
			return it->first;
		}
	}

	return sss::kInvalidUserId;
}

const std::string &NpToolkitManager::getNpOnlineIdFromUserId(const sce::SampleUtil::System::UserId &userId) {
	SCE_SAMPLE_UTIL_ASSERT(!m_signedInUserMap.empty());

	SignedInUserMap::const_iterator it;
	it = m_signedInUserMap.find(userId);
	return it->second.npOnlineId;
}

void NpToolkitManager::notifySingedIn(const np::UserInfo &signedInUserInfo) {
	addSingedInUser(signedInUserInfo);

	// Checking whether another onlineId is being checked whether it is verified account
	SignedInUserMap::iterator it;
	for (it = m_signedInUserMap.begin(); it != m_signedInUserMap.end(); it++) {
		if (it->second.npOnlineId != signedInUserInfo.npId.handle.data && !it->second.hasVerifiedAccountChecked) {
			return;
		}
	}

	m_verifiedAccountChecker.startCheckingVerifiedAccountLocal(signedInUserInfo.userId);
}

void NpToolkitManager::addSingedInUser(const np::UserInfo &userInfo) {
	//TODO: it seems singed-in event occurs at logged-in, but singed-out event doesn't occur at logged-out
	SCE_SAMPLE_UTIL_ASSERT(m_signedInUserMap.size() < SCE_USER_SERVICE_MAX_LOGIN_USERS);
	SCE_SAMPLE_UTIL_ASSERT(m_signedInUserMap.find(userInfo.userId) == m_signedInUserMap.end());

	m_signedInUserMap[userInfo.userId].npOnlineId = userInfo.npId.handle.data;
	printf("NpToolkitManager::addSingedInUser: UserId: %d, NpOnlineId: %s\n",
		userInfo.userId, userInfo.npId.handle.data);

	return;
}

void NpToolkitManager::removeSingedInUser(const sce::SampleUtil::System::UserId &userId) {
	//TODO: it seems singed-out event occurs at logged-in if the user is signed-out-state

	if (m_signedInUserMap.empty()) {
		return;
	}
	else if (m_signedInUserMap.find(userId) == m_signedInUserMap.end()) {
		return;
	}

	printf("NpToolkitManager::removeSingedInUser: UserId: %d, NpOnlineId: %s\n",
		userId, m_signedInUserMap[userId].npOnlineId.c_str());

	int ret = m_signedInUserMap.erase(userId);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, 1);

	return;
}

void NpToolkitManager::notifyGotNpUserInformation(const sce::SampleUtil::System::UserId &userId) {
	bool isUserAlreadySignedOut = false;
	if (m_signedInUserMap.empty()) {
		isUserAlreadySignedOut = true;
	}
	else if (m_signedInUserMap.find(userId) == m_signedInUserMap.end()) {
		isUserAlreadySignedOut = true;
	}

	if (isUserAlreadySignedOut) {
		m_verifiedAccountChecker.getCheckingVerifiedAccountResultLocal(userId);
	}
	else {
		m_signedInUserMap[userId].verifiedAccountCheckerResult = m_verifiedAccountChecker.getCheckingVerifiedAccountResultLocal(userId);
		m_signedInUserMap[userId].hasVerifiedAccountChecked = true;
	}

	// Checking whether there is another onlineId being not checked whether it is verified account
	SignedInUserMap::iterator it;
	for (it = m_signedInUserMap.begin(); it != m_signedInUserMap.end(); it++) {
		if (it->first != userId && !it->second.hasVerifiedAccountChecked) {
			m_verifiedAccountChecker.startCheckingVerifiedAccountLocal(it->first);
			return;
		}
	}

	return;
}

void NpToolkitManager::notifyGotNpUsersInformation(const sce::SampleUtil::System::UserId &userId) {
	VerifiedAccountCheckerResultMap resultMap;
	resultMap.clear();

	m_verifiedAccountChecker.getCheckingVerifiedAccountResultRemote(&resultMap);

	VerifiedAccountCheckerResultMap::iterator itResultMap;
	for (itResultMap = resultMap.begin(); itResultMap != resultMap.end(); itResultMap++) {
		printf("[%s] %s: %d\n", __func__, itResultMap->first.c_str(), itResultMap->second);

		for (int i = 0; i < m_remoteNpUserInfoList.size(); i++) {
			if (m_remoteNpUserInfoList.at(i).npOnlineId == itResultMap->first.c_str()) {
				m_remoteNpUserInfoList.at(i).verifiedAccountCheckerResult = itResultMap->second;
				m_remoteNpUserInfoList.at(i).hasVerifiedAccountChecked = true;
				break;
			}
		}
	}

	// Checking whether there is another onlineId being not checked whether it is verified account
	std::vector<std::string> notCheckedOnlineIdInRemoteList;
	notCheckedOnlineIdInRemoteList.clear();

	std::vector<NpUserInfo>::iterator it;
	for (it = m_remoteNpUserInfoList.begin(); it != m_remoteNpUserInfoList.end(); it++) {
		if (!it->hasVerifiedAccountChecked) {
			notCheckedOnlineIdInRemoteList.push_back(it->npOnlineId);
		}
	}

	if (notCheckedOnlineIdInRemoteList.size() > 0) {
		m_verifiedAccountChecker.startCheckingVerifiedAccountRemote(notCheckedOnlineIdInRemoteList, userId);
	}

	return;
}

void NpToolkitManager::requireStartingCheckingVerifiedAccountRemote(const std::vector<std::string> &onlineIdInRemoteList, const sce::SampleUtil::System::UserId &userIdInLocal) {
	// Checking whether another onlineId is being checked whether it is verified account
	bool shouldStartCheckingAfterCurrentCheckingFinished = false;

	std::vector<NpUserInfo>::iterator it;
	for (it = m_remoteNpUserInfoList.begin(); it != m_remoteNpUserInfoList.end(); it++) {
		if (!it->hasVerifiedAccountChecked) {
			shouldStartCheckingAfterCurrentCheckingFinished = true;
		}
	}

	for (int i = 0; i < onlineIdInRemoteList.size(); i++) {
		bool isFound = false;
		for (int j = 0; j < m_remoteNpUserInfoList.size(); j++) {
			if (m_remoteNpUserInfoList.at(j).npOnlineId == onlineIdInRemoteList.at(i)) {
				m_remoteNpUserInfoList.at(j).hasVerifiedAccountChecked = false;
				m_remoteNpUserInfoList.at(j).verifiedAccountCheckerResult = kVerifiedAccountCheckerResultUnknown;
				isFound = true;
				break;
			}
		}

		if (!isFound) {
			NpUserInfo npUserInfo;
			npUserInfo.npOnlineId = onlineIdInRemoteList.at(i);
			m_remoteNpUserInfoList.push_back(npUserInfo);
		}
	}

	if (!shouldStartCheckingAfterCurrentCheckingFinished) {
		m_verifiedAccountChecker.startCheckingVerifiedAccountRemote(onlineIdInRemoteList, userIdInLocal);
	}
}

bool NpToolkitManager::isVerifiedAccount(const std::string &npOnlineId) {
	if (m_signedInUserMap.empty() && m_remoteNpUserInfoList.empty()) {
		return false;
	}

	SignedInUserMap::iterator itMap;
	for (itMap = m_signedInUserMap.begin(); itMap != m_signedInUserMap.end(); itMap++) {
		if (itMap->second.npOnlineId == npOnlineId) {
			return itMap->second.verifiedAccountCheckerResult == kVerifiedAccountCheckerResultVerified;
		}
	}

	for (int i = 0; i < m_remoteNpUserInfoList.size(); i++) {
		if (m_remoteNpUserInfoList.at(i).npOnlineId == npOnlineId) {
			return m_remoteNpUserInfoList.at(i).verifiedAccountCheckerResult == kVerifiedAccountCheckerResultVerified;
		}
	}

	return false;
}

void NpToolkitManager::onEventArrived(const Service::Event *event, const void *userData, int userFlag) {
	if (!userData) { return; }
	int ret;

	switch (event->type) {
		case Service::kEventLogoutUser: {
			//TODO: it seems singed-in or singed-out event occurs at logged-in, but any event doesn't occur at logged-out
			Service::EventDataUserInfo *data = (Service::EventDataUserInfo*)userData;
			removeSingedInUser(data->userInfo->_userId);

			ret = np::Interface::userLoggedOut(data->userInfo->_userId);
			SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_TOOLKIT_NP_SUCCESS);

			printf("[%s] userLoggedOut\n", __FUNCTION__);

			break;
		}
		default: { break; }
	}

	return;
}

void NpToolkitManager::npToolkitCallback(const np::Event& event, void *appData) {
	NpToolkitManager *self = (NpToolkitManager*)appData;
	std::string serviceString;
	std::string eventString;
	switch (event.service) {
	case np::ServiceType::core:	serviceString = "core"; break;
	case np::ServiceType::matching:	serviceString = "matching"; break;
	case np::ServiceType::messaging:	serviceString = "messaging"; break;
	case np::ServiceType::profile:	serviceString = "profile"; break;
	case np::ServiceType::sessions:   serviceString = "sessions"; break;
	default:							serviceString = common::Util::formatString("unknown %d", event.service); break;
	}

	switch (event.event) {
	case np::Event::enetDown:eventString = "enetDown"; break;

	case np::Event::loggedIn:
		eventString = "loggedIn";
		self->notifySingedIn(event.userInformation);
		break;
	case np::Event::loggedOut:
		eventString = "loggedOut";
		self->removeSingedInUser(event.userInformation.userId);
		break;

	case np::Event::matchingSessionCreated:eventString = "matchingSessionCreated"; break;
	case np::Event::matchingSessionSearchCompleted:eventString = "matchingSessionSearchCompleted"; break;
	case np::Event::matchingSessionJoined:eventString = "matchingSessionJoined"; break;
	case np::Event::matchingSessionLeft:eventString = "matchingSessionLeft"; break;
	case np::Event::matchingSessionModified:eventString = "matchingSessionModified"; break;
	case np::Event::matchingSessionUpdate:eventString = "matchingSessionUpdate"; break;

	case np::Event::messageSent:eventString = "messageSent"; break;
	case np::Event::messageDialogTerminated:eventString = "messageDialogTerminated"; break;

	case np::Event::profileGotNpUserInformation:
		eventString = "profileGotNpUserInformation";
		self->notifyGotNpUserInformation(event.userInformation.userId);
		break;

	case np::Event::profileGotNpUsersInformation:
		eventString = "profileGotNpUsersInformation";
		self->notifyGotNpUsersInformation(event.userInformation.userId);
		break;

	case np::Event::npSessionInvitePostInvitationResult:eventString = "npSessionInvitePostInvitationResult"; break;

	default:
		eventString = common::Util::formatString("unknown %d", event.event);
		printf("NpToolkitManager::npToolkitCallback: received unknown event: returnCode = 0x%x\n", event.returnCode);
		break;
	}

	//self->m_debugWindow->debugPrintf("NP EVENT : Service=%s, event=%s\n", serviceString.c_str(), eventString.c_str());

	for (int i = 0; i<self->_listeners.size(); i++) {
		self->_listeners.at(i)->onNpEventArrived(event);
	}
}

common::Service::NpToolkitModule::NpToolkitModule() {
	int ret = sceSysmoduleLoadModule(SCE_SYSMODULE_NP_TOOLKIT);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
}

common::Service::NpToolkitModule::~NpToolkitModule() {
	int ret = sceSysmoduleUnloadModule(SCE_SYSMODULE_NP_TOOLKIT);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
}