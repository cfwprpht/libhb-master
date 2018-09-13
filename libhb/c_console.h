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
#include <cstdio>
#include <_types.h>

#ifdef LIBRARY_IMPL
#define __declspec(dllexport)
#else
#define __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	void WriteLine(const char *format, ...);
	char *ReadLine(void);
	void WriteError(const char *msg, ...);
	void WriteWarning(const char *msg, ...);
	void WriteColor(uint32_t color, const char *msg, ...);
	void LineBreak(void);
	void DeleteLastLine(void);
	char *GetSelectedLine(void);
	int GetSelectedLineIndex(void);
	void SetSelectedLine(char *line);
	void SetSelectedLineIndex(unsigned int line);
	void Pause(void);
#ifdef __cplusplus
}
#endif