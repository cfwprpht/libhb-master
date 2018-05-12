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

#include "tty.h"
#include "swiss_knife.h"
#include <stdarg.h>

using namespace sce::Vectormath::Simd::Aos;
using namespace LibHomebrew::Loot;

TTY::TtyItem                TTY::buffer[MENU_TTY_MAX_LINES];
uint32_t                    TTY::writePos = 0;
bool                        TTY::_isFocused = false;
int32_t                     TTY::maxLinesDrawNum = (MENU_TTY_SCREEN_PROPORTION / MENU_TTY_TEXT_SCALE);
int32_t                     TTY::HeadDrawPos = -maxLinesDrawNum;
uint32_t                    TTY::totalLinesWritten = 0;

TTY::TTY() {}
TTY::~TTY() {}

void TTY::onScreenPrintf(const char *text, ...) {
	int len = 0;
	va_list argptr;
	int startIndex = 0;
	bool newlinefound = false;
	int bufferLen = strlen(buffer[writePos].buffer);

	// Make a temp buffer big enough for a reasonable amount of input
	// 10 lines should be more than enough, anything over will be dropped.
	char tempBuffer[MENU_TTY_MAX_CHARACTERS_PER_LINE * 10] = { '\0' };

	buffer[writePos].colour = MENU_TTY_TEXT_COLOUR;

	va_start(argptr, text);
	vsnprintf(tempBuffer, MENU_TTY_MAX_CHARACTERS_PER_LINE * 10, text, argptr);
	va_end(argptr);

	// Also print out to UART
	// printf("%s", tempBuffer);

	len = strlen(tempBuffer);

	for (int i = 0; i < len; i++) {
		if (tempBuffer[i] == '\n' || (i - startIndex + bufferLen) >= MENU_TTY_MAX_CHARACTERS_PER_LINE - 1) {
			if (buffer[writePos].canOverwrite) {
				buffer[writePos].buffer[0] = '\0';
				buffer[writePos].canOverwrite = false;
			}

			strncat(buffer[writePos].buffer, tempBuffer + startIndex + (startIndex ? 1 : 0), i - startIndex);
			newlinefound = (tempBuffer[i] == '\n' ? true : false);

			writePos++;
			HeadDrawPos++;
			totalLinesWritten++;
			buffer[writePos].colour = MENU_TTY_TEXT_COLOUR;

			if ((i - startIndex + bufferLen) >= MENU_TTY_MAX_CHARACTERS_PER_LINE - 1) {
				char subString[len - i];
				memcpy(subString, tempBuffer + i, len - i);
				memset(tempBuffer, '\0', MENU_TTY_MAX_CHARACTERS_PER_LINE * 10);
				memcpy(tempBuffer, subString, len - i);
				len = strlen(tempBuffer);
				bufferLen = strlen(buffer[writePos].buffer);
				startIndex = 0;
				i = -1;
			} else startIndex = i;
		}
	}

	// If we didn't find any newlines or the last character wasn't a newline
	if (startIndex == 0 && !newlinefound) {
		if ((len + bufferLen) >= MENU_TTY_MAX_CHARACTERS_PER_LINE) {
			writePos++;
			HeadDrawPos++;
			totalLinesWritten++;
			buffer[writePos].colour = MENU_TTY_TEXT_COLOUR;
		} else strncat(buffer[writePos].buffer, tempBuffer, len);
	}

	if (writePos >= MENU_TTY_MAX_LINES) {
		writePos = 0;
		for (int i = 0; i < MENU_TTY_MAX_LINES; i++) buffer[i].canOverwrite = true;
	}

	if (HeadDrawPos >= MENU_TTY_MAX_LINES) HeadDrawPos = 0;
}

