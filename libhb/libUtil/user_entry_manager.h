#pragma once

#include "event.h"
#include <map>

namespace common {
	namespace Service {
		class UserEntryManager {
		public:
			int initialize(Service::EventDispatcher *eventDispatcher, sce::SampleUtil::System::UserIdManager *userIdManager);
			int finalize(void);
			int updateUserList(void);
			void updateUserColor(void);
			const UserInfo* getUser(sce::SampleUtil::System::UserId userId) const;
			sce::SampleUtil::System::UserId getUserIdFromUserName(const std::string &userName) const;
			void getUsers(UserInfo users[SCE_SAMPLE_UTIL_MAX_LOGIN_USERS]);
			size_t getNumUsers(void) const { return userMap.size(); }

		private:
			Service::EventDispatcher					                 *_eventDispatcher;
			sce::SampleUtil::System::UserIdManager		                 *_userIdManager;
			typedef std::map<sce::SampleUtil::System::UserId, UserInfo > UserMap;
			UserMap userMap;
		};
	}
}