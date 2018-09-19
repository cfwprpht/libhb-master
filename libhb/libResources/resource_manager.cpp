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
#include "printf.h"
#include "../libUtil/stdafx.h"
#include "resource_manager.h"
#include "../prx/console.h"
#include "../prx/logger.h"

using namespace LibHomebrew;
namespace ssg = sce::SampleUtil::Graphics;
namespace cu = common::Util;
namespace cc = common::Configuration;

PRX_INTERFACE cu::ResourceManager::ResourceManager(void) { }

PRX_INTERFACE cu::ResourceManager::~ResourceManager(void) { }

PRX_INTERFACE int cu::ResourceManager::initialize(ssg::GraphicsLoader *loader, ssg::SpriteRenderer *spriteRenderer, cc::Config *_config, const int displayWidth, const int displayHeight) {
	int ret;
	(void)ret;

	graphicsLoader  = loader;
	_spriteRenderer = spriteRenderer;
	_displayWidth   = displayWidth;
	_displayHeight  = displayHeight;
	config          = _config;
	isAddcont       = false;
	_printf("Variables overloaded.\n");

	/*ret = ssg::Collada::createColladaLoader( &colladaLoader, graphicsLoader );
	*/

	_printf("[ResourceManager] Checking for archive...");
	if (_config->useArchive) {
		_printf("use !\n[ResourceManager] Will try to mount the archive...");
		ret = psarcMount.mount(_config->archiveFile, _config->mountPoint);
		if (ret == SCE_OK) _printf("success !\n");
		else _printf("error !\n");
	} else _printf("don't use !\n");

	/*_printf("[ResourceManager] Creating Font Loader.\n");
	ret = ssg::createFontLoader(&fontLoader, graphicsLoader);

	_printf("[ResourceManager] Creating Font SB.\n");
	ssg::FontParam fontParam;
	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBold;
	fontParam.fontSizePixelH = displayHeight * 0.03;
	fontParam.fontSizePixelV = displayHeight * 0.03;
	ret = fontLoader->createFont(&fontSB, &fontParam, 64);

	_printf("[ResourceManager] Creating Font S.\n");
	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBlack;
	fontParam.fontSizePixelH = displayHeight * 0.04;
	fontParam.fontSizePixelV = displayHeight * 0.04;
	ret = fontLoader->createFont(&fontS, &fontParam, 64);

	_printf("[ResourceManager] Creating Font M.\n");
	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBlack;
	fontParam.fontSizePixelH = displayHeight * 0.08;
	fontParam.fontSizePixelV = displayHeight * 0.08;
	ret = fontLoader->createFont(&fontM, &fontParam, 64);

	_printf("[ResourceManager] Creating Font MB.\n");
	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBold;
	fontParam.fontSizePixelH = displayHeight * 0.15;
	fontParam.fontSizePixelV = displayHeight * 0.15;
	ret = fontLoader->createFont(&fontMB, &fontParam, 64);

	_printf("[ResourceManager] Creating Font L.\n");
	fontParam.setDefaults();
	fontParam.weight = ssg::kFontWeightBlack;
	fontParam.fontSizePixelH = displayHeight * 0.20;
	fontParam.fontSizePixelV = displayHeight * 0.20;
	ret = fontLoader->createFont(&fontL, &fontParam, 64);*/

	_printf("[ResourceManager] Initializing DMEM....");
	ret = directMemoryHeap.initialize(256 * 1024 * 1024, 256 * 1024 * 1024);
	if (ret != SCE_OK) _printf("error !\n");
	else _printf("OK !\n");

	return SCE_OK;
}

// You can add texture and collade here to load for your app.
PRX_INTERFACE int cu::ResourceManager::load(void) {
	//loadTexture("bg");
	return SCE_OK;
}

int cu::ResourceManager::loadTexture(const std::string &resourceName) {
	if (textureMap.find(resourceName) != textureMap.end()) {
		// alraedy loaded
		_printf("[ResourceManager] Texture already loaded.\n");
		return SCE_OK;
	}

	sce::SampleUtil::Graphics::Texture *texture = NULL;
	const char *texPath = config->texturePathMap[resourceName].c_str();
	_printf("[ResourceManager] Loading : %s ...", texPath);
	int ret = graphicsLoader->createTextureFromFile(&texture, texPath);
	if (ret != SCE_OK) { _printf("error !\n"); return ret; }
	else _printf("OK !\n");

	_printf("[ResourceManager] Load completed : %s\n", texPath);
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

PRX_INTERFACE sce::SampleUtil::Graphics::Texture *cu::ResourceManager::getTexture(const std::string &resourceName) {
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
	_printf("[ResourceManager] Loading : %s\n", colladaPath);
	int ret = colladaLoader->load(&c, colladaPath);
	_printf("[ResourceManager] Load completed : %s\n", colladaPath);

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

PRX_INTERFACE sce::SampleUtil::Graphics::Collada::ColladaData *cu::ResourceManager::getCollada(const std::string &resourceName) {
	if (colladaMap.find(resourceName) == colladaMap.end()) return NULL;
	return colladaMap[resourceName];
}

PRX_INTERFACE int cu::ResourceManager::finalize(void) {
	int ret;
	ret = directMemoryHeap.finalize();
		
	//unloadTexture("sephiroth");

	/*sce::SampleUtil::destroy(colladaLoader);

	sce::SampleUtil::destroy(fontMB);
	sce::SampleUtil::destroy(fontSB);
	sce::SampleUtil::destroy(fontL);
	sce::SampleUtil::destroy(fontM);
	sce::SampleUtil::destroy(fontS);
	sce::SampleUtil::destroy(fontLoader);*/

	if (config->useArchive) psarcMount.unmount();

	return SCE_OK;
}