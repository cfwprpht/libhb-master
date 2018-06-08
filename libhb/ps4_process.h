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

#include <_types.h>

namespace LibHomebrew {
	namespace Loot {
		class Proc {
		private:
			static bool verbose;

		public:
			static bool LoadExec(const char *path, char *const *argv);
			static int Kexec(const char *path);
			static int Kexec(void *buffer);
			static int Freedom(void);
			static int EnableSUDO(void);
			static int DisableSUDO(void);
			static int DisableProcASLR(void);
			static int EnableBrowser(void);
			static void Reboot(void);
			static void Verbose(bool state);
		};
	}
}