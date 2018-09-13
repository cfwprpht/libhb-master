#include "stdafx.h"
#include "user_entry_manager.h"

namespace cs = common::Service;
namespace ssi = sce::SampleUtil::Input;
namespace sss = sce::SampleUtil::System;
namespace ssg = sce::SampleUtil::Graphics;
namespace vm = sce::Vectormath::Simd::Aos;

const vm::Vector4 common::Service::UserInfo::kPlayerColorPreset[] = { vm::Vector4(0.0f, 0.5f, 1.0f, 1.0f),
vm::Vector4(1.0f, 0.0f, 0.0f, 1.0f),
vm::Vector4(0.0f, 1.0f, 0.0f, 1.0f),
vm::Vector4(1.0f, 0.0f, 1.0f, 1.0f) };

int cs::UserEntryManager::initialize(Service::EventDispatcher *eventDispatcher, sce::SampleUtil::System::UserIdManager *userIdManager) {
	int ret;
	(void)ret;

	_eventDispatcher = eventDispatcher;
	_userIdManager = userIdManager;

	sce::SampleUtil::System::UserIdList		list;
	memset(&list, 0x00, sizeof(list));

	ret = _userIdManager->getLoginUserIdList(&list);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	Service::EventDataUserInfo					data;
	for (int i = 0; i<SCE_USER_SERVICE_MAX_LOGIN_USERS; i++) {
		if (list.userId[i] != sss::kInvalidUserId) {
			sss::UserId userId = list.userId[i];

			memset(&data, 0x00, sizeof(data));
			UserInfo userInfo;
			userInfo._userId = list.userId[i];

			char userName[sss::kMaxUserNameLength + 1];
			ret = _userIdManager->getUserName(userId, userName, sizeof(userName));
			if (ret == SCE_OK) {
				userInfo._userName = userName;
			}

			ret = _userIdManager->getUserColor(userId, &userInfo._userColorIndex);
			if (ret == SCE_OK) {
				userInfo._playerColor = userInfo.kPlayerColorPreset[userInfo._userColorIndex];
			}

			userMap[userId] = userInfo;
			_eventDispatcher->dispatchEvent(Service::kEventLoginUser, &data, 0);
		}
	}

	return SCE_OK;
}

int cs::UserEntryManager::finalize(void) { return SCE_OK; }

int cs::UserEntryManager::updateUserList(void) {
	int ret;
	(void)ret;

	sce::SampleUtil::System::UserLoginStatusChangedEvents events;
	memset(&events, 0x00, sizeof(events));

	ret = _userIdManager->getUserLoginStatusChangedEventsOfLastUpdate(&events);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	Service::EventDataUserInfo					data;
	for (int i = 0; i<SCE_USER_SERVICE_MAX_LOGIN_USERS; i++) {
		if (events.leftUserIdList.userId[i] != sss::kInvalidUserId) {
			sss::UserId userId = events.leftUserIdList.userId[i];

			ret = sce::Toolkit::NP::Interface::userLoggedOut(userId);
			SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

			memset(&data, 0x00, sizeof(data));
			data.userInfo = &userMap.find(userId)->second;
			_eventDispatcher->dispatchEvent(Service::kEventLogoutUser, &data, 0);

			SCE_SAMPLE_UTIL_ASSERT(userMap.find(userId) != userMap.end());
			userMap.erase(userMap.find(userId));
		}
	}

	for (int i = 0; i<SCE_USER_SERVICE_MAX_LOGIN_USERS; i++) {
		if (events.joinedUserIdList.userId[i] != sss::kInvalidUserId) {
			sss::UserId userId = events.joinedUserIdList.userId[i];

			memset(&data, 0x00, sizeof(data));
			UserInfo userInfo;
			userInfo._userId = userId;

			char userName[sss::kMaxUserNameLength + 1];
			ret = _userIdManager->getUserName(userId, userName, sizeof(userName));
			if (ret == SCE_OK) {
				userInfo._userName = userName;
			}

			ret = _userIdManager->getUserColor(userId, &userInfo._userColorIndex);
			if (ret == SCE_OK) {
				userInfo._playerColor = userInfo.kPlayerColorPreset[userInfo._userColorIndex];
			}

			userMap[userId] = userInfo;
			data.userInfo = &userInfo;
			_eventDispatcher->dispatchEvent(Service::kEventLoginUser, &data, 0);
		}
	}

	return SCE_OK;
}

void cs::UserEntryManager::updateUserColor(void) {
	UserMap::iterator it;
	for (it = userMap.begin(); it != userMap.end(); it++) {
		UserInfo *userInfo = &it->second;
		int ret = _userIdManager->getUserColor(userInfo->_userId, &(userInfo->_userColorIndex));
		if (ret == SCE_OK) {
			userInfo->_playerColor = userInfo->kPlayerColorPreset[userInfo->_userColorIndex];
		}
	}
}

void cs::UserEntryManager::getUsers(UserInfo users[SCE_SAMPLE_UTIL_MAX_LOGIN_USERS]) {
	for (uint32_t i = 0; i < SCE_SAMPLE_UTIL_MAX_LOGIN_USERS; i++) {
		users[i]._userId = sss::kInvalidUserId;
		users[i]._userName = "";
	}
	int index = 0;
	UserMap::iterator it;
	for (it = userMap.begin(); it != userMap.end(); it++) {
		users[index] = it->second;
		index++;
	}
}

const cs::UserInfo* cs::UserEntryManager::getUser(sce::SampleUtil::System::UserId userId) const {
	UserMap::const_iterator it = userMap.find(userId);
	if (it == userMap.end()) {
		return NULL;
	}
	return &it->second;
}

sss::UserId cs::UserEntryManager::getUserIdFromUserName(const std::string &userName) const {
	sss::UserId userId = sss::kInvalidUserId;

	UserMap::const_iterator it;
	for (it = userMap.begin(); it != userMap.end(); it++) {
		const UserInfo *userInfo = &it->second;
		if (userInfo->_userName == userName) {
			userId = userInfo->_userId;
			break;
		}
	}

	return userId;
}