void TTY::onScreenPrintf(const uint32_t &colour, const char *text, ...) {
	int len = 0;
	va_list argptr;
	int startIndex = 0;
	bool newlinefound = false;
	int bufferLen = strlen(buffer[writePos].buffer);

	// Make a temp buffer big enough for a reasonable amount of input
	// 10 lines should be more than enough, anything over will be dropped.
	char tempBuffer[MENU_TTY_MAX_CHARACTERS_PER_LINE * 10] = { '\0' };

	buffer[writePos].colour = colour;

	va_start(argptr, text);
	vsnprintf(tempBuffer, MENU_TTY_MAX_CHARACTERS_PER_LINE * 10, text, argptr);
	va_end(argptr);

	// Also print out to UART
	//printf("%s", tempBuffer);

	len = strlen(tempBuffer);

	for (int i = 0; i < len; i++) {
		if (tempBuffer[i] == '\n' || (i - startIndex + bufferLen) >= MENU_TTY_MAX_CHARACTERS_PER_LINE - 1) {
			if (buffer[writePos].canOverwrite) {
				buffer[writePos].buffer[0] = '\0';
				buffer[writePos].canOverwrite = false;
			}

			strncat(buffer[writePos].buffer, tempBuffer + startIndex + (startIndex ? 1 : 0), i - startIndex);
			newlinefound = (tempBuffer[i] == '\n' ? true : false);

			writePos++;
			HeadDrawPos++;
			totalLinesWritten++;
			buffer[writePos].colour = colour;

			if ((i - startIndex + bufferLen) >= MENU_TTY_MAX_CHARACTERS_PER_LINE - 1) {
				char subString[len - i];
				memcpy(subString, tempBuffer + i, len - i);
				memset(tempBuffer, '\0', MENU_TTY_MAX_CHARACTERS_PER_LINE * 10);
				memcpy(tempBuffer, subString, len - i);
				len = strlen(tempBuffer);
				bufferLen = strlen(buffer[writePos].buffer);
				startIndex = 0;
				i = -1;
			} else startIndex = i;
		}
	}

	// If we didn't find any newlines or the last character wasn't a newline
	if (startIndex == 0 && !newlinefound) {
		if ((len + bufferLen) >= MENU_TTY_MAX_CHARACTERS_PER_LINE) {
			writePos++;
			HeadDrawPos++;
			totalLinesWritten++;
			buffer[writePos].colour = colour;
		} else strncat(buffer[writePos].buffer, tempBuffer, len);
	}

	if (writePos >= MENU_TTY_MAX_LINES) {
		writePos = 0;
		for (int i = 0; i < MENU_TTY_MAX_LINES; i++) buffer[i].canOverwrite = true;
	}

	if (HeadDrawPos >= MENU_TTY_MAX_LINES) HeadDrawPos = 0;
}

void TTY::update(sce::SampleUtil::Input::PadContext *controller) {
	// Quick and dirty: Only sample the controller one in four times to slow things down.
	static int num = 0;
	if (num++ % 4) return;

	if (controller->isButtonDown(sce::SampleUtil::Input::kButtonUp, sce::SampleUtil::Input::kButtonEventPatternAny)) {
		if (HeadDrawPos > 0) HeadDrawPos--;
	} else if (controller->isButtonDown(sce::SampleUtil::Input::kButtonDown, sce::SampleUtil::Input::kButtonEventPatternAny)) {
		if (HeadDrawPos < ((totalLinesWritten < MENU_TTY_MAX_LINES) ? totalLinesWritten : MENU_TTY_MAX_LINES) - maxLinesDrawNum) {
			HeadDrawPos++;
		}
	}
}

