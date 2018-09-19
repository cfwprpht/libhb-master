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
#include "c_console.h"
#include "ps4_forms.h"
#include "console.h"

using namespace PS4Forms;

extern "C" PRX_INTERFACE char *ReadLine(void) { return Console::ReadLine(); }

extern "C" PRX_INTERFACE void WriteLine(const char *format, ...) {
	char buf[1000];                                           // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, format);                                   // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), format, args);                // Format the string.
	TTY::onScreenPrintf(buf);                                 // Push buffer to tty.
	va_end(args);                                             // Close the variable argument list.
}

extern "C" PRX_INTERFACE void WriteError(const char *msg, ...) {
	char buf[1000];                                           // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, msg);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), msg, args);                   // Format the string.
	TTY::onScreenPrintf(TTY::tty_text_color_error, buf);      // Push buffer to tty.
	va_end(args);
}

extern "C" PRX_INTERFACE void WriteWarning(const char *msg, ...) {
	char buf[1000];                                           // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, msg);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), msg, args);                   // Format the string.
	TTY::onScreenPrintf(TTY::tty_text_color_warning, buf);    // Push buffer to tty.
	va_end(args);
}

extern "C" PRX_INTERFACE void WriteColor(uint32_t color, const char *msg, ...) {
	char buf[1000];                                           // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, msg);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), msg, args);                   // Format the string.
	TTY::onScreenPrintf(color, buf);                          // Push buffer to tty.
	va_end(args);                                             // Close the variable argument list.
}

extern "C" PRX_INTERFACE void LineBreak(void) {
	char buf[3];                                              // Initialize a buffer.
	sprintf(buf, "%s", "\n");                                 // Add the Linebreak.
	TTY::onScreenPrintf(buf);                                 // Push buffer to tty.
}

extern "C" PRX_INTERFACE void DeleteLastLine(void) { TTY::deleteLastLine(); }

extern "C" PRX_INTERFACE char *GetSelectedLine(void) { return TTY::getSelectedLine(); }

extern "C" PRX_INTERFACE int GetSelectedLineIndex(void) { return TTY::getSelectedLineIndex();}

extern "C" PRX_INTERFACE void SetSelectedLine(char *line) { TTY::setSelectedLine(line); }

extern "C" PRX_INTERFACE void SetSelectedLineIndex(unsigned int line) { TTY::setSelectedLineIndex(line); }

extern "C" PRX_INTERFACE void Pause(void) { Console::Pause(); }