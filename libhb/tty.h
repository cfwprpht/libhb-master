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

#include <string>
#include <stdint.h>
#include <sampleutil.h>

#define DISPLAY_WIDTH	1920
#define DISPLAY_HEIGHT	1080

///////////////////////////////
// Menu configurations
///////////////////////////////

#define MENU_CONFIG_CLEAR_COLOUR					0x00000000									// The menu background colour (ARGB)
#define MENU_CONFIG_ITESELECTED_COLOUR			0x00FF00FF									// Colour of text when highlighted or selected (RGBA)
#define MENU_CONFIG_ITEUNSELECTED_COLOUR			0xFF4400FF									// Colour of text when not highlighted (RGBA)

#define MENU_CONFIG_ITESELECTED_SCALE				0.035f										// Scale of selected menu item
#define MENU_CONFIG_ITEUNSELECTED_SCALE			0.025f										// Scale of unselected menu item

#define MENU_CONFIG_ITEX_MIN_CHARACTER_OFFSET		14											// Minimum character spacing
#define MENU_CONFIG_ITEX_OFFSET					0.35f * MENU_CONFIG_ITEUNSELECTED_SCALE	// Spacing in between columns
#define MENU_CONFIG_ITEY_OFFSET					0.035f										// Spacing in between rows

#define MENU_TTY_BACKGROUND_COLOUR_FOCUSED			0x758AFFFF									// The TTY background colour, when selected (RGBA)
#define MENU_TTY_BACKGROUND_COLOUR_UNFOCUSED		0x999999FF									// The TTY background colour, when unselected (RGBA)
#define MENU_TTY_TEXT_COLOUR						0xFFFFFFFF									// The TTY text colour (RGBA) [White]
#define MENU_TTY_TEXT_COLOUR_WARNING				0xFFFF11FF									// The TTY text colour for a warning (RGBA)
#define MENU_TTY_TEXT_COLOUR_ERROR					0xFF0000FF									// The TTY text colour for an error (RGBA)

#define MENU_TTY_TEXT_Y_OFFSET						0.04f										// The TTY text scale
#define MENU_TTY_TEXT_SCALE							0.03f										// The TTY text scale

#define MENU_TTY_SCREEN_PROPORTION					0.96f										// Proportion of the screen TTY takes up, from 0.0f to 1.0f
#define MENU_TTY_MAX_CHARACTERS_PER_LINE			100											// Maximum characters per line
#define MENU_TTY_MAX_LINES							1024										// Maximum lines stored in the TTY
#define MENU_TTY_SCROLL_BAR_SIZE					0.025f										// Maximum lines stored in the TTY

class TTY {
public:
	struct TtyItem {
		char		buffer[MENU_TTY_MAX_CHARACTERS_PER_LINE];
		uint32_t	colour;
		bool		canOverwrite;

		TtyItem() : colour(MENU_TTY_TEXT_COLOUR), canOverwrite(false) {
			memset(buffer, 0x0, MENU_TTY_MAX_CHARACTERS_PER_LINE);
		}
	};

	TTY();
	~TTY();

	// Adds text to the buffer
	static void onScreenPrintf(const char *text, ...);

	// Adds text to the buffer with a certain colour
	static void onScreenPrintf(const uint32_t &colour, const char *text, ...);

	// Take user input and updates things
	static void update(sce::SampleUtil::Input::PadContext * controller);

	// Renders strings to screen
	static void render(sce::SampleUtil::Graphics::SpriteRenderer * renderer, sce::SampleUtil::Graphics::GraphicsContext * context);

	// Toggles TTY focus
	static void toggleFocus() { _isFocused = !_isFocused; }

	// Is TTY in focus ?
	static bool isFocused() { return _isFocused; }

	// Clear the TTY
	static void clear();

private:
	// Buffer cotaining all TTY
	static TtyItem buffer[MENU_TTY_MAX_LINES];

	// Index in TTY array that we can write to next. Wraps around when MENU_TTY_MAX_LINES is reached
	static uint32_t writePos;

	// Index in TTY that should be drawn first
	static int32_t HeadDrawPos;

	// Total number of lines written to TTY
	static uint32_t totalLinesWritten;

	// Maximum amount of lines that can be drawn on screen
	static int32_t maxLinesDrawNum;

	// Is the TTY in focus ?
	static bool _isFocused;
};