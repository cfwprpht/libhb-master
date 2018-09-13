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

#ifdef LIBRARY_IMPL
#define __declspec(dllexport)
#else
#define __declspec(dllimport)
#endif

namespace LibHomebrew {
	class Logger {
	private:
		const char *pathToLog;
		static const char *_pathToLog;
		bool init    = false;
		static bool debug;
		static bool dbgInit;

	public:
		Logger(const char *path);
		Logger() {}
		~Logger() {}
		int Init(const char *path);
		static int InitDbg(const char *path);
		int Log(const char *msg, ...);
		static int Debug(const char *msg, ...);
		static void UseDebug(void);
		bool GetInitState(void);
		void Deinit();
	};
}