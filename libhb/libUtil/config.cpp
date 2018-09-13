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

#include "config.h"
#include "lua_util.h"
#include "util.h"
#include "../logger.h"

using namespace LibHomebrew;
namespace cc = common::Configuration;
namespace cu = common::Util;

static void convertFloatTable(cc::FloatValueMap &pathMap, const cu::LuaValue::Table *table) {
	common::Util::LuaValue::Table::const_iterator it;
	for (it = table->begin(); it != table->end(); it++) {
		std::string name = it->first;
		const common::Util::LuaValue *lv = it->second;
		SCE_SAMPLE_UTIL_ASSERT(lv->getType() == common::Util::LuaValue::kTypeNumber);//TODO:
		pathMap[name] = lv->asNumber();
	}
}

static int readModelPath2(const common::Util::LuaValue *lv, cc::PathMap  &modelPathMap) {
	if (!isTable(lv)) { return -1; }

	const common::Util::LuaValue *v = lv->getField("model_path");
	if (!isTable(v)) { return -1; }

	convertStringTable(modelPathMap, v->asTable());
	return SCE_OK;

}

static int readTexturePath2(const common::Util::LuaValue *lv, cc::PathMap  &texturePathMap) {
	if (!isTable(lv)) { return -1; }

	const common::Util::LuaValue *v = lv->getField("texture_path");
	if (!isTable(v)) { return -1; }

	convertStringTable(texturePathMap, v->asTable());
	return SCE_OK;
}

static int readSoundPath(const common::Util::LuaValue *lv, cc::SoundPath &soundPath) {
	if (!isTable(lv)) { return -1; }

	const common::Util::LuaValue *v = lv->getField("sound_path");
	if (!isTable(v)) { return -1; }

	cc::PathMap  soundPathMap;
	convertStringTable(soundPathMap, v->asTable());

	soundPath.gameBgm  = soundPathMap["game_bgm"];

	return SCE_OK;
}

static int readPlayStylesSensitivity(const common::Util::LuaValue *lv, cc::PlayStyleSensitivity &playStyleSensitivity) {
	if (!isTable(lv)) { return -1; }

	const common::Util::LuaValue *v = lv->getField("play_style_sensitivity");
	if (!isTable(v)) { return -1; }

	cc::FloatValueMap  playStyleSensitivityValueMap;
	convertFloatTable(playStyleSensitivityValueMap, v->asTable());


	playStyleSensitivity.leftStickMovableViewpoint = playStyleSensitivityValueMap["left_stick_movable_viewpoint"];
	playStyleSensitivity.motionSensorMovableViewpoint = playStyleSensitivityValueMap["motion_sensor_movable_viewpoint"];
	playStyleSensitivity.motionSensorFixedViewpoint = playStyleSensitivityValueMap["motion_sensor_fixed_viewpoint"];
	playStyleSensitivity.touchPadMovableViewpoint = playStyleSensitivityValueMap["touch_pad_movable_viewpoint"];
	playStyleSensitivity.touchPadFixedViewpoint = playStyleSensitivityValueMap["touch_pad_fixed_viewpoint"];
	playStyleSensitivity.padTrackerFixedViewpoint = playStyleSensitivityValueMap["pad_tracker_fixed_viewpoint"];

	return SCE_OK;
}

