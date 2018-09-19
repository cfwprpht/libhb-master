#pragma once

#include <libdbg.h>
#include <sampleutil.h>
#include "../libUtil/event.h"
#include "config.h"
#include "resource_manager.h"

#ifndef PRX_INTERFACE
#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif
#endif

namespace common {
	namespace Service {
		class PRX_INTERFACE SoundManager : private Service::EventListener {
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
				common::Service::EventDispatcher *eventDispatcher);

			int load(void);
			int finalize(void);
			virtual void onEventArrived(const Service::Event *event, const void *userData, int userFlag);
			~SoundManager(void) {}
		};
	}
}