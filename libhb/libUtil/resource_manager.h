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
#include "psarc_util.h"
#include "thread_util.h"
#include "config.h"

namespace common {
	namespace Util {
		class ResourceManager {
		public:
			ResourceManager(void);
			virtual ~ResourceManager(void);

			int initialize(sce::SampleUtil::Graphics::GraphicsLoader *loader,
				sce::SampleUtil::Graphics::SpriteRenderer *spriteRenderer,
				common::Configuration::Config *config,
				const int displayWidth, const int displayHeight);

			int finalize(void);

			sce::SampleUtil::Graphics::Texture *getTexture(const std::string &resourceName);
			sce::SampleUtil::Graphics::Collada::ColladaData *getCollada(const std::string &resourceName);
			common::Configuration::Config *config;
			bool verbose;

		public:
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

		private:
			int loadTexture(const std::string &resourceName);
			int unloadTexture(const std::string &resourceName);
			int loadCollada(const std::string &resourceName);
			int unloadCollada(const std::string &resourceName);
			std::hash_map<std::string, std::string> textTable;
		};
	}
}
