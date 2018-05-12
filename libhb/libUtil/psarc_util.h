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
#include <string>
#include <fios2.h>

namespace common {
	namespace Util {
		class PsarcMount {
		private:
			char m_dearchiverWorkBuffer[SCE_FIOS_PSARC_DEARCHIVER_WORK_BUFFER_SIZE]
				__attribute__((aligned(SCE_FIOS_PSARC_DEARCHIVER_WORK_BUFFER_ALIGNMENT)));
			int m_archiveIndex;
			SceFiosPsarcDearchiverContext m_dearchiverContext;
			SceFiosBuffer m_mountBuffer;
			SceFiosFH m_archiveFH;
		public:
			int mount(const std::string &archiveName, const std::string mountPoint);
			int unmount(void);

		};
	}
}