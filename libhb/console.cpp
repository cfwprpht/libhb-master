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

#include "console.h"
#include "defines.h"
#include "tty.h"
#include <sstream>
#include <_types.h>

// Including the pure C kernel console source header.
/*extern "C" {
#include "../kern_console_sample/kern_console_sample/kern_con_sample.h"
}*/

ScePthread LibHomebrew::Console::reader;
char LibHomebrew::Console::singleCenterBuff[128];

// Thread flags.
static bool stopThread = false;
static bool isRunning = false;

// Write a Line to the screen.
void LibHomebrew::Console::WriteLine(const char *msg, ...) {
	char buf[512];                                            // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, msg);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), msg, args);                   // Format the string.
	TTY::onScreenPrintf(buf);                                 // Push buffer to tty.
	va_end(args);                                             // Close the variable argument list.
}

// Write a red colored Line to the screen.
void LibHomebrew::Console::WriteError(const char *msg, ...) {
	char buf[512];                                            // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, msg);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), msg, args);                   // Format the string.
	TTY::onScreenPrintf(MENU_TTY_TEXT_COLOUR_ERROR, buf);     // Push buffer to tty.
	va_end(args);                                             // Close the variable argument list.
}

// Write a yellow colored Line to the screen.
void LibHomebrew::Console::WriteWarning(const char *msg, ...) {
	char buf[512];                                            // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, msg);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), msg, args);                   // Format the string.
	TTY::onScreenPrintf(MENU_TTY_TEXT_COLOUR_WARNING, buf);   // Push buffer to tty.
	va_end(args);                                             // Close the variable argument list.
}

// Add a Line Break.
void LibHomebrew::Console::LineBreak(void) {
	char buf[3];                                              // Initialize a buffer.
	sprintf(buf, "%s", "\n");                                 // Add the Linebreak.
	TTY::onScreenPrintf(buf);                                 // Push buffer to tty.
}

// Write a Line with your own color.
void LibHomebrew::Console::WriteColor(uint32_t color, const char *msg, ...) {
	char buf[512];                                            // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, msg);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), msg, args);                   // Format the string.
	TTY::onScreenPrintf(color, buf);                          // Push buffer to tty.
	va_end(args);                                             // Close the variable argument list.
}

// Write a single, centered message to the screen.
void LibHomebrew::Console::SingleLine(const char *msg, ...) {
	va_list args;                                                     // Initialize a variable list.
	va_start(args, msg);                                              // Get the args into a type specific list.
	memset(singleCenterBuff, 0, sizeof(singleCenterBuff));
	vsnprintf(singleCenterBuff, sizeof(singleCenterBuff), msg, args); // Format the string.
	va_end(args);                                                     // Close the variable argument list.
}

// Clear a single, centered message.
void LibHomebrew::Console::SingleLineClear(void) { memset(singleCenterBuff, 0, sizeof(singleCenterBuff)); }

// The actual Reader function. Checks the C char buffer from kconsole source for data and writes it out to the userland console.
/*void *Reader(void *) {
	LibHomebrew::Console::WriteLine("Hello from the Reader Thread !\n");

	// Set up Buffers and clear them.
	char msg[264];
	char errmsg[264];
	char warnmsg[264];
	memset(&msg, 0, sizeof(msg));
	memset(&errmsg, 0, sizeof(errmsg));
	memset(&warnmsg, 0, sizeof(warnmsg));

	// Loop over the buffers.
	while (!stopThread) {
		if (isMsgPresent() > 0) {
			LibHomebrew::Console::WriteLine("Got message.\n");
			getMsg(msg);
			LibHomebrew::Console::WriteLine(msg);
			LibHomebrew::Console::LineBreak();
			memset(&msg, 0, sizeof(msg));
		}
		if (isErrorMsgPresent() > 0) {
			LibHomebrew::Console::WriteLine("Got error.\n");
			getErrorMsg(errmsg);
			LibHomebrew::Console::WriteError(errmsg);
			LibHomebrew::Console::LineBreak();
			memset(&errmsg, 0, sizeof(errmsg));
		}
		if (isWarningMsgPresent() > 0) {
			LibHomebrew::Console::WriteLine("Got warning.\n");
			getWarningMsg(warnmsg);
			LibHomebrew::Console::WriteWarning(warnmsg);
			LibHomebrew::Console::LineBreak();
			memset(&warnmsg, 0, sizeof(warnmsg));
		}
	}
	isRunning = false;
	LibHomebrew::Console::WriteLine("ByBy from the Reader Thread !\n");
	return 0;
}

// Instance Initializer.
void LibHomebrew::Console::KConsoleOn(void) {
	// Setup The Reader Thread.
	if (scePthreadCreate(&reader, NULL, Reader, NULL, "KConsoleReader") != SCE_OK) {
		Console::WriteError("Couldn't start KConsole Reader Thread.\n");
		isRunning = false;
	} else {
		stopThread = false;
		isRunning = true;
		Console::WriteLine("KCon Reader Thread startet.\n");
	}
}

// Instance Deinitializer.
void LibHomebrew::Console::KConsoleOff(void) { 
	Console::WriteLine("Stopping Reader Thread.\n");
	if (isRunning) stopThread = true;
	while (isRunning) { /* Nothing to do here, just wait for the Thread to be exited cracefully.  }
	for (int i = 0; i < 50; i++) { /* Give the Thread function some milli secs to return.  }
}*/