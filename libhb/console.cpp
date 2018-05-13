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
extern "C" {
#include "kconsole.h"
}

// Kernel Console Userland Buffer. It's set up here to ensure it's really a userland buffer.
static char kcon_usrld_buffer[264];
static char kcon_usrld_error_buffer[264];
static char kcon_usrld_warning_buffer[264];

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

// Write a single, centered message to the screen.
void LibHomebrew::Console::SingleLine(const char *msg, ...) {
	va_list args;                                                     // Initialize a variable list.
	va_start(args, msg);                                              // Get the args into a type specific list.
	memset(singleCenterBuff, 0, sizeof(singleCenterBuff));
	vsnprintf(singleCenterBuff, sizeof(singleCenterBuff), msg, args); // Format the string.
	va_end(args);                                                     // Close the variable argument list.
}

// Add a Line Break.
void LibHomebrew::Console::LineBreak(void) {
	memset(singleCenterBuff, 0, sizeof(singleCenterBuff));
	sprintf(singleCenterBuff, "\n");
}

// Clear a single, centered message.
void LibHomebrew::Console::SingleLineClear(void) { memset(singleCenterBuff, 0, sizeof(singleCenterBuff)); }

// Write a Line with your own color.
void LibHomebrew::Console::WriteColor(uint32_t color, const char *msg, ...) {
	char buf[512];                                            // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, msg);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), msg, args);                   // Format the string.
	TTY::onScreenPrintf(color, buf);                          // Push buffer to tty.
	va_end(args);                                             // Close the variable argument list.
}

// The actual Reader function. Checks the C char buffer from kconsole source for data and writes it out to the userland console.
void *Reader(void *) {
	LibHomebrew::Console::WriteLine("Hello from the Reader Thread !\n");
	while (!stopThread) {
		if (strlen(kcon_usrld_buffer) > 0) {
			LibHomebrew::Console::WriteLine("Got message.\n");
			LibHomebrew::Console::WriteLine(kcon_usrld_buffer);
			memset(kcon_usrld_buffer, 0, sizeof(kcon_usrld_buffer));
		}
		if (strlen(kcon_usrld_error_buffer) > 0) {
			LibHomebrew::Console::WriteLine("Got error.\n");
			LibHomebrew::Console::WriteError(kcon_usrld_error_buffer);
			memset(kcon_usrld_error_buffer, 0, sizeof(kcon_usrld_error_buffer));
		}
		if (strlen(kcon_usrld_warning_buffer) > 0) {
			LibHomebrew::Console::WriteLine("Got warning.\n");
			LibHomebrew::Console::WriteWarning(kcon_usrld_warning_buffer);
			memset(kcon_usrld_warning_buffer, 0, sizeof(kcon_usrld_warning_buffer));
		}
	}
	isRunning = false;
	LibHomebrew::Console::WriteLine("ByBy from the Reader Thread !\n");
	return 0;
}

// Instance Initializer.
LibHomebrew::KConsole::KConsole() { 
	// Clear Buffer.
	memset(kcon_usrld_buffer, 0, sizeof(kcon_usrld_buffer));
	memset(kcon_usrld_error_buffer, 0, sizeof(kcon_usrld_error_buffer));
	memset(kcon_usrld_warning_buffer, 0, sizeof(kcon_usrld_warning_buffer));
	
	// Point the Kernel Console Buffer, which we use to write into userland, to our userland one.
	kcon_buffer         = kcon_usrld_buffer;
	kcon_error_buffer   = kcon_usrld_error_buffer;
	kcon_warning_buffer = kcon_usrld_warning_buffer;

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
LibHomebrew::KConsole::~KConsole() { 
	LibHomebrew::Console::WriteLine("Stopping Reader Thread.\n");
	if (isRunning) stopThread = true;
	while (isRunning) { /* Nothing to do here, just wait for the Thread to be exited cracefully. */ }
	for (int i = 0; i < 50; i++) { /* Give the Thread function some milli secs to return. */ }
}

// Closing the Instance.
void LibHomebrew::KConsole::Close(void) { this->~KConsole(); }