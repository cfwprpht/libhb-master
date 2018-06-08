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

#include <stdio.h>
#include <sstream>
#include <_types.h>
#include <_pthread.h>
#include <sampleutil.h>

namespace ssi = sce::SampleUtil::Input;

namespace LibHomebrew {
	class Console {
	private:
		static bool stopThread;
		static ScePthread reader;
		static ScePthread writer;
		static ssi::Button input;

		static void *Reader(void *);
		static void *Writer(void *);

	public:
		static char singleCenterBuff[128];

		Console() {}
		~Console(){}
		static void WriteLine(const char *msg, ...);
		static void WriteError(const char *msg, ...);
		static void WriteWarning(const char *msg, ...);
		static void SingleLine(const char *msg, ...);		
		static void WriteColor(uint32_t color, const char *msg, ...);
		static void SingleLineClear(void);
		static void LineBreak(void);
		static void SetUserInput(ssi::Button usrIn);
		static bool RunKConsole(bool rw);
		static void StopKConsole(void);
	};
}