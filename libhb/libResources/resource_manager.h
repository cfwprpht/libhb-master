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

#pragma once

#include <hash_map>
#include <sampleutil.h>
#include "../libUtil/psarc_util.h"
#include "../libUtil/thread_util.h"
#include "config.h"

#ifndef PRX_INTERFACE
#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif
#endif

namespace common {
	namespace Util {
		class ResourceManager {
		public:
			common::Configuration::Config *config;
			bool verbose;
			int		_displayWidth;
			int		_displayHeight;

			sce::SampleUtil::Graphics::GraphicsLoader *graphicsLoader;
			sce::SampleUtil::Graphics::SpriteRenderer *_spriteRenderer;

			typedef stdext::hash_map<std::string, sce::SampleUtil::Graphics::Texture*> TextureMap;
			TextureMap textureMap;

			typedef stdext::hash_map<std::string, sce::SampleUtil::Graphics::Collada::ColladaData*> ColladaMap;
			ColladaMap colladaMap;

			sce::SampleUtil::Graphics::Collada::ColladaLoader	*colladaLoader;
			common::Util::PsarcMount					         psarcMount;
			sce::SampleUtil::Graphics::FontLoader				*fontLoader;
			sce::SampleUtil::Graphics::Font						*fontMB;
			sce::SampleUtil::Graphics::Font						*fontSB;
			sce::SampleUtil::Graphics::Font						*fontL;
			sce::SampleUtil::Graphics::Font						*fontM;
			sce::SampleUtil::Graphics::Font						*fontS;
			bool                                                isAddcont;
			Util::DirectMemoryHeap                              directMemoryHeap;

			PRX_INTERFACE ResourceManager(void);
			PRX_INTERFACE virtual ~ResourceManager(void);

			PRX_INTERFACE int initialize(sce::SampleUtil::Graphics::GraphicsLoader *loader,
				sce::SampleUtil::Graphics::SpriteRenderer *spriteRenderer,
				common::Configuration::Config *config,
				const int displayWidth, const int displayHeight);

			PRX_INTERFACE int finalize(void);
			PRX_INTERFACE int load(void);
			PRX_INTERFACE sce::SampleUtil::Graphics::Texture *getTexture(const std::string &resourceName);
			PRX_INTERFACE sce::SampleUtil::Graphics::Collada::ColladaData *getCollada(const std::string &resourceName);

		private:			
			int loadTexture(const std::string &resourceName);
			int unloadTexture(const std::string &resourceName);
			int loadCollada(const std::string &resourceName);
			int unloadCollada(const std::string &resourceName);
			std::hash_map<std::string, std::string> textTable;
		};
	}
}
