/*
*
*        _       _________ ______            _______  _______  _______  ______   _______  _______
*       ( \      \__   __/(  ___ \ |\     /|(  ___  )(       )(  ____ \(  ___ \ (  ____ )(  ____ \|\     /|
*       | (         ) (   | (   ) )| )   ( || (   ) || () () || (    \/| (   ) )| (    )|| (    \/| )   ( |
*       | |         | |   | (__/ / | (___) || |   | || || || || (__    | (__/ / | (____)|| (__    | | _ | |
*       | |         | |   |  __ (  |  ___  || |   | || |(_)| ||  __)   |  __ (  |     __)|  __)   | |( )| |
*       | |         | |   | (  \ \ | (   ) || |   | || |   | || (      | (  \ \ | (\ (   | (      | || || |
*       | (____/\___) (___| )___) )| )   ( || (___) || )   ( || (____/\| )___) )| ) \ \__| (____/\| () () |
*       (_______/\_______/|/ \___/ |/     \|(_______)|/     \|(_______/|/ \___/ |/   \__/(_______/(_______)
*
*
*
*/

#pragma comment( lib , "libhb_stub_weak.a")
#pragma comment( lib , "libUtil_stub_weak.a")

#define LIBRARY_IMPL  (1)
#include "../libUtil/stdafx.h"
#include <stdio.h>
#include "printf.h"
#include "sound_manager.h"
#include "../prx/logger.h"

using namespace LibHomebrew;
using namespace common::Configuration;
using namespace common::Service;
using namespace sce::SampleUtil::Audio;

namespace ssa = sce::SampleUtil::Audio;
namespace sss = sce::SampleUtil::System;

int SoundManager::initialize(AudioContext *audioContext, Config *config, EventDispatcher *eventDispatcher) {
	int ret;
	_audioContext = audioContext;
	_config = config;

	//const SoundPath *soundPath = &_config->soundPath;

	/*_printf("[SoundManager] loading click sound data from file : %s...", soundPath->click.c_str());
	ret = ssa::createVoiceDataFromFile(&buttonClickVoiceData, soundPath->click.c_str(), false);
	if (ret != SCE_OK) {
		_printf("fail !\n");
	} else {
		_printf("[SoundManager] OK\n   creating sound and setting volume...");
		ret = createVoiceAndSetVolume(&buttonClickVoice, buttonClickVoiceData, 0, kClickVoiceVolume, kClickVoiceVolume);
		if (ret != SCE_OK) _printf("fail !\n");
		else _printf("OK\n");
	}	

	_printf("[SoundManager] loading game bgm sound data from file : %s...", soundPath->gameBgm.c_str());
	ret = ssa::createVoiceDataFromFile(&gameBgmVoiceData, soundPath->gameBgm.c_str(), true);
	if (ret != SCE_OK) _printf("fail !\n");
	else {
		_printf("[SoundManager] OK\n   creating sound and setting volume...");
		ret = createVoiceAndSetVolume(&gameBgmVoice, gameBgmVoiceData, 0, kGameBgmVoiceVolume, kGameBgmVoiceVolume);
		if (ret != SCE_OK) _printf("fail !\n");
		else {
			_printf("OK\n     setting bgm output to bgm buss...");
			ret = gameBgmVoice->setOutput(0, _audioContext->getBgmBuss());
			if (ret != SCE_OK) _printf("fail !\n");
			else _printf("OK\n");
		}
	}	
	
	_printf("[SoundManager] Clearing Personal Infos.\n");
	for (uint32_t i = 0; i < SCE_SAMPLE_UTIL_MAX_LOGIN_USERS; i++) {
		personalInfos[i].userId = SCE_SAMPLE_UTIL_USER_ID_INVALID;
		personalInfos[i].padSpeaker = NULL;

		_printf("[SoundManager] creating sound and setting volume for user %d...", personalInfos[i].userId);
		ret = createVoiceAndSetVolume(&personalInfos[i].secretFoundVoice, secretFoundVoiceData, 0, kSecretVoiceVolume, kSecretVoiceVolume);
		if (ret != SCE_OK) _printf("fail !\n");
		else _printf("OK !\n");
	}*/

	_printf("[SoundManager] Register Sound Event Listener...");
	ret = eventDispatcher->registerListener(this);
	if (ret != SCE_OK) _printf("fail !\n");
	else _printf("OK !\n");
	
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

			_printf("[SoundManager] Creating Pad Speaker Buss for user 0x%x...", userId);
			ret = _audioContext->createPadSpeakerBuss(&personalInfos[i].padSpeaker, personalInfos[i].userId);
			if (ret != SCE_OK) _printf("fail !\n");
			else _printf("OK !\n");

			if (isLoaded) {
				/*_printf("[SoundManager] Setting Sound output to Pad Speaker of user 0x%x...", userId);
				ret = personalInfos[i].secretFoundVoice->setOutput(0, personalInfos[i].padSpeaker);
				if (ret != SCE_OK) _printf("fail !\n");
				else _printf("OK !\n");*/
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