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

#include "stdafx.h"
#include "resource_manager.h"
#include "../console.h"

using namespace LibHomebrew;
namespace ssg = sce::SampleUtil::Graphics;
namespace cu = common::Util;
namespace cc = common::Configuration;

cu::ResourceManager::ResourceManager(void) { }

cu::ResourceManager::~ResourceManager(void) { }

int cu::ResourceManager::initialize(ssg::GraphicsLoader *loader, ssg::SpriteRenderer *spriteRenderer, cc::Config *_config, const int displayWidth, const int displayHeight) {
	int ret;
	(void)ret;

	graphicsLoader = loader;
	_spriteRenderer = spriteRenderer;
	_displayWidth = displayWidth;
	_displayHeight = displayHeight;
	config = _config;

	isAddcont = false;

	if (_config->useArchive) {
		ret = psarcMount.mount(_config->archiveFile, _config->mountPoint);
		SCE_SAMPLE_UTIL_ASSERT_MSG(SCE_OK == ret, "ret=%#x", ret);
		if (ret == SCE_OK) if (verbose) Console::WriteLine("[Archive file was mounted.]\n");
	}

#if 0 //TODO: Bug 57527
	ret = loadTextTable(config->useArchive ? ("/app0/game_data/lua_script/text.lua") : "/app0/game_data/lua_script/text.lua",
		config->language, "texttable", textTable);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ret = loadTextTable(config->useArchive ? (config->mountPoint + "lua_script/ui.lua") : "/app0/game_data/lua_script/ui.lua",
		config->language, "uitemplatetable", uiTemplateTable);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
#endif

	ret = ssg::createFontLoader(&fontLoader, graphicsLoader);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ssg::FontParam fontParam;
	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBold;
	fontParam.fontSizePixelH = displayHeight * 0.03;
	fontParam.fontSizePixelV = displayHeight * 0.03;
	ret = fontLoader->createFont(&fontSB, &fontParam, 64);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBlack;
	fontParam.fontSizePixelH = displayHeight * 0.04;
	fontParam.fontSizePixelV = displayHeight * 0.04;
	ret = fontLoader->createFont(&fontS, &fontParam, 64);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBlack;
	fontParam.fontSizePixelH = displayHeight * 0.08;
	fontParam.fontSizePixelV = displayHeight * 0.08;
	ret = fontLoader->createFont(&fontM, &fontParam, 64);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBold;
	fontParam.fontSizePixelH = displayHeight * 0.15;
	fontParam.fontSizePixelV = displayHeight * 0.15;
	ret = fontLoader->createFont(&fontMB, &fontParam, 64);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBlack;
	fontParam.fontSizePixelH = displayHeight * 0.20;
	fontParam.fontSizePixelV = displayHeight * 0.20;
	ret = fontLoader->createFont(&fontL, &fontParam, 64);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ret = directMemoryHeap.initialize(256 * 1024 * 1024, 256 * 1024 * 1024);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	return SCE_OK;
}

int cu::ResourceManager::loadTexture(const std::string &resourceName) {
	if (textureMap.find(resourceName) != textureMap.end()) {
		// alraedy loaded
		return SCE_OK;
	}

	sce::SampleUtil::Graphics::Texture *texture = NULL;
	const char *texPath = config->texturePathMap[resourceName].c_str();
	if (verbose) Console::WriteLine("Loading : %s\n", texPath);
	int ret = graphicsLoader->createTextureFromFile(&texture, texPath);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	if (verbose) Console::WriteLine("Load completed : %s\n", texPath);
	textureMap[resourceName] = texture;
	return ret;
}

int cu::ResourceManager::unloadTexture(const std::string &resourceName) {
	if (textureMap.find(resourceName) == textureMap.end()) return SCE_OK;
	sce::SampleUtil::Graphics::Texture *texture = textureMap[resourceName];
	sce::SampleUtil::destroy(texture);
	textureMap.erase(resourceName);
	return SCE_OK;
}

sce::SampleUtil::Graphics::Texture *cu::ResourceManager::getTexture(const std::string &resourceName) {
	if (textureMap.find(resourceName) == textureMap.end()) return NULL;
	return textureMap[resourceName];
}

int cu::ResourceManager::loadCollada(const std::string &resourceName) {
	if (colladaMap.find(resourceName) != colladaMap.end()) {
		// alraedy loaded
		return SCE_OK;
	}

	sce::SampleUtil::Graphics::Collada::ColladaData *c = NULL;
	const char* colladaPath = config->modelPathMap[resourceName].c_str();
	if (verbose) Console::WriteLine("Loading : %s\n", colladaPath);
	int ret = colladaLoader->load(&c, colladaPath);
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	if (verbose) Console::WriteLine("Load completed : %s\n", colladaPath);

	colladaMap[resourceName] = c;

	return ret;
}

int cu::ResourceManager::unloadCollada(const std::string &resourceName) {
	if (colladaMap.find(resourceName) == colladaMap.end()) return SCE_OK;
	sce::SampleUtil::Graphics::Collada::ColladaData *collada = colladaMap[resourceName];
	sce::SampleUtil::destroy(collada);
	colladaMap.erase(resourceName);
	return SCE_OK;
}

sce::SampleUtil::Graphics::Collada::ColladaData *cu::ResourceManager::getCollada(const std::string &resourceName) {
	if (colladaMap.find(resourceName) == colladaMap.end()) return NULL;
	return colladaMap[resourceName];
}

int cu::ResourceManager::finalize(void) {
	int ret;
	ret = directMemoryHeap.finalize();
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ret = unloadTexture("busy48");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	ret = unloadTexture("eff_explode");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	ret = unloadTexture("eff_shot");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	ret = unloadTexture("target_g");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	ret = unloadTexture("bullet");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	ret = unloadTexture("verified_account_icon_large");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
	ret = unloadTexture("verified_account_icon_small");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ret = unloadCollada("shooting_range");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ret = unloadCollada("target_a");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ret = unloadCollada("gun");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	ret = unloadCollada("bullet");
	SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);

	sce::SampleUtil::destroy(colladaLoader);

	sce::SampleUtil::destroy(fontMB);
	sce::SampleUtil::destroy(fontSB);
	sce::SampleUtil::destroy(fontL);
	sce::SampleUtil::destroy(fontM);
	sce::SampleUtil::destroy(fontS);
	sce::SampleUtil::destroy(fontLoader);

	if (config->useArchive) psarcMount.unmount();

	return SCE_OK;
}