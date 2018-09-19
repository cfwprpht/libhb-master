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
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	PRX_INTERFACE void WriteLine(const char *format, ...);
	PRX_INTERFACE char *ReadLine(void);
	PRX_INTERFACE void WriteError(const char *msg, ...);
	PRX_INTERFACE void WriteWarning(const char *msg, ...);
	PRX_INTERFACE void WriteColor(uint32_t color, const char *msg, ...);
	PRX_INTERFACE void LineBreak(void);
	PRX_INTERFACE void DeleteLastLine(void);
	PRX_INTERFACE char *GetSelectedLine(void);
	PRX_INTERFACE int GetSelectedLineIndex(void);
	PRX_INTERFACE void SetSelectedLine(char *line);
	PRX_INTERFACE void SetSelectedLineIndex(unsigned int line);
	PRX_INTERFACE void Pause(void);
#ifdef __cplusplus
}
#endif