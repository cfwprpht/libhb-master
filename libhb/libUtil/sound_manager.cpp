#include "stdafx.h"
#include <stdio.h>
#include "sound_manager.h"
#include "../logger.h"

using namespace LibHomebrew;
using namespace common::Configuration;
using namespace common::Service;
using namespace sce::SampleUtil;
using namespace sce::SampleUtil::Audio;

namespace ssa = sce::SampleUtil::Audio;
namespace sss = sce::SampleUtil::System;


int SoundManager::initialize(AudioContext *audioContext, Config *config, EventDispatcher *eventDispatcher) {
	int ret;
	_audioContext = audioContext;
	_config = config;

	//const SoundPath *soundPath = &_config->soundPath;

	/*Logger::Debug("loading click sound data from file : %s...", soundPath->click.c_str());
	ret = ssa::createVoiceDataFromFile(&buttonClickVoiceData, soundPath->click.c_str(), false);
	if (ret != SCE_OK) {
		Logger::Debug("fail !\n");
	} else {
		Logger::Debug("OK\n   creating sound and setting volume...");
		ret = createVoiceAndSetVolume(&buttonClickVoice, buttonClickVoiceData, 0, kClickVoiceVolume, kClickVoiceVolume);
		if (ret != SCE_OK) Logger::Debug("fail !\n");
		else Logger::Debug("OK\n");
	}	

	Logger::Debug("loading game bgm sound data from file : %s...", soundPath->gameBgm.c_str());
	ret = ssa::createVoiceDataFromFile(&gameBgmVoiceData, soundPath->gameBgm.c_str(), true);
	if (ret != SCE_OK) Logger::Debug("fail !\n");
	else {
		Logger::Debug("OK\n   creating sound and setting volume...");
		ret = createVoiceAndSetVolume(&gameBgmVoice, gameBgmVoiceData, 0, kGameBgmVoiceVolume, kGameBgmVoiceVolume);
		if (ret != SCE_OK) Logger::Debug("fail !\n");
		else {
			Logger::Debug("OK\n     setting bgm output to bgm buss...");
			ret = gameBgmVoice->setOutput(0, _audioContext->getBgmBuss());
			if (ret != SCE_OK) Logger::Debug("fail !\n");
			else Logger::Debug("OK\n");
		}
	}	
	
	Logger::Debug("Clearing Personal Infos.\n");
	for (uint32_t i = 0; i < SCE_SAMPLE_UTIL_MAX_LOGIN_USERS; i++) {
		personalInfos[i].userId = SCE_SAMPLE_UTIL_USER_ID_INVALID;
		personalInfos[i].padSpeaker = NULL;

		Logger::Debug("creating sound and setting volume for user %d...", personalInfos[i].userId);
		ret = createVoiceAndSetVolume(&personalInfos[i].secretFoundVoice, secretFoundVoiceData, 0, kSecretVoiceVolume, kSecretVoiceVolume);
		if (ret != SCE_OK) Logger::Debug("fail !\n");
		else Logger::Debug("ok !\n");
	}*/

	Logger::Debug("Register Sound Event Listener...");
	ret = eventDispatcher->registerListener(this);
	if (ret != SCE_OK) Logger::Debug("fail !\n");
	else Logger::Debug("ok !\n");
	
	return SCE_OK;
}

int SoundManager::createVoiceAndSetVolume(ssa::Voice **outVoice, const ssa::VoiceData *voiceData, uint32_t volumeOutIndex, float volumeLeft, float volumeRight) {
	int ret;

	ssa::Voice *voice;
	ret = _audioContext->createVoice(&voice, voiceData);
	if (ret != SCE_OK) return ret;

	ssa::Volume volume;
	memset(&volume, 0, sizeof(volume));
	volume.left = volumeLeft;
	volume.right = volumeRight;
	ret = voice->setVolume(volumeOutIndex, &volume);
	if (ret < SCE_OK) return ret;

	*outVoice = voice;
	return SCE_OK;
}

int SoundManager::onLogin(sce::SampleUtil::System::UserId userId) {
	int ret;
	for (int i = 0; i < SCE_SAMPLE_UTIL_MAX_LOGIN_USERS; i++) {
		if (personalInfos[i].userId == SCE_SAMPLE_UTIL_USER_ID_INVALID) {
			personalInfos[i].userId = userId;

			Logger::Debug("Creating Pad Speaker Buss for user 0x%x...", userId);
			ret = _audioContext->createPadSpeakerBuss(&personalInfos[i].padSpeaker, personalInfos[i].userId);
			if (ret != SCE_OK) Logger::Debug("fail !\n");
			else Logger::Debug("ok !\n");

			if (isLoaded) {
				/*Logger::Debug("Setting Sound output to Pad Speaker of user 0x%x...", userId);
				ret = personalInfos[i].secretFoundVoice->setOutput(0, personalInfos[i].padSpeaker);
				if (ret != SCE_OK) Logger::Debug("fail !\n");
				else Logger::Debug("ok !\n");*/
			}
			break;
		}
	}
	return SCE_OK;
}

int SoundManager::load(void) {
	//int ret;
	if (isLoaded) { return SCE_OK; }

	for (int i = 0; i < SCE_SAMPLE_UTIL_MAX_LOGIN_USERS; i++) {
		/*if (personalInfos[i].userId != SCE_SAMPLE_UTIL_USER_ID_INVALID) {
			ret = personalInfos[i].secretFoundVoice->setOutput(0, personalInfos[i].padSpeaker);
			SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
		}*/
	}

	isLoaded = true;
	return SCE_OK;
}

int SoundManager::finalize(void) {
	for (uint32_t i = 0; i < SCE_SAMPLE_UTIL_MAX_LOGIN_USERS; i++) {
		if (personalInfos[i].padSpeaker != NULL) {
			sce::SampleUtil::destroy(personalInfos[i].padSpeaker);
			personalInfos[i].padSpeaker = NULL;
		}
		/*personalInfos[i].secretFoundVoice->kill();
		sce::SampleUtil::destroy(personalInfos[i].secretFoundVoice);*/
	}

	gameBgmVoice->kill();
	gameBgmVoice->keyOff();

	sce::SampleUtil::destroy(gameBgmVoice);
	gameBgmVoice = NULL;
		
	sce::SampleUtil::destroy(gameBgmVoiceData);
	gameBgmVoiceData = NULL;	

	isLoaded = false;
	return SCE_OK;
}

void SoundManager::onEventArrived(const Service::Event *event, const void *userData, int userFlag) {
	switch (event->type) {
		case Service::kEventLoginUser: {
			int ret;
			Service::EventDataUserInfo *data = (Service::EventDataUserInfo*)userData;
			ret = onLogin(data->userInfo->_userId);
			SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
			break;
		}
		case Service::kEventLogoutUser: {
			Service::EventDataUserInfo *data = (Service::EventDataUserInfo*)userData;
			for (int i = 0; i < SCE_SAMPLE_UTIL_MAX_LOGIN_USERS; i++) {
				if (personalInfos[i].userId == data->userInfo->_userId) {
					sce::SampleUtil::destroy(personalInfos[i].padSpeaker);
					personalInfos[i].padSpeaker = NULL;
					personalInfos[i].userId = SCE_SAMPLE_UTIL_USER_ID_INVALID;
					break;
				}
			}
			break;
		}
		default: { break; }
	}

	if (!isLoaded) { return; }

	switch (event->type) {
		case Service::kEventGameStart: {
			break;
		}
		case Service::kEventGameStop: {
			break;
		}
		case Service::kEventClick: {
			break;
		}
		default: { break; }
	}
}