void TTY::render(sce::SampleUtil::Graphics::SpriteRenderer *renderer, sce::SampleUtil::Graphics::GraphicsContext *context) {
	// Draw background coloured box
	/*renderer->fillRect(context,
		Vector2(0.0f, 1.0f - MENU_TTY_SCREEN_PROPORTION),
		Vector2(1.0f, MENU_TTY_SCREEN_PROPORTION),
		Vector4(((isFocused ? MENU_TTY_BACKGROUND_COLOUR_FOCUSED : MENU_TTY_BACKGROUND_COLOUR_UNFOCUSED) >> 24 & 0xff) / (float)0xff,
		((isFocused ? MENU_TTY_BACKGROUND_COLOUR_FOCUSED : MENU_TTY_BACKGROUND_COLOUR_UNFOCUSED) >> 16 & 0xff) / (float)0xff,
			((isFocused ? MENU_TTY_BACKGROUND_COLOUR_FOCUSED : MENU_TTY_BACKGROUND_COLOUR_UNFOCUSED) >> 8 & 0xff) / (float)0xff,
			((isFocused ? MENU_TTY_BACKGROUND_COLOUR_FOCUSED : MENU_TTY_BACKGROUND_COLOUR_UNFOCUSED) & 0xff) / (float)0xff));*/

	// Draw scrollbar to show that you can scroll
	if (_isFocused) {
		renderer->fillRect(context, Vector2(0.95f, 1.0f - MENU_TTY_SCREEN_PROPORTION), Vector2(0.001f, MENU_TTY_SCREEN_PROPORTION), Vector4(1.0f));
		renderer->fillRect(context, Vector2(0.95f + MENU_TTY_SCROLL_BAR_SIZE, 1.0f - MENU_TTY_SCREEN_PROPORTION), Vector2(0.001f, MENU_TTY_SCREEN_PROPORTION), Vector4(1.0f));
		renderer->fillRect(context, Vector2(0.95f, 1.0f - MENU_TTY_SCREEN_PROPORTION + (((float)HeadDrawPos / (float)(totalLinesWritten - maxLinesDrawNum)) * MENU_TTY_SCREEN_PROPORTION) * (1.0f - MENU_TTY_SCROLL_BAR_SIZE*2.0f)),
			Vector2(MENU_TTY_SCROLL_BAR_SIZE, MENU_TTY_SCROLL_BAR_SIZE), Vector4(1.0f));
	}

	// Draw the text
	int i = (HeadDrawPos < 0 ? 0 : HeadDrawPos);
	int j = 0;

	for (; i < MENU_TTY_MAX_LINES && j < maxLinesDrawNum; i++, j++) {
		renderer->drawDebugStringf(context,
			Vector2(0.0f, 1.0f - MENU_TTY_SCREEN_PROPORTION + j * MENU_TTY_TEXT_SCALE),
			MENU_TTY_TEXT_SCALE,
			Vector4((buffer[i].colour >> 24 & 0xff) / (float)0xff,
			(buffer[i].colour >> 16 & 0xff) / (float)0xff,
				(buffer[i].colour >> 8 & 0xff) / (float)0xff,
				(buffer[i].colour & 0xff) / (float)0xff),
			buffer[i].buffer);
	}

	for (i = 0; i < HeadDrawPos && j < maxLinesDrawNum; i++, j++) {
		renderer->drawDebugStringf(context,
			Vector2(0.0f, 1.0f - MENU_TTY_SCREEN_PROPORTION + j * MENU_TTY_TEXT_SCALE),
			MENU_TTY_TEXT_SCALE,
			Vector4((buffer[i].colour >> 24 & 0xff) / (float)0xff,
			(buffer[i].colour >> 16 & 0xff) / (float)0xff,
				(buffer[i].colour >> 8 & 0xff) / (float)0xff,
				(buffer[i].colour & 0xff) / (float)0xff),
			buffer[i].buffer);
	}
}

void TTY::clear() {
	for (int i = 0; i < (MENU_TTY_MAX_LINES - 1); i++) {
		if (strlen(buffer[i].buffer) > 0) memset(buffer[i].buffer, 0, MENU_TTY_MAX_CHARACTERS_PER_LINE);
	}
	writePos = 0;
	HeadDrawPos = -maxLinesDrawNum;
	totalLinesWritten = 0;
}