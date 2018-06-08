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

extern "C" {
#include "lv2.h"
}

// Buffer for the singlen centered message.
char LibHomebrew::Console::singleCenterBuff[128];

// Internal flag. Used to stop the reader thread gracefully.
bool LibHomebrew::Console::stopThread = false;

// The Reader Thread.
ScePthread LibHomebrew::Console::reader;

// The Writer Thread.
ScePthread LibHomebrew::Console::writer;

// The User Button Input.
ssi::Button LibHomebrew::Console::input;

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

// Sets the user input into the console class.
void LibHomebrew::Console::SetUserInput(ssi::Button usrIn) { input = usrIn; }

// The Reader Thread Function to run in.
void *LibHomebrew::Console::Reader(void *) {
	// Set up Buffers and clear them.
	char msg[1024];
	char errmsg[1024];
	char warnmsg[1024];
	memset(msg, 0, sizeof(msg));
	memset(errmsg, 0, sizeof(errmsg));
	memset(warnmsg, 0, sizeof(warnmsg));

	// Loop over the buffers.
	while (!stopThread) {
		if (kconsole.getMsg(msg) > 0) {
			Console::WriteLine(msg);
			memset(msg, 0, sizeof(msg));
		}
		if (kconsole.getErrorMsg(errmsg) > 0) {
			Console::WriteError(errmsg);
			memset(errmsg, 0, sizeof(errmsg));
		}
		if (kconsole.getWarningMsg(warnmsg) > 0) {
			Console::WriteWarning(warnmsg);
			memset(warnmsg, 0, sizeof(warnmsg));
		}
	}
	return 0;
}

// The Reader Thread Function to run in.
void *LibHomebrew::Console::Writer(void *) {
	// Set up Buffer and clear.
	char msg[10];
	memset(msg, 0, sizeof(msg));

	// Loop over user input.
	while (!stopThread) {
		// If user made a input.
		if (input != ssi::kButtonNone) {
			// Switch the input and set the buffer.
			ssi::Button caseSwitch = input;
			input = ssi::kButtonNone;
			switch (caseSwitch) {
				case ssi::kButtonCross:
					sprintf(msg, "%s", "CROSS");
					break;
				case ssi::kButtonTriangle:
					sprintf(msg, "%s", "TRIANGLE");
					break;
				case ssi::kButtonCircle:
					sprintf(msg, "%s", "CIRCLE");
					break;
				case ssi::kButtonSquare:
					sprintf(msg, "%s", "SQUARE");
					break;
				case ssi::kButtonL1:
					sprintf(msg, "%s", "L1");
					break;
				case ssi::kButtonL2:
					sprintf(msg, "%s", "L2");
					break;
				case ssi::kButtonL3:
					sprintf(msg, "%s", "L3");
					break;
				case ssi::kButtonR1:
					sprintf(msg, "%s", "R1");
					break;
				case ssi::kButtonR2:
					sprintf(msg, "%s", "R2");
					break;
				case ssi::kButtonR3:
					sprintf(msg, "%s", "R3");
					break;
				case ssi::kButtonLeft:
					sprintf(msg, "%s", "LEFT");
					break;
				case ssi::kButtonRight:
					sprintf(msg, "%s", "RIGHT");
					break;
				case ssi::kButtonTouchPad:
					sprintf(msg, "%s", "TOUCHPAD");
					break;
				case ssi::kButtonOptions:
					sprintf(msg, "%s", "OPTIONS");
					break;
				default:
					break;
			}

			// Overload the message to the kernel console now and clear the buffer then.
			if (strlen(msg) > 0) {
				kconsole.setMsg(msg);
				memset(msg, 0, sizeof(msg));
			}
		}
	}
	return 0;
}

// Run Kernel Console Reader Thread.
bool LibHomebrew::Console::RunKConsole(bool rw) {
	// Initialize Kernel Console.
	kconsole_init();
	
	// Setup The Reader Thread.
	if (scePthreadCreate(&reader, NULL, Reader, NULL, "KConsoleReader") != SCE_OK) {
		Console::WriteError("Couldn't start KConsole Reader Thread.\n");
		return false;
	} else if (rw) { // Shall we use the Writer Thread too ?
		if (scePthreadCreate(&writer, NULL, Writer, NULL, "KConsoleWriter") != SCE_OK) {
			Console::WriteError("Couldn't start KConsole Writer Thread.\n");
			return false;
		}
	}
	return true;
}

// Stop Kernel Console Reader Thread.
void LibHomebrew::Console::StopKConsole(void) { stopThread = true; }