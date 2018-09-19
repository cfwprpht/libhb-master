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

#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif

namespace LibHomebrew {
	namespace Loot {
		class PRX_INTERFACE Proc {
		private:
			static bool verbose;

		public:
			static bool SysLoadExec(const char *path, char *const *argv);
			static int UserLoadExec(const char *path, void * arg1, void *arg2);
			static int LoadExecBin(const char *path, void * arg1, void *arg2);
			static int Kexec(const char *path);
			static int Kexec(void *buffer);
			static int Freedom(void);
			static int EnableMMAPSelf(void);
			static int DisableMMAPSelf(void);
			static int DisableProcASLR(void);
			static int EnableBrowser(void);
			static void Reboot(void);
			static void Verbose(bool state);
		};
	}
}