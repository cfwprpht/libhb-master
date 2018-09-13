#pragma once

#include <libdbg.h>
#include <sampleutil.h>
#include "event.h"
#include "config.h"
#include "resource_manager.h"
#include "user_entry_manager.h"

namespace common {
	namespace Service {
		class SoundManager : private Service::EventListener {
		private:
			sce::SampleUtil::Audio::AudioContext	*_audioContext;
			sce::SampleUtil::Audio::VoiceData		*gameBgmVoiceData;
			sce::SampleUtil::Audio::Voice			*gameBgmVoice;	

			struct PersonalInfo {
				sce::SampleUtil::System::UserId userId;
				sce::SampleUtil::Audio::Buss *padSpeaker;
			};

			PersonalInfo personalInfos[SCE_SAMPLE_UTIL_MAX_LOGIN_USERS];
			const common::Configuration::Config *_config;
			bool isLoaded;
			int onLogin(sce::SampleUtil::System::UserId UserId);
			int createVoiceAndSetVolume(sce::SampleUtil::Audio::Voice **outVoice, const sce::SampleUtil::Audio::VoiceData *voiceData,
				uint32_t volumeOutIndex, float volumeLeft, float volumeRight);

			//const float kGameBgmVoiceVolume  = 0.40f;
			

		public:
			int initialize(sce::SampleUtil::Audio::AudioContext *audioContext,
				Configuration::Config *config,
				common::Service::UserEntryManager *userEntryManager,
				common::Service::EventDispatcher *eventDispatcher);

			int load(void);
			int finalize(void);
			virtual void onEventArrived(const Service::Event *event, const void *userData, int userFlag);
			~SoundManager(void) {}
		};
	}
}