static int readConfig(lua_State *L, cc::Config &config, std::string language) {
	int ret;
	lua_getglobal(L, "config");
	int top = lua_gettop(L);

	const common::Util::LuaValue *lvConfig = common::Util::LuaValue::parse(L);
	lua_settop(L, top);
	const common::Util::LuaValue *lv = lvConfig;
	if (!isTable(lv)) {
		delete lv;
		return -1;
	}
	
	const common::Util::LuaValue *archive_file = lvConfig->getField("archive_file");
	const common::Util::LuaValue *mount_point = lvConfig->getField("mount_point");
	const common::Util::LuaValue *livearea_data_dir = lvConfig->getField("livearea_data_dir");
	const common::Util::LuaValue *useArchive = lvConfig->getField("useArchive");
	const common::Util::LuaValue *enableSaveDataDialog = lvConfig->getField("enable_save_data_dialog");
	const common::Util::LuaValue *enableDummySaveDataLoad = lvConfig->getField("enable_dummy_save_data_load");
	const common::Util::LuaValue *single_play_save_data_dir_name = lvConfig->getField("single_play_save_data_dir_name");
	const common::Util::LuaValue *multi_play_save_data_dir_name = lvConfig->getField("multi_play_save_data_dir_name");
	const common::Util::LuaValue *single_play_save_data_file_name = lvConfig->getField("single_play_save_data_file_name");
	const common::Util::LuaValue *multi_play_save_data_file_name = lvConfig->getField("multi_play_save_data_file_name");
	const common::Util::LuaValue *load_exec_argument_header_for_invitation = lvConfig->getField("load_exec_argument_header_for_invitation");
	const common::Util::LuaValue *load_exec_argument_header_for_play_together = lvConfig->getField("load_exec_argument_header_for_play_together");

	const common::Util::LuaValue *enableLongestNameTest = lvConfig->getField("enable_longest_name_test");
	const common::Util::LuaValue *longestUserName = lvConfig->getField("longestUserName");
	const common::Util::LuaValue *longestNpOnlineId = lvConfig->getField("longestNpOnlineId");

	const common::Util::LuaValue *playStyleSelectTime = lvConfig->getField("play_style_select_time");
	const common::Util::LuaValue *gameReadyTime = lvConfig->getField("game_ready_time");
	const common::Util::LuaValue *gamePlayTime = lvConfig->getField("game_play_time");
	const common::Util::LuaValue *timeupDisplayTime = lvConfig->getField("timeup_display_time");
	const common::Util::LuaValue *videoRecordingNotificationDisplayTime = lvConfig->getField("video_recording_notification_display_time");
	const common::Util::LuaValue *npServiceLabel = lvConfig->getField("np_service_label");
	const common::Util::LuaValue *npScoreBoardId = lvConfig->getField("np_score_board_id");

	if (!isString(archive_file)) { delete lv;	return -1; }
	if (!isString(mount_point)) { delete lv;	return -1; }
	if (!isString(livearea_data_dir)) { delete lv;	return -1; }
	if (!isString(single_play_save_data_dir_name)) { delete lv;	return -1; }
	if (!isString(multi_play_save_data_dir_name)) { delete lv;	return -1; }
	if (!isString(single_play_save_data_file_name)) { delete lv;	return -1; }
	if (!isString(multi_play_save_data_file_name)) { delete lv;	return -1; }
	if (!isString(load_exec_argument_header_for_invitation)) { delete lv;	return -1; }
	if (!isString(load_exec_argument_header_for_play_together)) { delete lv;	return -1; }

	config.archiveFile = archive_file->asString();
	config.mountPoint = mount_point->asString();
	config.liveareaDataDir = livearea_data_dir->asString();
	config.useArchive = isBool(useArchive) ? useArchive->asBool() : false;
	config.enableSaveDataDialog = isBool(enableSaveDataDialog) ? enableSaveDataDialog->asBool() : true;
	config.enableDummySaveDataLoad = isBool(enableDummySaveDataLoad) ? enableDummySaveDataLoad->asBool() : true;
	config.singlePlaySaveDataDirName = single_play_save_data_dir_name->asString();
	config.multiPlaySaveDataDirName = multi_play_save_data_dir_name->asString();
	config.singlePlaySaveDataFileName = single_play_save_data_file_name->asString();
	config.multiPlaySaveDataFileName = multi_play_save_data_file_name->asString();
	config.loadExecArgumentHeaderForInvitation = load_exec_argument_header_for_invitation->asString();
	config.loadExecArgumentHeaderForPlayTogether = load_exec_argument_header_for_play_together->asString();

	config.playStyleSelectTime = isNumber(playStyleSelectTime) ? playStyleSelectTime->asNumber() : 0;
	config.gameReadyTime = isNumber(gameReadyTime) ? gameReadyTime->asNumber() : 0;
	config.gamePlayTime = isNumber(gamePlayTime) ? gamePlayTime->asNumber() : 0;
	config.timeupDisplayTime = isNumber(timeupDisplayTime) ? timeupDisplayTime->asNumber() : 0;
	config.videoRecordingNotificationDisplayTime = isNumber(videoRecordingNotificationDisplayTime) ? videoRecordingNotificationDisplayTime->asNumber() : 0;
	config.npServiceLabel = isNumber(npServiceLabel) ? npServiceLabel->asNumber() : 0;
	config.npScoreBoardId = isNumber(npScoreBoardId) ? npScoreBoardId->asNumber() : 0;

	config.enableLongestNameTest = isBool(enableLongestNameTest) ? enableLongestNameTest->asBool() : false;
	config.longestUserName = isString(longestUserName) ? longestUserName->asString() : "(Not found)";
	config.longestNpOnlineId = isString(longestNpOnlineId) ? longestNpOnlineId->asString() : "(Not found)";

	ret = readModelPath2(lv, config.modelPathMap);
	if (ret != SCE_OK) { delete lv; return ret; }
	ret = readTexturePath2(lv, config.texturePathMap);
	if (ret != SCE_OK) { delete lv; return ret; }
	ret = readSoundPath(lv, config.soundPath);
	if (ret != SCE_OK) { delete lv; return ret; }

	ret = readPlayStylesSensitivity(lv, config.playStyleSensitivity);

	{
		const common::Util::LuaValue *textTable = lvConfig->getField("text_table_ucs2");
		common::Util::LuaValue::Table::const_iterator it;
		for (it = textTable->asTable()->begin(); it != textTable->asTable()->end(); it++) {
			std::string tag = it->first;
			const common::Util::LuaValue* subTable = it->second;
			std::string text = subTable->getField(language)->asString();
			std::wstring ucs2text = common::Util::convertUtf8toUcs2(text);
			config.textTableUcs2[tag] = ucs2text;
		}
	}
	{
		const common::Util::LuaValue *textTable = lvConfig->getField("text_table_utf8");
		common::Util::LuaValue::Table::const_iterator it;
		for (it = textTable->asTable()->begin(); it != textTable->asTable()->end(); it++) {
			std::string tag = it->first;
			const common::Util::LuaValue* subTable = it->second;
			config.textTableUtf8[tag] = subTable->getField(language)->asString();
		}
	}
	{
		const common::Util::LuaValue *tmltTable = lvConfig->getField("scene_template_table");
		common::Util::LuaValue::Table::const_iterator it;
		for (it = tmltTable->asTable()->begin(); it != tmltTable->asTable()->end(); it++) {
			std::string tag = it->first;
			const common::Util::LuaValue* tmlt = it->second;
			config.sceneTemplateTable[tag] = tmlt->asString();
		}
	}

	delete lv;
	return SCE_OK;
}

