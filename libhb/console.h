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
#include "ps4_forms.h"

#ifdef LIBRARY_IMPL
#define __declspec(dllexport)
#else
#define __declspec(dllimport)
#endif

using namespace PS4Forms;
namespace ssi = sce::SampleUtil::Input;

namespace LibHomebrew {
	class Console {
	private:
		static bool stopThread;
		static ScePthread reader;
		static ScePthread writer;
		static ssi::Button input;
		static bool isPaused;

		static void *Reader(void *);
		static void *Writer(void *);

	public:
		static char singleCenterBuff[128];

		Console() {}
		~Console() {}
		static char *ReadLine(void);
		static void WriteLine(const char *msg, ...);
		static void WriteError(const char *msg, ...);
		static void WriteWarning(const char *msg, ...);
		static void SingleLine(const char *msg, ...);
		static void WriteColor(uint32_t color, const char *msg, ...);
		static char *GetSelectedLine(void);
		static int GetSelectedLineIndex(void);
		static void SetSelectedLine(char *line);
		static void SetSelectedLineIndex(unsigned int line);
		static void SingleLineClear(void);
		static void Clear(void);
		static void Pause(void);
		static void EnableHighlighting(void);
		static void DisableHighlighting(void);
		static void LineBreak(void);
		static void DeleteLastLine(void);
		static void SetXCoordinate(float value);
		static void SetYCoordinate(float value);
		static float GetXCoordinate(void);
		static float GetYCoordinate(void);
		static void SetUserInput(ssi::Button usrIn);
		static bool RunKConsole(bool rw);
		static void StopKConsole(void);
	};
}