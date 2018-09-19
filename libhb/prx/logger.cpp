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

#define LIBRARY_IMPL  (1)

#include <sampleutil.h>
#include "logger.h"
#include "syscalls.h"

bool LibHomebrew::Logger::dbgInit = false;
const char *LibHomebrew::Logger::_pathToLog = nullptr;

/* Instance Initializer */
LibHomebrew::Logger::Logger(const char *path) {
	int fp = Sys::open(path, 0, O_RDONLY);           // Try to open the file.
	if (fp == -1) return;                            // Do file exists ?
	Sys::close(fp);                                  // Close the file.
	pathToLog = strdup(path);                        // Store path so we can write into the file later.
	init = true;                                     // Set to initialized.	
}

/* Initialize. */
int LibHomebrew::Logger::Init(const char *path) {
	if (!init) {
		FILE *fd = fopen(path, "ab+");
		if (!fd) return 1;                               // Do file exists ?
		fclose(fd);                                      // Close the file.
		pathToLog = strdup(path);                        // Store path so we can write into the file later.
		init = true;                                     // Set to initialized.		
	}
	return SCE_OK;                                       // Return success.
}

/* Write a message to the log. */
int LibHomebrew::Logger::Log(const char *msg, ...) {
	if (!init) return 1;                            // Check if logger would be initialized else we return.
	FILE *fd = fopen(pathToLog, "a");
	if (fd) {
		va_list args;
		va_start(args, msg);
		vfprintf(fd, msg, args);
		va_end(args);
		fclose(fd);
		return SCE_OK;
	}
	return 1;
}

/* Return the initialisation state. */
bool LibHomebrew::Logger::GetInitState(void) { return init; }

/* Free resources. */
void LibHomebrew::Logger::Deinit() { delete pathToLog; }