int cc::Config::initialize(const char* configPath, const std::string& _language) {
	int ret = 0;

	Logger::Debug("Giving Power to LUA.\n");
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	lua_pushstring(L, _language.c_str());
	lua_setglobal(L, "language");
	Logger::Debug("Language set.\n");

#ifdef ENABLE_PATCH
	Logger::Debug("Enable Patch.\n");
	lua_pushboolean(L, 1);
#else
	lua_pushboolean(L, 0);
#endif
	lua_setglobal(L, "ispatched");
	Logger::Debug("Global set 'ispatched'.\n");

	Logger::Debug("Calling LUA DoFile.\n");
	ret = luaL_dofile(L, configPath);

	if (ret != 0) {
		const char *msg = NULL;
		if (!lua_isnil(L, -1)) {
			msg = lua_tostring(L, -1);
			Logger::Debug("ERROR when executing %s\n", configPath);
			Logger::Debug("   Reason: %s.\n", msg);
			lua_pop(L, 1);
		}
		return ret;
	}
	Logger::Debug("Executing %s ok !\n", configPath);

	Logger::Debug("Reading configuration now...");
	ret = readConfig(L, *this, _language);
	if (ret != SCE_OK) Logger::Debug("error !\n");
	else Logger::Debug("ok !\n");

	lua_close(L);
	Logger::Debug("Taked Power from LUA away.\n");
	lastLoadedFilePath = configPath;
	language = _language;
	return ret;
}

int cc::Config::down() { return initialize(lastLoadedFilePath.c_str(), language); }

std::wstring cc::Config::getTextUcs2(const std::string &id) {
	cc::TextTableUcs2::iterator it = textTableUcs2.find(id);
	if (it == textTableUcs2.end()) return UCS2("### TEXT NOT FOUND ###");
	return it->second;
}

std::string cc::Config::getTextUtf8(const std::string &id) {
	cc::TextTableUtf8::iterator it = textTableUtf8.find(id);
	if (it == textTableUtf8.end()) return "### TEXT NOT FOUND ###";
	return it->second;
}