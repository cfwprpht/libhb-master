#pragma once

#include <stdint.h>
#include <scebase.h>
#include <vector>
#include <sampleutil.h>
#include "util.h"

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
	}
}