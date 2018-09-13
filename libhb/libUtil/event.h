#pragma once

#include <stdint.h>
#include <scebase.h>
#include <vector>
#include <sampleutil.h>
#include "util.h"
#include <np_toolkit.h>

#pragma comment( lib , "libSceNpToolkit_stub_weak.a" )

namespace common {
	namespace Service {
		struct UserInfo {
			sce::SampleUtil::System::UserId _userId;
			std::string _userName;
			static const sce::Vectormath::Simd::Aos::Vector4 kPlayerColorPreset[];
			sce::SampleUtil::System::UserColor _userColorIndex;
			sce::Vectormath::Simd::Aos::Vector4	_playerColor;
			std::string _npOnlineId;
			UserInfo(void) { reset(); }
			void reset(void) {
				_userId = sce::SampleUtil::System::kInvalidUserId;
				_userName.clear();
				_userColorIndex = sce::SampleUtil::System::UserColor::kUserColorBlue;
				_playerColor = sce::Vectormath::Simd::Aos::Vector4(0.0f);
				_npOnlineId.clear();
			}
			void dump(void) const {
				printf("[userId=%x, name=%s, npid=%s]\n", _userId, _userName.c_str(), _npOnlineId.c_str());
			}
		};

		enum GameEventType {
			kEventGameStart,
			kEventGameStop,
			kEventClick,
			kEventReady,
			kEventCancel,
			kEventLoginUser,
			kEventLogoutUser,
			kEventInvalid,
		};

		struct EventDataUserInfo { UserInfo* userInfo; };

		struct Event {
			uint32_t type;
			union {
				uint8_t rawData[16];
				uint32_t consecutiveCount;
			} data;
		};

		class EventListener {
		public:
			virtual void onEventArrived(const Event *event, const void *userData, int userFlag) = 0;
			virtual ~EventListener(void) {}
		};

		class EventDispatcher {
		private:
			std::vector<EventListener*> _listeners;

		public:
			int initialize(void);
			int finalize(void);
			int registerListener(EventListener *listener);
			int unregisterListener(EventListener *listener);
			void dispatchEvent(const Event *event, const void *userData, int userFlag);
			void dispatchEvent(uint32_t eventType, const void *userData, int userFlag);
		};

		class NpEventListener {
		public:
			virtual void onNpEventArrived(const sce::Toolkit::NP::Event& event) = 0;
			virtual ~NpEventListener(void) {}
		};

		class NPToolkitStlAllocator : public sce::Toolkit::NP::AllocImplementation {
			virtual void* allocate(int sizeInBytes) { return (::operator new(sizeInBytes)); }

			virtual void deallocate(void* p, unsigned int cnt) { ::operator delete(p); }
		};

		class NpToolkitModule {
		public:
			NpToolkitModule();
			~NpToolkitModule();
		};

		class NpToolkitManager : public Service::EventListener {
		private:
			enum VerifiedAccountCheckerResult {
				kVerifiedAccountCheckerResultUnknown,
				kVerifiedAccountCheckerResultVerified,
				kVerifiedAccountCheckerResultNotVerified,
			};

			typedef std::string NpOnlineId;
			typedef std::map<NpOnlineId, VerifiedAccountCheckerResult> VerifiedAccountCheckerResultMap;

			struct VerifiedAccountChecker {
				sce::Toolkit::NP::Utilities::Future<sce::Toolkit::NP::NpUser> m_localNpUserInformationFuture;
				void startCheckingVerifiedAccountLocal(const sce::SampleUtil::System::UserId &userId);
				VerifiedAccountCheckerResult getCheckingVerifiedAccountResultLocal(const sce::SampleUtil::System::UserId &userId);

				sce::Toolkit::NP::Utilities::Future<sce::Toolkit::NP::NpUsers> m_remoteNpUsersInformationFuture;
				void startCheckingVerifiedAccountRemote(const std::vector<std::string> &onlineIdInRemoteList, const sce::SampleUtil::System::UserId &userIdInLocal); /* userIdInLocal is the user Id who wants to know the result of this check */
				void getCheckingVerifiedAccountResultRemote(VerifiedAccountCheckerResultMap *outMap);
			};

			struct NpUserInfo {
				std::string npOnlineId;
				VerifiedAccountCheckerResult verifiedAccountCheckerResult;
				bool hasVerifiedAccountChecked;

				NpUserInfo(void) :npOnlineId(""), verifiedAccountCheckerResult(kVerifiedAccountCheckerResultUnknown), hasVerifiedAccountChecked(false)
				{}
			};

			std::vector<NpEventListener*> _listeners;
			typedef std::map<sce::SampleUtil::System::UserId, NpUserInfo> SignedInUserMap;
			SignedInUserMap m_signedInUserMap;
			std::vector<NpUserInfo> m_remoteNpUserInfoList;
			NPToolkitStlAllocator* m_npToolkitAllocator;
			VerifiedAccountChecker m_verifiedAccountChecker;
			sce::Toolkit::NP::SessionInformation m_sessionInformation;

		public:
			int initialize(void);
			int finalize(void);
			int registerNpEventListener(NpEventListener *listener);
			int unregisterNpEventListener(NpEventListener *listener);
			sce::SampleUtil::System::UserId getUserIdFromNpOnlineId(const std::string &npOnlineId);
			const std::string &getNpOnlineIdFromUserId(const sce::SampleUtil::System::UserId &userId);

			/* userIdInLocal is the user ID who wants to know the result of this check.
			Whether local signed-in users are verified or not is automaticlly checked in this class. */
			void requireStartingCheckingVerifiedAccountRemote(const std::vector<std::string> &onlineIdInRemoteList, const sce::SampleUtil::System::UserId &userIdInLocal);

			bool isVerifiedAccount(const std::string &npOnlineId);
			void setSessionInformation(sce::Toolkit::NP::SessionInformation& sessionInformation) { m_sessionInformation = sessionInformation; };
			void getSessionInformation(sce::Toolkit::NP::SessionInformation& sessionInformation) { sessionInformation = m_sessionInformation; };

		private:
			static void npToolkitCallback(const sce::Toolkit::NP::Event& event, void *appData);
			void notifySingedIn(const sce::Toolkit::NP::UserInfo &signedInUserInfo);
			void addSingedInUser(const sce::Toolkit::NP::UserInfo &userInfo);
			void removeSingedInUser(const sce::SampleUtil::System::UserId &userId);
			void notifyGotNpUserInformation(const sce::SampleUtil::System::UserId &userId);
			void notifyGotNpUsersInformation(const sce::SampleUtil::System::UserId &userId);
			virtual void onEventArrived(const Service::Event *event, const void *userData, int userFlag);
		};
	}
}