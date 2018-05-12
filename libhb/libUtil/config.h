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

#include <string>
#include <stdint.h>
#include <stdio.h>
#include <hash_map>

#pragma once

namespace common {
	namespace Configuration {
		class SoundPath {
		public:
			void dump() {}
			void clear() {}

		public:
			std::string		shot;
			std::string		explosion;
			std::string		reload;
			std::string		gameBgm;
			std::string		applause;

		};

		class PlayStyleSensitivity {
		public:
			void dump() {}
			void clear() {}

		public:
			float	leftStickMovableViewpoint;
			float	motionSensorMovableViewpoint;
			float	motionSensorFixedViewpoint;
			float	touchPadMovableViewpoint;
			float	touchPadFixedViewpoint;
			float	padTrackerFixedViewpoint;
		};


		typedef std::hash_map<std::string, std::string>  PathMap;
		typedef std::hash_map<std::string, float>        FloatValueMap;
		typedef std::hash_map<std::string, std::wstring> TextTableUcs2;
		typedef std::hash_map<std::string, std::string>  TextTableUtf8;
		typedef std::hash_map<std::string, std::string>  TemplateTable;


		class Config {
		public:
			int initialize(const char* configPath, const std::string& language);
			int reload();
			void clear();
			void dump();
			std::wstring getTextUcs2(const std::string &id);
			std::string getTextUtf8(const std::string &id);

		public:
			bool		useArchive;
			std::string language;
			std::string	archiveFile;
			std::string	mountPoint;
			std::string liveareaDataDir;
			bool       enableSaveDataDialog;
			bool       enableDummySaveDataLoad;
			std::string	singlePlaySaveDataDirName;
			std::string	multiPlaySaveDataDirName;
			std::string	singlePlaySaveDataFileName;
			std::string	multiPlaySaveDataFileName;
			std::string	loadExecArgumentHeaderForInvitation;
			std::string	loadExecArgumentHeaderForPlayTogether;

			int playStyleSelectTime;
			int gameReadyTime;
			int gamePlayTime;
			int timeupDisplayTime;
			int videoRecordingNotificationDisplayTime;
			int npServiceLabel;
			int npScoreBoardId;

			bool enableLongestNameTest;
			std::string longestUserName;
			std::string longestNpOnlineId;

			PathMap modelPathMap;
			PathMap texturePathMap;
			SoundPath soundPath;

			TextTableUcs2 textTableUcs2;
			TextTableUtf8 textTableUtf8;
			TemplateTable sceneTemplateTable;


			PlayStyleSensitivity		playStyleSensitivity;

			std::string	lastLoadedFilePath;
		};

	}
}
