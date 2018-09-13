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

#include <algorithm>
#include "ps4_forms.h"
#include "logger.h"

using namespace LibHomebrew;

/*-----------------------------------------------------------------
PS4Forms Namespace
Static Functions
------------------------------------------------------------------*/
// Calculate if the cursor/stick is within the Range of this object.
bool IsObjectFocus(float x, float y, Position _position, Size _size) {
	if ((x >= _position.getX()) && (x <= (_position.getX() + _size.getY()))) {
		if ((y >= _position.getY()) && (y <= (_position.getY() + _size.getX()))) {
			return true;
		}
	}
	return false;
}

/*-----------------------------------------------------------------
Thread Class
------------------------------------------------------------------*/
// Mount a function to the symbol.
void PS4Forms::ThreadFunc::Add(off_t *func) { Func = (void *(*)(void *))func; }

/* Ready to use */
#pragma region WidgetBase
/*-----------------------------------------------------------------
                            Widget Base
-------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::WidgetBase::WidgetBase() {
	show = focus = isFocused = false;
	visible = true;
	_id = strdup("");
	_position = Position(0, 0);
	_size = Size(0, 0);
	_color = WHITE;
}

// Class Initializer.
PS4Forms::WidgetBase::WidgetBase(const char *id, Position position, Size size, Color color) {
	show = focus = isFocused = false;
	visible = true;
	_id = strdup(id);
	_position = position;
	_size = size;
	_color = color;
}

// Class Deinitializer.
PS4Forms::WidgetBase::~WidgetBase() { free(_id); }

// Get Text aka Title of this form.
char *PS4Forms::WidgetBase::getId(void) {
	if (_id != nullptr) return strdup(_id);
	else return strdup("");
}

// Set Text aka Title of this form.
void PS4Forms::WidgetBase::setId(const char *text) {
	if (_id != nullptr) free(_id);
	_id = strdup(text);
}

// Set Position of this form.
void PS4Forms::WidgetBase::setPosition(Position position) { _position = position; }

// Set the Parent Position of this form.
void PS4Forms::WidgetBase::setParentPos(Position parentPos) { _position = Position((parentPos.getX() + _position.getX()), (parentPos.getY() + _position.getY())); }

// Get Position of this form.
Position PS4Forms::WidgetBase::getPosition(void) { return _position; }

// Set Size of this form.
void PS4Forms::WidgetBase::setSize(Size size) { _size = size; }

// Get Size of this form.
Size PS4Forms::WidgetBase::getSize(void) { return _size; }

// Set Color of this form.
void PS4Forms::WidgetBase::setColor(Color color) { _color = color; }

// Get Color of this form.
Color PS4Forms::WidgetBase::getColor(void) { return _color; }

// Set this Object to master focus. Will be drawen at last, so displayed as first.
void PS4Forms::WidgetBase::Focused(bool focused) { isFocused = focused; }

// Show Form.
void PS4Forms::WidgetBase::Show(void) { show = true; }

// Add a function Thread to the funcs vector.
void PS4Forms::WidgetBase::AddFunc(ThreadFunc *func) { if (func != nullptr) funcs.push_back(func); }

// Add a function Thread to the funcs vector.
void PS4Forms::WidgetBase::AddFunc(off_t *func) {
	// Initialize a new ThreadFunc class object.
	ThreadFunc *newFunc = new ThreadFunc();

	// Mount function to the symbol.
	newFunc->Func = (void *(*)(void *))func;

	// Add new psydo click event to the list.
	funcs.push_back(newFunc);
}

// Remove a function Thread from the funcs vector.
void PS4Forms::WidgetBase::RemoveFunc(ThreadFunc *func) {
	if (func != nullptr) {
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if ((*it) == func) (*it)->~ThreadFunc();
			funcs.erase(it);
		}
	}
}

// Remove a function Thread from the funcs vector.
void PS4Forms::WidgetBase::RemoveFunc(off_t *func) {
	if (func != nullptr) {
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if ((*it)->Func == (void *(*)(void *))func) (*it)->~ThreadFunc();
			funcs.erase(it);
		}
	}
}

// Sets label visabillity.
void PS4Forms::WidgetBase::Visible(bool Visible) { visible = Visible; }

// Gets label actual visible state.
bool PS4Forms::WidgetBase::isVisible(void) { return visible; }

// Psydo Event. Will set clicked flag to true if object hase focus.
void PS4Forms::WidgetBase::XPressed(void) {
	if (focus) clicked = true;
	else clicked = false;
}

// Another Psydo Event. Will run all functions within the funcs vector aslong their Thread is not running.
void PS4Forms::WidgetBase::ClickEvent(void) {
	if (clicked) {
		int counter = 0;
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if (counter == 0) {
				if (thread0 == NULL) {
					scePthreadCreate(&thread0, NULL, (*it)->Func, NULL, "WidgetClickEvent0");
				}
			}
			else if (counter == 1) {
				if (thread1 == NULL) {
					scePthreadCreate(&thread1, NULL, (*it)->Func, NULL, "LabelClickEvent1");
				}
			}
			else if (counter == 2) {
				if (thread2 == NULL) {
					scePthreadCreate(&thread2, NULL, (*it)->Func, NULL, "LabelClickEvent2");
				}
			}
			else if (counter == 3) {
				if (thread3 == NULL) {
					scePthreadCreate(&thread3, NULL, (*it)->Func, NULL, "LabelClickEvent3");
				}
			}
			else if (counter == 4) {
				if (thread4 == NULL) {
					scePthreadCreate(&thread4, NULL, (*it)->Func, NULL, "LabelClickEvent4");
				}
			}
			else if (counter == 5) {
				if (thread5 == NULL) {
					scePthreadCreate(&thread5, NULL, (*it)->Func, NULL, "LabelClickEvent5");
				}
			}
			else if (counter == 6) {
				if (thread6 == NULL) {
					scePthreadCreate(&thread6, NULL, (*it)->Func, NULL, "LabelClickEvent6");
				}
			}
			counter++;
		}
		clicked = false;
	}
}

// Hide Form.
void PS4Forms::WidgetBase::Hide(void) { show = false; }

// Close Form.
void PS4Forms::WidgetBase::Close(void) { this->~WidgetBase(); }

// Compares the updated cursor postion against the position and size of the form and will set 'isCursorRange' to true, if the stick, cursor is within our form.
bool PS4Forms::WidgetBase::UpdateCursor(float x, float y) {
	if (IsObjectFocus(x, y, _position, _size)) focus = true;
	else focus = false;
	return focus;
}

// Set the SpriteRenderer.
void PS4Forms::WidgetBase::SetSprite(ssg::SpriteRenderer *sprite) { _sprite = sprite; }

// Get Cursor/stick condition for this form. True means cursor, stick is within the form. False means it is not.
bool PS4Forms::WidgetBase::isFocus(void) { return focus; }

// Get actual show state of this Form. True means the Form is showen, false means the form is hiding.
bool PS4Forms::WidgetBase::isShowen(void) { return show; }

// Set parent of a child.
void PS4Forms::WidgetBase::setParent(WidgetBase *widget) { parent = widget; }

// Drawing the Form and all his childrens if any.
void PS4Forms::WidgetBase::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Draw the Widget.
	if (show & visible) {
		if (widgets.size() > 0) {
			for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
	}
}

// Add a BaseWidget Object.
void PS4Forms::WidgetBase::Add(WidgetBase *child) {
	if (child != nullptr) {
		child->setParent((WidgetBase *)this);
		widgets.push_back(child);
	}
}

// Remove a WidgetBase Object.
void PS4Forms::WidgetBase::Remove(WidgetBase *child) {
	if (child != nullptr) {
		widgets.erase(std::remove(widgets.begin(), widgets.end(), child), widgets.end());
	}
}

// Get a Widget based on his ID.
PS4Forms::WidgetBase *PS4Forms::WidgetBase::getWidgetById(String id) {
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}
#pragma endregion WidgetBase

/* Ready to use */
#pragma region Image
/*-----------------------------------------------------------------
                 Image Class
------------------------------------------------------------------*/
// Instanze initializer.
PS4Forms::Image::Image(ssg::GraphicsLoader *loader, const char *path) {
	graphicsLoader = loader;
	loadTexture(path);
}

// Instanze initializer.
PS4Forms::Image::Image(ssg::GraphicsLoader *loader, String &path) {
	graphicsLoader = loader;
	loadTexture(path);
}

// Instanze deinitializer.
PS4Forms::Image::~Image() { unloadTexture(); }

// Set the graphics loader for this Image class instanze.
void PS4Forms::Image::setGraphicsLoader(ssg::GraphicsLoader *loader) { if (loader != nullptr) graphicsLoader = loader; }

// Load a Texture from path.
int PS4Forms::Image::loadTexture(const String &resourceName) {
	if (_texture != NULL) unloadTexture();
	return graphicsLoader->createTextureFromFile(&_texture, resourceName.c_str());
}

// Load a Texture from path.
int PS4Forms::Image::loadTexture(const char *resourceName) {
	if (_texture != NULL) unloadTexture();
	return graphicsLoader->createTextureFromFile(&_texture, resourceName);
}

// Unload a Texture.
void PS4Forms::Image::unloadTexture(void) { if (_texture) sce::SampleUtil::destroy(_texture); }

// Get pointer to the texture in memory.
ssg::Texture *PS4Forms::Image::getTexture(void) { return _texture; }
#pragma endregion Image

/* Ready to use */
#pragma region Label
/*-----------------------------------------------------------------
                 Label Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::Label::Label() {
	show = focus = isFocused = false;
	visible = true;
	_id = strdup("");
	_text = strdup("");
	_position = Position(0, 0);
	_size = 0.03f;
	_color = WHITE;
}

// Class Initializer.
PS4Forms::Label::Label(const char *id, const char *text, Position position, float size, Color color) {
	show = focus = isFocused = false;
	visible = true;
	_id = strdup(id);
	_text = strdup(text);
	_position = position;
	_size = size;
	_color = color;
}

// Class Deinitializer.
PS4Forms::Label::~Label() { free(_id); free(_text); }

// Set the Label Text to draw.
void PS4Forms::Label::setText(const char *text) {
	if (_text != nullptr) free(_text);
	_text = strdup(text);
}

// Get the label Text to draw.
char *PS4Forms::Label::getText(void) {
	if (_text != nullptr) return strdup(_text);
	else return strdup("");
}

// Drawing the Form and all his childrens if any.
void PS4Forms::Label::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Draw the label.
	if (show & visible) sprite->drawDebugStringf(graphicsContext, _position, _size, _color, _text);
}
#pragma endregion Label

/* Ready to use */
#pragma region TextBox
/*------------------------------------------------------------------
                  TextBox Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::TextBox::TextBox() {
	show = focus = isFocused = false;
	visible = true;
	_id = strdup("");
	_text = strdup("");
	_position = Position(0, 0);
	_size = Size(0.03, 0.3);
	_textSize = CHAR_N;
	_textColor = BLACK;
	_color = WHITE;
}

// Class Initializer.
PS4Forms::TextBox::TextBox(const char *id, const char *text, Position position, Size size, float textSize, Color color, Color textColor) {
	show = focus = isFocused = false;
	visible = true;
	_id = strdup(id);
	_text = strdup(text);
	_position = position;
	_size = size;
	_textSize = textSize;
	_textColor = textColor;
	_color = color;
}

// Class Deinitializer.
PS4Forms::TextBox::~TextBox() { free(_id); }

// Set the TextBox Text to draw.
void PS4Forms::TextBox::setText(const char *text) {
	if (_text != nullptr) free(_text);
	_text = strdup(text);
}

// Get the TextBox Text to draw.
char *PS4Forms::TextBox::getText(void) {
	if (_text != nullptr) return strdup(_text);
	else return strdup("");
}

// Set Text Color of this form.
void PS4Forms::TextBox::setTextColor(Color color) { _textColor = color; }

// Set the Borders Color.
void PS4Forms::TextBox::BorderColor(Color color) { _borderColor = color; }

// Get the Borders Color.
Color PS4Forms::TextBox::getBorderColor(void) { return _borderColor; }

// Get Text Color of this form.
Color PS4Forms::TextBox::getTextColor(void) { return _textColor; }

// Set text size.
void PS4Forms::TextBox::setTextize(float size) { _textSize = size; }

// Get text size.
float PS4Forms::TextBox::getTextSize(void) { return _textSize; }

// Drawing the Form and all his childrens if any.
void PS4Forms::TextBox::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Draw the TextBox.
	if (show & visible) {
		_sprite->fillRect(graphicsContext, _position, _size, WHITE);
		_sprite->drawRect(graphicsContext, _position, _size, BLACK);
		_sprite->drawDebugStringf(graphicsContext, vecmath::Vector2(_position.getX() + 0.01, _position.getY() + 0.01), _textSize, _color, _id);
	}
}
#pragma endregion TextBox

/* Ready to use */
#pragma region PictureBox
/*------------------------------------------------------------------
                    PictureBox Class
-------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::PictureBox::PictureBox() {
	show = focus = clicked = isFocused = false;
	visible = true;
}

// Set Texture of this instance.
void PS4Forms::PictureBox::addImage(ssg::Texture *texture) { _texture = texture; }

// Add a Image to the Form.
void PS4Forms::PictureBox::addImage(Image *image) { _texture = image->getTexture(); }

// Set H Allign.
void PS4Forms::PictureBox::setHAlign(HAlign hAlign) { _hAlign = hAlign; }

// Set V Allign.
void PS4Forms::PictureBox::setVAlign(VAlign vAlign) { _vAlign = vAlign; }

// Set Fill mode.
void PS4Forms::PictureBox::setFillMode(FillMode fillMode) { _fillMode = fillMode; }

// Draw the image to the screen.
void PS4Forms::PictureBox::Draw(ssg::GraphicsContext *context, ssg::SpriteRenderer *sprite) {
	if (show & visible) {
		// Check first.
		if (_texture == NULL) {
			Logger::Debug("texture is null : %s(%p).\n", _id, this);
			return;
		}
		if ((_texture->getWidth() == 0) || (_texture->getHeight() == 0)) {
			Logger::Debug("texture size is zero.\n");
			return;
		}
		if ((_size.getX() == 0) || (_size.getY() == 0)) {
			Logger::Debug("Either x and/or y coordinate are zero.\n");
			return;
		}

		// Now draw.
		Position windowPosition = _position;
		Size windowSize = _size;
		Position windowCenterPoition = windowPosition + 0.5 * windowSize;
		Size rtSize(context->getCurrentRenderTarget()->getWidth(), context->getCurrentRenderTarget()->getHeight());
		Size texSize(_texture->getWidth(), _texture->getHeight());

		float aspectRatio = rtSize.getX() / rtSize.getY();
		float aspectRatioInv = rtSize.getY() / rtSize.getX();
		Size drawSize;

		switch (_fillMode) {
			case kFillModeDotByDot:
				drawSize = divPerElem(texSize, rtSize);
				break;
			case kFillModeExpand:
				drawSize = windowSize;
				break;
			case kFillModeVExpand:
				drawSize = Size(aspectRatioInv * windowSize.getY() * texSize.getX() / texSize.getY(), windowSize.getY());
				break;
			case kFillModeHExpand:
				drawSize = Size(windowSize.getX(), aspectRatio * windowSize.getX() * texSize.getY() / texSize.getX());
				break;
			default:
				SCE_SAMPLE_UTIL_ASSERT(0);
		}

		float drawX = 0.0f;
		switch (_hAlign) {
			case kHAlignLeft:
				drawX = windowPosition.getX();
				break;
			case kHAlignRight:
				drawX = windowPosition.getX() + windowSize.getX() - drawSize.getX();
				break;
			case kHAlignCenter:
				drawX = windowCenterPoition.getX() - 0.5 * drawSize.getX();
				break;
			default:
				SCE_SAMPLE_UTIL_ASSERT(0);
		}

		float drawY = 0.0f;
		switch (_vAlign) {
			case kVAlignTop:
				drawY = windowPosition.getY();
				break;
			case kVAlignBottom:
				drawY = windowPosition.getY() + windowSize.getY() - drawSize.getY();
				break;
			case kVAlignCenter:
				drawY = windowCenterPoition.getY() - 0.5 * drawSize.getY();
				break;
			default:
				SCE_SAMPLE_UTIL_ASSERT(0);
		}

		// Draw Texture to screen.
		sprite->drawTexture(context, Position(drawX, drawY), drawSize, _texture);
	}
}
#pragma endregion PictureBox

/* Ready to use */
#pragma region TTY
float                   PS4Forms::TTY::tty_text_scale = 0.03f;
float                   PS4Forms::TTY::config_iteselected_scale = 0.035f;			  // Scale of selected menu item
float                   PS4Forms::TTY::config_iteunselected_scale = 0.025f;	      // Scale of unselected menu item
int                     PS4Forms::TTY::config_itex_min_character_offset = 14;	  // Minimum character spacing
float                   PS4Forms::TTY::config_itex_offset = 0.35f * config_iteunselected_scale;                  // Spacing in between columns
float                   PS4Forms::TTY::config_itey_offset = 0.035f;				  // Spacing in between rows			
int                     PS4Forms::TTY::tty_max_charachters_per_line = 100;		  // Maximum characters per line
int                     PS4Forms::TTY::tty_max_lines = 1024;						  // Maximum lines stored in the TTY
float                   PS4Forms::TTY::tty_scroll_bar_size = 0.025f;				  // Maximum lines stored in the TTY
float                   PS4Forms::TTY::screen_proportion = 0.3f;				      // Proportion of the screen TTY takes up, from 0.0f to 1.0f
uint32_t                PS4Forms::TTY::tty_text_color = 0xFFFFFFFF;				      // The TTY text colour (RGBA) [White]
uint32_t                PS4Forms::TTY::tty_text_color_selection = 0x00000000;		  // The TTY text colour (RGBA) [Black]
uint32_t                PS4Forms::TTY::tty_text_color_warning = 0xFFFF11FF;		      // The TTY text colour for a warning (RGBA)
uint32_t                PS4Forms::TTY::tty_text_color_error = 0xFF0000FF;			  // The TTY text colour for an error (RGBA)
PS4Forms::TTY::TtyItem  PS4Forms::TTY::buffer[1000];
uint32_t                PS4Forms::TTY::writePos = 0;
bool                    PS4Forms::TTY::_isFocused = false;
bool                    PS4Forms::TTY::_useLineHighlighting = false;
uint32_t                PS4Forms::TTY::selectedLineIndex = 0;
int32_t                 PS4Forms::TTY::maxLinesDrawNum = ((1.0f - 0.04f) / tty_text_scale);
int32_t                 PS4Forms::TTY::HeadDrawPos = -maxLinesDrawNum;
uint32_t                PS4Forms::TTY::totalLinesWritten = 0;
uint32_t                PS4Forms::TTY::black = tty_text_color_selection;
Color                   PS4Forms::TTY::selection = Vector4((black >> 24 & 0xff) / (float)0xff, (black >> 16 & 0xff) / (float)0xff, (black >> 8 & 0xff) / (float)0xff, (black & 0xff) / (float)0xff);
Size                    PS4Forms::TTY::selectionSize = Size(0.2f, 0.03f);
float                   PS4Forms::TTY::xcoordinate = 0.0f;
float                   PS4Forms::TTY::ycoordinate = 0.0f;
float                   PS4Forms::TTY::fontSize = tty_text_scale;

PS4Forms::TTY::TTY() {}
PS4Forms::TTY::~TTY() {}

void PS4Forms::TTY::onScreenPrintf(const char *text, ...) {
	int len = 0;
	int absoluteSize = 0;
	int counted = 0;
	va_list argptr;
	int startIndex = 0;
	bool newlinefound = false;
	bool dataLeft = true;
	int bufferLen = strlen(buffer[writePos].buffer);

	// Make a temp buffer big enough for a reasonable amount of input
	// 10 lines should be more than enough, anything over will be dropped.
	char tempBuffer[1000] = { '\0' };
	va_start(argptr, text);
	vsnprintf(tempBuffer, tty_max_charachters_per_line * 10, text, argptr);
	va_end(argptr);

	// Also print out to Debug Log.
	//Logger::Debug("buffer: %s\n", tempBuffer);

	// Get the length of the overloaded text.
	len = strlen(tempBuffer);

	// Store it as absoluteSize.
	absoluteSize = len;

	// Handle char by char colorizing.
	struct moreColor charByChar;
	charByChar.color = tty_text_color;

	// Check if there are new lines and adjust lines to setup.
	for (int i = 0; i < len; i++) {
		if (tempBuffer[i] == '\n' || tempBuffer[i] == '\r' || (i - startIndex + bufferLen) >= tty_max_charachters_per_line - 1) {
			if (buffer[writePos].canOverwrite) {
				buffer[writePos].buffer[0] = '\0';
				buffer[writePos].canOverwrite = false;
			}

			// Did we found a rewind ?
			if (tempBuffer[i] == '\r' ? true : false) {
				deleteLastLine();
				memset(buffer[writePos].buffer, 0, 100);
				buffer[writePos].colorize.clear();
			}

			// Copy what we got into the screen buffer.
			strncat(buffer[writePos].buffer, tempBuffer + startIndex + (startIndex ? 1 : 0), i - startIndex);

			// Did we found a new line ?
			newlinefound = (tempBuffer[i] == '\n' ? true : false);

			// Is there data left ?
			if (len - (i + 1) == 0) dataLeft = false;

			// Save amount of formated chars.
			counted += i;

			// Resolve correct amount of chars to colorize.
			if (!startIndex) charByChar.num = i + 1 - startIndex;
			else charByChar.num = i + 1 - startIndex + 1;

			// If there was a new line found, we need to decrement one more character.
			if (newlinefound) charByChar.num -= 1;		

			// Write the color to use and the amount of chars to colorize into the array.
			buffer[writePos].colorize.push_back(charByChar);

			// New buffer line.
			writePos++;
			HeadDrawPos++;
			totalLinesWritten++;

			// Line full and still data left.
			if ((i - startIndex + bufferLen) >= tty_max_charachters_per_line - 1) {
				// Allocate a substring array.
				char *subString = (char *)malloc(len - i);

				// Copy left text into substring.
				memcpy(subString, tempBuffer + i, len - i);

				// Clear temporary Buffer.
				memset(tempBuffer, '\0', tty_max_charachters_per_line * 10);

				// Restore left text.
				memcpy(tempBuffer, subString, len - i);

				// Get the length of the text left.
				len = strlen(tempBuffer);

				// Get the amount of filled chars. (right now will be 0 since a new buffer line was added.)
				bufferLen = strlen(buffer[writePos].buffer);

				// Reset Start Index to begin of array.
				startIndex = 0;

				// Adjust loop counter to point to the first entry of the temporary buffer with the next loop start.
				i = -1;

				// Free the used memory.
				free(subString);
			} else startIndex = i;   // Set start index to the actual position within the array.
		}
	}

	// Do we still have some data left to write into the screen buffer ?
	if (dataLeft) {
		// Calculate left characters.
		int _len = absoluteSize - (counted + 1);

		// Store the last left text.
		strncat(buffer[writePos].buffer, tempBuffer, _len);

		// Save the amount of the text to colorize and push into array.
		charByChar.num = _len;
		buffer[writePos].colorize.push_back(charByChar);
	}

	if (writePos >= tty_max_lines) {
		writePos = 0;
		for (int i = 0; i < tty_max_lines; i++) buffer[i].canOverwrite = true;
	}

	if (HeadDrawPos >= tty_max_lines) HeadDrawPos = 0;
}

void PS4Forms::TTY::onScreenPrintf(const uint32_t &colour, const char *text, ...) {
	int len = 0;
	int absoluteSize = 0;
	int counted = 0;
	va_list argptr;
	int startIndex = 0;
	bool newlinefound = false;
	bool dataLeft = true;
	int bufferLen = strlen(buffer[writePos].buffer);

	// Make a temp buffer big enough for a reasonable amount of input
	// 10 lines should be more than enough, anything over will be dropped.
	char tempBuffer[1000] = { '\0' };
	va_start(argptr, text);
	vsnprintf(tempBuffer, tty_max_charachters_per_line * 10, text, argptr);
	va_end(argptr);

	// Also print out to Debug Log.
	//Logger::Debug("buffer: %s\n", tempBuffer);

	// Get the length of the overloaded text.
	len = strlen(tempBuffer);

	// Store it as absoluteSize.
	absoluteSize = len;

	// Handle char by char colorizing.
	struct moreColor charByChar;
	charByChar.color = colour;

	// Check if there are new lines and adjust lines to setup.
	for (int i = 0; i < len; i++) {
		if (tempBuffer[i] == '\n' || tempBuffer[i] == '\r' || (i - startIndex + bufferLen) >= tty_max_charachters_per_line - 1) {
			if (buffer[writePos].canOverwrite) {
				buffer[writePos].buffer[0] = '\0';
				buffer[writePos].canOverwrite = false;
			}

			// Did we found a rewind ?
			if (tempBuffer[i] == '\r' ? true : false) {
				deleteLastLine();
				memset(buffer[writePos].buffer, 0, 100);
				buffer[writePos].colorize.clear();
			}

			// Copy what we got into the screen buffer.
			strncat(buffer[writePos].buffer, tempBuffer + startIndex + (startIndex ? 1 : 0), i - startIndex);

			// Did we found a new line ?
			newlinefound = (tempBuffer[i] == '\n' ? true : false);

			// Is there data left ?
			if (len - (i + 1) == 0) dataLeft = false;

			// Save amount of formated chars.
			counted += i;

			// Resolve correct amount of chars to colorize.
			if (!startIndex) charByChar.num = i + 1 - startIndex;
			else charByChar.num = i + 1 - startIndex + 1;

			// If there was a new line found, we need to decrement one more character.
			if (newlinefound) charByChar.num -= 1;

			// Write the color to use and the amount of chars to colorize into the array.
			buffer[writePos].colorize.push_back(charByChar);

			// New buffer line.
			writePos++;
			HeadDrawPos++;
			totalLinesWritten++;

			// Line full and still data left.
			if ((i - startIndex + bufferLen) >= tty_max_charachters_per_line - 1) {
				// Allocate a substring array.
				char *subString = (char *)malloc(len - i);

				// Copy left text into substring.
				memcpy(subString, tempBuffer + i, len - i);

				// Clear temporary Buffer.
				memset(tempBuffer, '\0', tty_max_charachters_per_line * 10);

				// Restore left text.
				memcpy(tempBuffer, subString, len - i);

				// Get the length of the text left.
				len = strlen(tempBuffer);

				// Get the amount of filled chars. (right now will be 0 since a new buffer line was added.)
				bufferLen = strlen(buffer[writePos].buffer);

				// Reset Start Index to begin of array.
				startIndex = 0;

				// Adjust loop counter to point to the first entry of the temporary buffer with the next loop start.
				i = -1;

				// Free the used memory.
				free(subString);
			}
			else startIndex = i;   // Set start index to the actual position within the array.
		}
	}

	// Do we still have some data left to write into the screen buffer ?
	if (dataLeft) {
		// Calculate left characters.
		int _len = absoluteSize - counted;

		// Store the last left text.
		strncat(buffer[writePos].buffer, tempBuffer, _len);

		// Save the amount of the text to colorize and push into array.
		charByChar.num = _len;
		buffer[writePos].colorize.push_back(charByChar);
	}

	if (writePos >= tty_max_lines) {
		writePos = 0;
		for (int i = 0; i < tty_max_lines; i++) buffer[i].canOverwrite = true;
	}

	if (HeadDrawPos >= tty_max_lines) HeadDrawPos = 0;
}

void PS4Forms::TTY::update(sce::SampleUtil::Input::PadContext *controller) {
	static int num = 0;
	if (num++ % 5) return;            // Quick and dirty: Only sample the controller one in five times to slow things down.

	if (controller->isButtonDown(sce::SampleUtil::Input::kButtonUp, sce::SampleUtil::Input::kButtonEventPatternAny)) {
		if (selectedLineIndex > 0) selectedLineIndex--;
		if (_useLineHighlighting) {
			if ((selectedLineIndex == HeadDrawPos) && (HeadDrawPos > 0)) HeadDrawPos--;
		} else if (HeadDrawPos > 0) HeadDrawPos--;
	} else if (controller->isButtonDown(sce::SampleUtil::Input::kButtonDown, sce::SampleUtil::Input::kButtonEventPatternAny)) {
		if (selectedLineIndex < totalLinesWritten) selectedLineIndex++;
		if (_useLineHighlighting) {
			if (selectedLineIndex == HeadDrawPos) {
				if (HeadDrawPos < ((totalLinesWritten < tty_max_lines) ? totalLinesWritten : tty_max_lines) - maxLinesDrawNum) {
					HeadDrawPos++;
				}
			}
		} else if (HeadDrawPos < ((totalLinesWritten < tty_max_lines) ? totalLinesWritten : tty_max_lines) - maxLinesDrawNum) {
			HeadDrawPos++;
		}
	}

	// Just to be sure, if we reache a larg number, we clear the variable.
	if (num > 2137483647) num = 0;
}
// **Since we are within the routine of 'digi cross up pressed' we can determine with storing the last sample rate and counting one rate up,
//     with additional comparing against the actual sample rate, if up is still pressed and was not released.

void PS4Forms::TTY::render(sce::SampleUtil::Graphics::SpriteRenderer *renderer, sce::SampleUtil::Graphics::GraphicsContext *context) {
	// Draw background coloured box
	/*renderer->fillRect(context,
	Vector2(0.0f, 1.0f - TTY_SCREEN_PROPORTION),
	Vector2(1.0f, TTY_SCREEN_PROPORTION),
	Vector4(((isFocused ? TTY_BACKGROUND_COLOUR_FOCUSED : TTY_BACKGROUND_COLOUR_UNFOCUSED) >> 24 & 0xff) / (float)0xff,
	((isFocused ? TTY_BACKGROUND_COLOUR_FOCUSED : TTY_BACKGROUND_COLOUR_UNFOCUSED) >> 16 & 0xff) / (float)0xff,
	((isFocused ? TTY_BACKGROUND_COLOUR_FOCUSED : TTY_BACKGROUND_COLOUR_UNFOCUSED) >> 8 & 0xff) / (float)0xff,
	((isFocused ? TTY_BACKGROUND_COLOUR_FOCUSED : TTY_BACKGROUND_COLOUR_UNFOCUSED) & 0xff) / (float)0xff));*/

	// Draw scrollbar to show that you can scroll
	/*if (_isFocused) {
	renderer->fillRect(context, Vector2(0.95f, 1.0f - TTY_SCREEN_PROPORTION), Vector2(0.001f, TTY_SCREEN_PROPORTION), Vector4(1.0f));
	renderer->fillRect(context, Vector2(0.95f + tty_scroll_bar_size, 1.0f - TTY_SCREEN_PROPORTION), Vector2(0.001f, TTY_SCREEN_PROPORTION), Vector4(1.0f));
	renderer->fillRect(context, Vector2(0.95f, 1.0f - TTY_SCREEN_PROPORTION + (((float)HeadDrawPos / (float)(totalLinesWritten - maxLinesDrawNum)) * TTY_SCREEN_PROPORTION) * (1.0f - tty_scroll_bar_size*2.0f)),
	Vector2(tty_scroll_bar_size, tty_scroll_bar_size), Vector4(1.0f));
	}*/

	// Draw the text
	int i = (HeadDrawPos < 0 ? 0 : HeadDrawPos);
	int j = 0;
	int indexer;
	char buff[101];

	for (; i < tty_max_lines && j < maxLinesDrawNum; i++, j++) {
		if (_useLineHighlighting && (i == selectedLineIndex)) {
			renderer->fillRect(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), selectionSize, WHITE);
			renderer->drawDebugStringf(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), tty_text_scale, BLACK, buffer[i].buffer);
		} else {
			// Store position within line.
			indexer = 0;

			// Loop over all pushed char by char colorizing definations.
			for (std::vector<moreColor>::iterator it = buffer[i].colorize.begin(); it != buffer[i].colorize.end(); it++) {
				// Clear buffer before filling up.
				memset(buff, 0, 101);

				// Copy the specific amount on characters to colorize into a new buffer.
				memcpy(buff, buffer[i].buffer + indexer, (*it).num);				

				// Draw the string now.
				renderer->drawDebugStringf(context, Position(xcoordinate + (indexer / (float)(CHAR_COL + 35)), (ycoordinate + (j * tty_text_scale))), tty_text_scale,
					Vector4(((*it).color >> 24 & 0xff) / (float)0xff, ((*it).color >> 16 & 0xff) / (float)0xff, ((*it).color >> 8 & 0xff) / (float)0xff, ((*it).color & 0xff) / (float)0xff),
					buff);

				// Save position.
				indexer += (*it).num;
			}
		}
	}

	for (i = 0; i < HeadDrawPos && j < maxLinesDrawNum; i++, j++) {
		if (_useLineHighlighting && (i == selectedLineIndex)) {
			renderer->fillRect(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), selectionSize, WHITE);
			renderer->drawDebugStringf(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), tty_text_scale, BLACK, buffer[i].buffer);
		} else {
			// Store position within line.
			indexer = 0;

			// Loop over all pushed char by char colorizing definations.
			for (std::vector<moreColor>::iterator it = buffer[i].colorize.begin(); it != buffer[i].colorize.end(); it++) {
				// Clear buffer before filling up.
				memset(buff, 0, 101);

				// Copy the specific amount on characters to colorize into a new buffer.
				memcpy(buff, buffer[i].buffer + indexer, (*it).num);

				// Draw the string now.
				renderer->drawDebugStringf(context, Position(xcoordinate + (indexer / (float)(CHAR_COL + 35)), (ycoordinate + (j * tty_text_scale))), tty_text_scale,
					Vector4(((*it).color >> 24 & 0xff) / (float)0xff, ((*it).color >> 16 & 0xff) / (float)0xff, ((*it).color >> 8 & 0xff) / (float)0xff, ((*it).color & 0xff) / (float)0xff),
					buff);

				// Save position.
				indexer += (*it).num;
			}
		}
	}
}

char *PS4Forms::TTY::getSelectedLine(void) { return strdup(buffer[selectedLineIndex].buffer); }

unsigned int PS4Forms::TTY::getSelectedLineIndex(void) { return selectedLineIndex; }

void PS4Forms::TTY::setSelectedLine(char *line) {
	if (_useLineHighlighting) {
		for (int i = 0; i < totalLinesWritten; i++) {
			if (!strcmp(buffer[i].buffer, line)) selectedLineIndex = i;
		}
	}
}

void PS4Forms::TTY::setSelectedLineIndex(unsigned int line) {
	if (_useLineHighlighting) {
		if (line <= totalLinesWritten) selectedLineIndex = line;
	}
}

void PS4Forms::TTY::useLineHighlighting(bool condition) { _useLineHighlighting = condition; }

void PS4Forms::TTY::deleteLastLine(void) {
	writePos--;
	HeadDrawPos--;
	totalLinesWritten--;
}

int PS4Forms::TTY::getLinesCount(void) { return maxLinesDrawNum; }

void PS4Forms::TTY::reloadMaxLinesDrawNum(void) { maxLinesDrawNum = ((1.0f - ycoordinate) / fontSize); }

float PS4Forms::TTY::getFontSize(void) { return fontSize; }

void PS4Forms::TTY::setFontSize(float size) {
	fontSize = size;
	maxLinesDrawNum = ((1.0f - size) / tty_text_scale);
}

void PS4Forms::TTY::setX(float value) { xcoordinate = value; }

void PS4Forms::TTY::setY(float value) { ycoordinate = value; }

float PS4Forms::TTY::getX(void) { return xcoordinate; }

float PS4Forms::TTY::getY(void) { return ycoordinate; }

void PS4Forms::TTY::clear() {
	for (int i = 0; i < (tty_max_lines - 1); i++) {
		if (strlen(buffer[i].buffer) > 0) {
			memset(buffer[i].buffer, 0, tty_max_charachters_per_line);
			buffer[i].colorize.clear();
		}
	}
	writePos = totalLinesWritten = selectedLineIndex = 0;
	HeadDrawPos = -maxLinesDrawNum;
}
#pragma endregion TTY

#pragma region TTYD
PS4Forms::TTYD::TTYD() {
	TtyItem buff;
	buffer.push_back(buff);
	writePos = 0;
	_isFocused = false;
	_useLineHighlighting = false;
	scrollWithNewLine = false;
	selectedLineIndex = 0;
	HeadDrawPos = -maxLinesDrawNum;
	totalLinesWritten = 0;
	maxLinesDrawNum = ((1.0f - 0.04f) / tty_text_scale);
	black = 0x00000000;
	selection = Vector4((black >> 24 & 0xff) / (float)0xff, (black >> 16 & 0xff) / (float)0xff, (black >> 8 & 0xff) / (float)0xff, (black & 0xff) / (float)0xff);
	selectionSize = Size(0.2f, 0.03f);
	xcoordinate = 0.0f;
	ycoordinate = 0.0f;
	config_iteselected_scale = 0.035f;								 // Scale of selected menu item
	config_iteunselected_scale = 0.025f;								 // Scale of unselected menu item
	config_itex_min_character_offset = 14;									 // Minimum character spacing
	config_itex_offset = 0.35f * config_iteunselected_scale;   // Spacing in between columns
	config_itey_offset = 0.035f;								 // Spacing in between rows
	tty_text_color = 0xFFFFFFFF;									// The TTY text colour (RGBA) [White]
	tty_text_color_selection = 0x00000000;									// The TTY text colour (RGBA) [Black]
	tty_text_color_warning = 0xFFFF11FF;									// The TTY text colour for a warning (RGBA)
	tty_text_color_error = 0xFF0000FF;									// The TTY text colour for an error (RGBA)
	tty_text_scale = 0.03f;								 // The TTY text scale
	tty_max_charachters_per_line = 100;									 // Maximum characters per line
	tty_max_lines = 1024;									 // Maximum lines stored in the TTY
	tty_scroll_bar_size = 0.025f;								 // Maximum lines stored in the TTY
	screen_proportion = 0.3f;									 // Proportion of the screen TTY takes up, from 0.0f to 1.0f
}

PS4Forms::TTYD::~TTYD() {
	clear();
	while (!buffer.empty()) {
		buffer.pop_back();
	}
}

void PS4Forms::TTYD::onScreenPrintf(const char *text, ...) {
	int len = 0;
	int absoluteSize = 0;
	int counted = 0;
	va_list argptr;
	int startIndex = 0;
	bool newlinefound = false;
	bool dataLeft = true;
	int bufferLen = strlen(buffer[writePos].buffer);

	// Make a temp buffer big enough for a reasonable amount of input
	// 10 lines should be more than enough, anything over will be dropped.
	char tempBuffer[1000] = { '\0' };
	va_start(argptr, text);
	vsnprintf(tempBuffer, tty_max_charachters_per_line * 10, text, argptr);
	va_end(argptr);

	// Also print out to Debug Log.
	//Logger::Debug("buffer: %s\n", tempBuffer);

	// Get the length of the overloaded text.
	len = strlen(tempBuffer);

	// Store it as absoluteSize.
	absoluteSize = len;

	// Handle char by char colorizing.
	struct moreColor charByChar;
	charByChar.color = tty_text_color;		
	
	// Check if there are new lines and adjust lines to setup.
	for (int i = 0; i < len; i++) {
		if (tempBuffer[i] == '\n' || tempBuffer[i] == '\r' || (i - startIndex + bufferLen) >= tty_max_charachters_per_line - 1) {
			if (buffer[writePos].canOverwrite) {
				buffer[writePos].buffer[0] = '\0';
				buffer[writePos].canOverwrite = false;
			}

			// Did we found a rewind ?
			if (tempBuffer[i] == '\r' ? true : false) {
				deleteLastLine();
				memset(buffer[writePos].buffer, 0, 100);
				buffer[writePos].colorize.clear();
			}

			// Copy what we got into the screen buffer.
			strncat(buffer[writePos].buffer, tempBuffer + startIndex + (startIndex ? 1 : 0), i - startIndex);

			// Did we found a new line ?
			newlinefound = (tempBuffer[i] == '\n' ? true : false);

			// Is there data left ?
			if (len - (i + 1) == 0) dataLeft = false;

			// Save amount of formated chars.
			counted += i;

			// Resolve correct amount of chars to colorize.
			if (!startIndex) charByChar.num = i + 1 - startIndex;
			else charByChar.num = i + 1 - startIndex + 1;

			// If there was a new line found, we need to decrement one more character.
			if (newlinefound) charByChar.num -= 1;

			// Write the color to use and the amount of chars to colorize into the array.
			buffer[writePos].colorize.push_back(charByChar);			

			// New buffer line.
			TtyItem buff;
			buffer.push_back(buff);
			writePos++;
			if (scrollWithNewLine) HeadDrawPos++;
			totalLinesWritten++;			

			// Line full and still data left.
			if ((i - startIndex + bufferLen) >= tty_max_charachters_per_line - 1) {
				// Allocate a substring array.
				char *subString = (char *)malloc(len - i);

				// Copy left text into substring.
				memcpy(subString, tempBuffer + i, len - i);

				// Clear temporary Buffer.
				memset(tempBuffer, '\0', tty_max_charachters_per_line * 10);

				// Restore left text.
				memcpy(tempBuffer, subString, len - i);

				// Get the length of the text left.
				len = strlen(tempBuffer);

				// Get the amount of filled chars. (right now will be 0 since a new buffer line was added.)
				bufferLen = strlen(buffer[writePos].buffer);

				// Reset Start Index to begin of array.
				startIndex = 0;

				// Adjust loop counter to point to the first entry of the temporary buffer with the next loop start.
				i = -1;

				// Free the used memory.
				free(subString);
			} else startIndex = i;   // Set start index to the actual position within the array.
		}
	}

	// Do we still have some data left to write into the screen buffer ?
	if (dataLeft) {
		// Calculate left characters.
		int _len = absoluteSize - counted;

		// Store the last left text.
		strncat(buffer[writePos].buffer, tempBuffer, _len);

		// Save the amount of the text to colorize and push into array.
		charByChar.num = _len;
		buffer[writePos].colorize.push_back(charByChar);
	}

	if (writePos >= tty_max_lines) {
		writePos = 0;
		for (int i = 0; i < tty_max_lines; i++) buffer[i].canOverwrite = true;
	}

	if (HeadDrawPos >= tty_max_lines) HeadDrawPos = 0;
}

void PS4Forms::TTYD::onScreenPrintf(const uint32_t &colour, const char *text, ...) {
	int len = 0;
	int absoluteSize = 0;
	int counted = 0;
	va_list argptr;
	int startIndex = 0;
	bool newlinefound = false;
	bool dataLeft = true;
	int bufferLen = strlen(buffer[writePos].buffer);

	// Make a temp buffer big enough for a reasonable amount of input
	// 10 lines should be more than enough, anything over will be dropped.
	char tempBuffer[1000] = { '\0' };
	va_start(argptr, text);
	vsnprintf(tempBuffer, tty_max_charachters_per_line * 10, text, argptr);
	va_end(argptr);

	// Also print out to Debug Log.
	//Logger::Debug("buffer: %s\n", tempBuffer);

	// Get the length of the overloaded text.
	len = strlen(tempBuffer);

	// Store it as absoluteSize.
	absoluteSize = len;

	// Handle char by char colorizing.
	struct moreColor charByChar;
	charByChar.color = colour;

	// Check if there are new lines and adjust lines to setup.
	for (int i = 0; i < len; i++) {
		if (tempBuffer[i] == '\n' || tempBuffer[i] == '\r' || (i - startIndex + bufferLen) >= tty_max_charachters_per_line - 1) {
			if (buffer[writePos].canOverwrite) {
				buffer[writePos].buffer[0] = '\0';
				buffer[writePos].canOverwrite = false;
			}

			// Did we found a rewind ?
			if (tempBuffer[i] == '\r' ? true : false) {
				deleteLastLine();
				memset(buffer[writePos].buffer, 0, 100);
				buffer[writePos].colorize.clear();
			}

			// Copy what we got into the screen buffer.
			strncat(buffer[writePos].buffer, tempBuffer + startIndex + (startIndex ? 1 : 0), i - startIndex);

			// Did we found a new line ?
			newlinefound = (tempBuffer[i] == '\n' ? true : false);

			// Is there data left ?
			if (len - (i + 1) == 0) dataLeft = false;

			// Save amount of formated chars.
			counted += i;

			// Resolve correct amount of chars to colorize.
			if (!startIndex) charByChar.num = i + 1 - startIndex;
			else charByChar.num = i + 1 - startIndex + 1;

			// If there was a new line found, we need to decrement one more character.
			if (newlinefound) charByChar.num -= 1;

			// Write the color to use and the amount of chars to colorize into the array.
			buffer[writePos].colorize.push_back(charByChar);

			// New buffer line.
			writePos++;
			HeadDrawPos++;
			totalLinesWritten++;

			// Line full and still data left.
			if ((i - startIndex + bufferLen) >= tty_max_charachters_per_line - 1) {
				// Allocate a substring array.
				char *subString = (char *)malloc(len - i);

				// Copy left text into substring.
				memcpy(subString, tempBuffer + i, len - i);

				// Clear temporary Buffer.
				memset(tempBuffer, '\0', tty_max_charachters_per_line * 10);

				// Restore left text.
				memcpy(tempBuffer, subString, len - i);

				// Get the length of the text left.
				len = strlen(tempBuffer);

				// Get the amount of filled chars. (right now will be 0 since a new buffer line was added.)
				bufferLen = strlen(buffer[writePos].buffer);

				// Reset Start Index to begin of array.
				startIndex = 0;

				// Adjust loop counter to point to the first entry of the temporary buffer with the next loop start.
				i = -1;

				// Free the used memory.
				free(subString);
			} else startIndex = i;   // Set start index to the actual position within the array.
		}
	}
	
	// Do we still have some data left to write into the screen buffer ?
	if (dataLeft) {
		// Calculate left characters.
		int _len = absoluteSize - counted;

		// Store the last left text.
		strncat(buffer[writePos].buffer, tempBuffer, _len);

		// Save the amount of the text to colorize and push into array.
		charByChar.num = _len;
		buffer[writePos].colorize.push_back(charByChar);
	}

	if (writePos >= tty_text_color) {
		writePos = 0;
		for (int i = 0; i < tty_text_color; i++) buffer[i].canOverwrite = true;
	}

	if (HeadDrawPos >= tty_text_color) HeadDrawPos = 0;
}

void PS4Forms::TTYD::update(sce::SampleUtil::Input::PadContext *controller) {
	static int num = 0;
	if (num++ % 5) return;            // Quick and dirty: Only sample the controller one in five times to slow things down.

	if (controller->isButtonDown(sce::SampleUtil::Input::kButtonUp, sce::SampleUtil::Input::kButtonEventPatternAny)) {
		if (selectedLineIndex > 0) selectedLineIndex--;
		if (scrollWithNewLine) {
			if (_useLineHighlighting) {
				if ((selectedLineIndex == HeadDrawPos) && (HeadDrawPos > 0)) HeadDrawPos--;
			} else if (HeadDrawPos > 0) HeadDrawPos--;
		}
	} else if (controller->isButtonDown(sce::SampleUtil::Input::kButtonDown, sce::SampleUtil::Input::kButtonEventPatternAny)) {
		if (selectedLineIndex < totalLinesWritten) selectedLineIndex++;
		if (scrollWithNewLine) {
			if (_useLineHighlighting) {
				if (selectedLineIndex == HeadDrawPos) {
					if (HeadDrawPos < ((totalLinesWritten < tty_max_lines) ? totalLinesWritten : tty_max_lines) - maxLinesDrawNum) {
						HeadDrawPos++;
					}
				}
			} else if (HeadDrawPos < ((totalLinesWritten < tty_max_lines) ? totalLinesWritten : tty_max_lines) - maxLinesDrawNum) {
				HeadDrawPos++;
			}
		}
	}

	// Just to be sure, if we reache a larg number, we clear the variable.
	if (num > 2137483647) num = 0;
}
// **Since we are within the routine of 'digi cross up pressed' we can determine with storing the last sample rate and counting one rate up,
//     with additional comparing against the actual sample rate, if up is still pressed and was not released.

void PS4Forms::TTYD::render(sce::SampleUtil::Graphics::SpriteRenderer *renderer, sce::SampleUtil::Graphics::GraphicsContext *context) {
	// Draw background coloured box
	/*renderer->fillRect(context,
	Vector2(0.0f, 1.0f - TTY_SCREEN_PROPORTION),
	Vector2(1.0f, TTY_SCREEN_PROPORTION),
	Vector4(((isFocused ? TTY_BACKGROUND_COLOUR_FOCUSED : TTY_BACKGROUND_COLOUR_UNFOCUSED) >> 24 & 0xff) / (float)0xff,
	((isFocused ? TTY_BACKGROUND_COLOUR_FOCUSED : TTY_BACKGROUND_COLOUR_UNFOCUSED) >> 16 & 0xff) / (float)0xff,
	((isFocused ? TTY_BACKGROUND_COLOUR_FOCUSED : TTY_BACKGROUND_COLOUR_UNFOCUSED) >> 8 & 0xff) / (float)0xff,
	((isFocused ? TTY_BACKGROUND_COLOUR_FOCUSED : TTY_BACKGROUND_COLOUR_UNFOCUSED) & 0xff) / (float)0xff));*/

	// Draw scrollbar to show that you can scroll
	/*if (_isFocused) {
	renderer->fillRect(context, Vector2(0.95f, 1.0f - TTY_SCREEN_PROPORTION), Vector2(0.001f, TTY_SCREEN_PROPORTION), Vector4(1.0f));
	renderer->fillRect(context, Vector2(0.95f + tty_scroll_bar_size, 1.0f - TTY_SCREEN_PROPORTION), Vector2(0.001f, TTY_SCREEN_PROPORTION), Vector4(1.0f));
	renderer->fillRect(context, Vector2(0.95f, 1.0f - TTY_SCREEN_PROPORTION + (((float)HeadDrawPos / (float)(totalLinesWritten - maxLinesDrawNum)) * TTY_SCREEN_PROPORTION) * (1.0f - tty_scroll_bar_size*2.0f)),
	Vector2(tty_scroll_bar_size, tty_scroll_bar_size), Vector4(1.0f));
	}*/

	// Draw the text
	int i = (HeadDrawPos < 0 ? 0 : HeadDrawPos);
	int j = 0;
	int indexer;
	char buff[101];

	for (; i < tty_max_lines && j < maxLinesDrawNum; i++, j++) {
		if (_useLineHighlighting && (i == selectedLineIndex)) {			
			renderer->fillRect(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), selectionSize, WHITE);
			renderer->drawDebugStringf(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), tty_text_scale, BLACK, buffer[i].buffer);
		} else {
			// Store position within line.
			indexer = 0;

			// Loop over all pushed char by char colorizing definations.
			for (std::vector<moreColor>::iterator it = buffer[i].colorize.begin(); it != buffer[i].colorize.end(); it++) {
				// Clear buffer before filling up.
				memset(buff, 0, 101);
				
				// Copy the specific amount on characters to colorize into a new buffer.
				memcpy(buff, buffer[i].buffer + indexer, (*it).num);

				// Save position.
				indexer = (*it).num - 1;

				// Draw the string now.
				renderer->drawDebugStringf(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), tty_text_scale,
					Vector4(((*it).color >> 24 & 0xff) / (float)0xff, ((*it).color >> 16 & 0xff) / (float)0xff, ((*it).color >> 8 & 0xff) / (float)0xff, ((*it).color & 0xff) / (float)0xff),
					buff);
			}
		}
	}

	for (i = 0; i < HeadDrawPos && j < maxLinesDrawNum; i++, j++) {
		if (_useLineHighlighting && (i == selectedLineIndex)) {
			renderer->fillRect(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), selectionSize, WHITE);
			renderer->drawDebugStringf(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), tty_text_scale, BLACK, buffer[i].buffer);
		} else {
			// Store position within line.
			indexer = 0;

			// Loop over all pushed char by char colorizing definations.
			for (std::vector<moreColor>::iterator it = buffer[i].colorize.begin(); it != buffer[i].colorize.end(); it++) {
				// Clear buffer before filling up.
				memset(buff, 0, 101);

				// Copy the specific amount on characters to colorize into a new buffer.
				memcpy(buff, buffer[i].buffer + indexer, (*it).num);

				// Save position.
				indexer = (*it).num - 1;

				// Draw the string now.
				renderer->drawDebugStringf(context, Position(xcoordinate, (ycoordinate + (j * tty_text_scale))), tty_text_scale,
					Vector4(((*it).color >> 24 & 0xff) / (float)0xff, ((*it).color >> 16 & 0xff) / (float)0xff, ((*it).color >> 8 & 0xff) / (float)0xff, ((*it).color & 0xff) / (float)0xff),
					buff);
			}
		}
	}
}

char *PS4Forms::TTYD::getSelectedLine(void) { return strdup(buffer[selectedLineIndex].buffer); }

unsigned int PS4Forms::TTYD::getSelectedLineIndex(void) { return selectedLineIndex; }

void PS4Forms::TTYD::setSelectedLine(char *line) {
	if (_useLineHighlighting) {
		for (int i = 0; i < totalLinesWritten; i++) {
			if (!strcmp(buffer[i].buffer, line)) selectedLineIndex = i;
		}
	}
}

void PS4Forms::TTYD::setSelectedLineIndex(unsigned int line) {
	if (_useLineHighlighting) {
		if (line <= totalLinesWritten) selectedLineIndex = line;
	}
}

void PS4Forms::TTYD::useLineHighlighting(bool condition) { _useLineHighlighting = condition; }

void PS4Forms::TTYD::deleteLastLine(void) {
	buffer.pop_back();
	writePos--;
	if (scrollWithNewLine) HeadDrawPos--;
	totalLinesWritten--;	
}

int PS4Forms::TTYD::getLinesCount(void) { return maxLinesDrawNum; }

void PS4Forms::TTYD::reloadMaxLinesDrawNum(void) { maxLinesDrawNum = ((1.0f - ycoordinate) / tty_text_scale); }

void PS4Forms::TTYD::DoScrollWithNewLine(bool state) { scrollWithNewLine = state; }

float PS4Forms::TTYD::getFontSize(void) { return tty_text_scale; }

void PS4Forms::TTYD::setFontSize(float size) { tty_text_scale = size; }

void PS4Forms::TTYD::TextColor(uint32_t color) { tty_text_color = color; }

uint32_t PS4Forms::TTYD::getTextColor(void) { return tty_text_color; }

void PS4Forms::TTYD::setX(float value) { xcoordinate = value; }

void PS4Forms::TTYD::setY(float value) {
	ycoordinate = value;
	maxLinesDrawNum = ((1.0f - value) / tty_text_scale);
}

float PS4Forms::TTYD::getX(void) { return xcoordinate; }

float PS4Forms::TTYD::getY(void) { return ycoordinate; }

void PS4Forms::TTYD::clear() {
	for (int i = 0; i < (tty_text_scale - 1); i++) {
		if (strlen(buffer[i].buffer) > 0) {
			memset(buffer[i].buffer, 0, tty_max_charachters_per_line);
			buffer[i].colorize.clear();
		}
	}
	writePos = totalLinesWritten = selectedLineIndex = 0;
	HeadDrawPos = -maxLinesDrawNum;
}
#pragma endregion TTYD

/* Ready to use */
#pragma region RichTextBox
/*------------------------------------------------------------------
                  RichTextBox Class
-------------------------------------------------------------------*/
// 
PS4Forms::RichTextBox::RichTextBox() {
	ttyd = new TTYD();
	show = focus = isFocused = false;
	visible = true;
	_id = strdup("");
	_position = Position(0.0f, 0.0f);
	_size = Size(0.0f, 0.0f);
	ttyd->setX(_position.getX() + 0.01f);
	ttyd->setY(_position.getY() + 0.01f);
}

// Instance Deinitializer.
PS4Forms::RichTextBox::~RichTextBox() {
	ttyd->~TTYD();
	free(_id);
}

// Set Color of this form.
void PS4Forms::RichTextBox::setBackGroundColor(Color color) { _color = color; }

// Set Position of this form.
void PS4Forms::RichTextBox::setPosition(Position position) {
	_position = position;
	ttyd->setX(position.getX() + 0.01f);
	ttyd->setY(position.getY() + 0.01f);
}

// Get Color of this form.
Color PS4Forms::RichTextBox::getBackGroundColor(void) { return _color; }

// Drawing the Form and all his childrens if any.
void PS4Forms::RichTextBox::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Draw background color.
	sprite->fillRect(graphicsContext, _position, _size, _color);

	// Render TTY now.
	ttyd->render(sprite, graphicsContext);
}

// [Pipe] Take user input and updates things.
void PS4Forms::RichTextBox::Update(sce::SampleUtil::Input::PadContext *controller) { ttyd->update(controller); }

// [Pipe] Adds text to the buffer.
void PS4Forms::RichTextBox::Write(const char *format, ...) {
	char buf[1000];                                           // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, format);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), format, args);                // Format the string.
	ttyd->onScreenPrintf(buf);                                // Push buffer to tty.
	va_end(args);                                             // Close the variable argument list.
}

// [Pipe] Adds text to the buffer.
void PS4Forms::RichTextBox::WriteLine(const char *format, ...) {
	char buf[1000];                                           // Initialize a buffer.
	va_list args;                                             // Initialize a variable list.
	va_start(args, format);                                      // Get the args into a type specific list.
	vsnprintf(buf, sizeof(buf), format, args);                // Format the string.
	ttyd->onScreenPrintf(buf);                                // Push buffer to tty.
	va_end(args);                                             // Close the variable argument list.
	LineBreak();
}

//  [Pipe] Add a Line Break.
void PS4Forms::RichTextBox::LineBreak(void) {
	char buf[3];                                              // Initialize a buffer.
	sprintf(buf, "%s", "\n");                                 // Add the Linebreak.
	ttyd->onScreenPrintf(buf);                                // Push buffer to tty.
}

// [Pipe] Get current selected line.
char *PS4Forms::RichTextBox::getSelectedLine(void) { return ttyd->getSelectedLine(); }

// [Pipe] Get current selected line indexer.
unsigned int PS4Forms::RichTextBox::getSelectedLineIndex(void) { return ttyd->getSelectedLineIndex(); }

// [Pipe] Set current selected line.
void PS4Forms::RichTextBox::setSelectedLine(char *line) { return ttyd->setSelectedLine(line); }

// [Pipe] Set current selected line indexer.
void PS4Forms::RichTextBox::setSelectedLineIndex(unsigned int line) { return ttyd->setSelectedLineIndex(line); }

// [Pipe] Set to use Line Highlighting or not.
void PS4Forms::RichTextBox::useLineHighlighting(bool condition) { return ttyd->useLineHighlighting(condition); }

// [Pipe] Set a EventDispatcher to use for sound effects.
void PS4Forms::RichTextBox::setEvent(common::Service::EventDispatcher *eventDispatcher) { ttyd->setEvent(eventDispatcher); }

// [Pipe] Deletes the last line from the buffer.
void PS4Forms::RichTextBox::deleteLastLine(void) { ttyd->deleteLastLine(); }

// [Pipe] Determine if selected line is a dir.
bool PS4Forms::RichTextBox::isSelectedLineDir(void) { return ttyd->isSelectedLineDir(); }

// [Pipe] 
int PS4Forms::RichTextBox::GetLinesCount(void) { return ttyd->getLinesCount(); }

// [Pipe] Reload the maximum lines to draw after a position and/or a font size change.
void PS4Forms::RichTextBox::reloadMaxLinesDrawNum(void) { ttyd->reloadMaxLinesDrawNum(); }

// [Pipe] Get the Font Size.
float PS4Forms::RichTextBox::getFontSize(void) { return ttyd->getFontSize(); }

// [Pipe] Set the Font Size.
void PS4Forms::RichTextBox::setFontSize(float size) { return ttyd->setFontSize(size); }

// 
void PS4Forms::RichTextBox::TextColor(uint32_t color) { ttyd->TextColor(color); }

// 
uint32_t PS4Forms::RichTextBox::getTextColor(void) { return ttyd->getTextColor(); }

// [Pipe] Set the Y Coordinate.
void PS4Forms::RichTextBox::setY(float position) { return ttyd->setY(position); }

// [Pipe] Set the X Coordinate.
void PS4Forms::RichTextBox::setX(float position) { return ttyd->setX(position); }

// [Pipe] Get the Y Coordinate.
float PS4Forms::RichTextBox::getY(void) { return ttyd->getY(); }

// [Pipe] Set the X Coordinate.
float PS4Forms::RichTextBox::getX(void) { return ttyd->getX(); }

// [Pipe] Clear the TTY.
void PS4Forms::RichTextBox::Clear(void) { return ttyd->clear(); }
#pragma endregion RichTextBox

/* Ready to use */
#pragma region Button
/*------------------------------------------------------------------
                  Button Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::Button::Button() {
	show = focus = isFocused = false;
	visible = true;
	charsize = CHAR_S;
	_id = strdup("");
	_text = strdup("");
	_position =Position(0.0f, 0.0f);
	_size = Size(0.05f, 0.10f);
	_color = BLUE;
	_borderColor = LIGHT_GRAY;
	_textColor = BLACK;
	img = new PictureBox();
}

// Class Initializer.
PS4Forms::Button::Button(const char *id, const char *text, float _charsize, Position position, Size size, Color color, Color textcolor) {
	show = focus = isFocused = false;
	visible = true;
	charsize = _charsize;
	_id = strdup(id);
	_text = strdup(text);
	_position = position;
	_size = size;
	_color = color;
	_textColor = textcolor;
	_borderColor = LIGHT_GRAY;
	img->~PictureBox();
}

// Class Deinitializer.
PS4Forms::Button::~Button() { free(_id); free (_text); }

// Set the TextBox Text to draw.
void PS4Forms::Button::setText(const char *text) {
	if (_text != nullptr) free(_text);
	_text = strdup(text);
}

// Get the TextBox Text to draw.
char *PS4Forms::Button::getText(void) {
	if (_text != nullptr) return strdup(_text);
	else return strdup("");
}

// Set Titel Color of this form.
void PS4Forms::Button::TextColor(Color color) { _textColor = color; }

// Set the Borders Color.
void PS4Forms::Button::BorderColor(Color color) { _borderColor = color; }

// Get the Borders Color.
Color PS4Forms::Button::getBorderColor(void) { return _borderColor; }

// Get Titel Color of this form.
Color PS4Forms::Button::getTextColor(void) { return _textColor; }

// Set Position of this form.
void PS4Forms::Button::setPosition(Position position) {
	_position = position;
	if (img != nullptr) img->setPosition(Position(position.getX() + img->getPosition().getX(), position.getY() + img->getPosition().getY()));
}

// Set Size of this form.
void PS4Forms::Button::setSize(Size size) {
	_size = size;
	if (img != nullptr) {
		if (img->getSize().getX() > size.getX()) img->setSize(Size(size.getX(), img->getSize().getY()));
		if (img->getSize().getY() > size.getY()) img->setSize(Size(img->getSize().getX(), size.getY()));
	}
}

// Drawing the Form and all his childrens if any.
void PS4Forms::Button::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Shall we draw ?
	if (show & visible) {
		// Draw the form.
		if (!focus) _sprite->fillRect(graphicsContext, _position, _size, _color);
		else _sprite->fillRect(graphicsContext, _position, _size, WHITE);

		// Draw Border.
		if (!focus) _sprite->drawRect(graphicsContext, _position, _size, _borderColor);
		else _sprite->drawRect(graphicsContext, _position, _size, BLACK);

		// Draw Text into the button.
		if (strlen(_text) > 0) _sprite->drawDebugStringf(graphicsContext, vecmath::Vector2(_position.getX() + 0.01, _position.getY() + charsize + 0.005), charsize, _textColor, _text);

		// Draw icon.
		if (img != nullptr) img->Draw(graphicsContext, sprite);
	}
}
#pragma endregion Button

/* Ready to use */
#pragma region Scrollbar
/*------------------------------------------------------------------
                  ScrollBar Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::ScrollBar::ScrollBar() {
	tty_scroll_bar_size = 0.025f;								 // Maximum lines stored in the TTY
	screen_proportion = 0.3f;									 // Proportion of the screen TTY takes up, from 0.0f to 1.0f
	show = focus = isFocused = false;
	visible = true;
	_position =Position(0, 0);
	_size0 = vecmath::Vector2(0.001f, screen_proportion);
	_size = Size(tty_scroll_bar_size, tty_scroll_bar_size);
	y = 1.0f - screen_proportion;
	_color = BLUE;
	_borderColor = LIGHT_GRAY;
	head = 0;	
	linesWritten = 0;
	maxLines = 0;
}

// Class Initializer.
PS4Forms::ScrollBar::ScrollBar(Position position, Color color) {
	tty_scroll_bar_size = 0.025f;								 // Maximum lines stored in the TTY
	screen_proportion = 0.3f;									 // Proportion of the screen TTY takes up, from 0.0f to 1.0f
	show = focus = isFocused = false;
	visible = true;
	_position = position;
	_size0 = vecmath::Vector2(0.001f, screen_proportion);
	_size = Size(tty_scroll_bar_size, tty_scroll_bar_size);
	y = 1.0f - screen_proportion;
	_color = color;
	_borderColor = LIGHT_GRAY;
	head = 0;
	linesWritten = 0;
	maxLines = 0;
}

// Get position of Head to draw.
int32_t PS4Forms::ScrollBar::getHead(void) { return head; }

// Set position of head to draw.
void PS4Forms::ScrollBar::Head(int32_t num) { head = num; }

// Get count of lines written.
uint32_t PS4Forms::ScrollBar::getLinesWritten(void) { return linesWritten; }

// Set count of lines written.
void PS4Forms::ScrollBar::LinesWritten(uint32_t num) { linesWritten = num; }

// Set maximum lines. 
int32_t PS4Forms::ScrollBar::getMaxLines(void) { return maxLines; }

// Get maximum lines.
void PS4Forms::ScrollBar::MaxLines(int32_t num) { maxLines = num; }

// Set the Borders Color.
void PS4Forms::ScrollBar::BorderColor(Color color) { _borderColor = color; }

// Get the Borders Color.
Color PS4Forms::ScrollBar::getBorderColor(void) { return _borderColor; }

// Set base x position.
void PS4Forms::ScrollBar::InitX(float _x) { x = _x; }

// Getter and Setter for scrolbar_size and screen_propoertion.

// Drawing the Form and all his childrens if any.
void PS4Forms::ScrollBar::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Draw the form.
	if (show & visible) {
		if (focus) {
			float _y = (y + ((((float)head / (float)(linesWritten - maxLines)) * screen_proportion) * (1.0f - tty_scroll_bar_size * 2.0f)));
			_sprite->fillRect(graphicsContext, vecmath::Vector2(x, y), _size0, _color);
			_sprite->fillRect(graphicsContext, vecmath::Vector2(x + tty_scroll_bar_size, y), _size0, _color);
			_sprite->fillRect(graphicsContext, vecmath::Vector2(x, _y), _size, _color);
		}
	}
}
#pragma endregion Scrollbar

/* Ready to use */
#pragma region CheckBox
/*------------------------------------------------------------------
                  CheckBox Class
------------------------------------------------------------------*/
// Class initializer.
PS4Forms::CheckBox::CheckBox() {
	show = focus = isFocused = false;
	visible = true;
	_id = strdup("");
	_text = strdup("");
	_position = Position(0, 0);
	_size = Size(0.02, 0.02);
	_color = WHITE;
	_textColor = BLACK;
	_borderColor = LIGHT_GRAY;
}

// Class initializer.
PS4Forms::CheckBox::CheckBox(const char *id, const char *text, Position position, Color color, Color textcolor) {
	show = focus = isFocused = false;
	visible = true;
	_id = strdup(id);
	_text = strdup(text);
	_position = position;
	_size = Size(0.02, 0.02);
	_color = color;
	_textColor = textcolor;
	_borderColor = LIGHT_GRAY;
}

// Class deinitializer.
PS4Forms::CheckBox::~CheckBox() { free(_id); free(_text); }

// Set the Label Text to draw.
void PS4Forms::CheckBox::setText(const char *text) {
	if (_text != nullptr) free(_text);
	_text = strdup(text);
}

// Get the label Text to draw.
char *PS4Forms::CheckBox::getText(void) {
	if (_text != nullptr) return strdup(_text);
	else return strdup("");
}

// Set Titel Color of this form.
void PS4Forms::CheckBox::TextColor(Color color) { _textColor = color; }

// Get Titel Color of this form.
Color PS4Forms::CheckBox::getTextColor(void) { return _textColor; }

// Set the Borders Color.
void PS4Forms::CheckBox::BorderColor(Color color) { _borderColor = color; }

// Get the Borders Color.
Color PS4Forms::CheckBox::getBorderColor(void) { return _borderColor; }

// Drawing the Form and all his childrens if any.
void PS4Forms::CheckBox::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Shall we draw ?
	if (show & visible) {
		// Draw the form.
		_sprite->fillRect(graphicsContext, _position, _size, _color);

		// Draw Border.
		if (focus) _sprite->drawRect(graphicsContext, _position, _size, BLACK);
		else _sprite->drawRect(graphicsContext, _position, _size, _borderColor);

		// Shall we draw a checked box ?
		if (checked) _sprite->fillRect(graphicsContext, vecmath::Vector2(_position.getX() + 0.002, _position.getY() + 0.002), vecmath::Vector2(0.018, 0.018), BLACK);
	}
}
#pragma endregion CheckBox

/* Ready to use */
#pragma region Form
/*------------------------------------------------------------------
                         Form Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::Form::Form() {
	show = focus = closed = isFocused = false;
	visible = visibleBoarder = true;
	_id = strdup("");
	_text = strdup("");
	_position = Position(0, 0);
	_size = Size(0, 0);
	_color = DARK_GRAY;
	_textColor = BLACK;
	_borderColor = LIGHT_GRAY;
}

// Class Initializer.
PS4Forms::Form::Form(const char *id, const char *text, Position position, Size size, Color color, Color textcolor) {
	show = focus = closed = isFocused = false;
	visible = visibleBoarder = true;
	_id = strdup(id);
	_text = strdup(text);
	_position = position;
	_size = size;
	_color = color;
	_textColor = textcolor;
	_borderColor = LIGHT_GRAY;
}

// Class Deinitializer.
PS4Forms::Form::~Form() {
	// Between the closing we tell the blocking function to return. So it will have some time before the whole object get killed.
	closed = true;

	// Check vector if form hase childrens and close them first.
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			(*it)->Close();
		}
	}
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			(*it)->Close();
		}
	}
	if (labels.size() > 0) {
		for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
			(*it)->Close();
		}
	}
	if (button.size() > 0) {
		for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
			(*it)->Close();
		}
	}
	if (sbar.size() > 0) {
		for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
			(*it)->Close();
		}
	}
	if (checkB.size() > 0) {
		for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
			(*it)->Close();
		}
	}
	if (textb.size() > 0) {
		for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
			(*it)->Close();
		}
	}
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			(*it)->Close();
		}
	}
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			(*it)->Close();
		}
	}

	free(_id);
	free(_text);
}

// Set the Label Text to draw.
void PS4Forms::Form::setText(const char *text) {
	if (_text != nullptr) free(_text);
	_text = strdup(text);
}

// Get the label Text to draw.
char *PS4Forms::Form::getText(void) {
	if (_text != nullptr) return strdup(_text);
	else return strdup("");
}

// Set Titel Color of this form.
void PS4Forms::Form::TextColor(Color color) { _textColor = color; }

// Get Titel Color of this form.
Color PS4Forms::Form::getTextColor(void) { return _textColor; }

// Set the Borders Visability state.
void PS4Forms::Form::BorderVisible(bool state) { visibleBoarder = state; }

// Set Position of this form.
void PS4Forms::Form::setPosition(Position position) {
	_position = position;

	// Add the Parent Postion to all childs.
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (labels.size() > 0) {
		for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (button.size() > 0) {
		for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (sbar.size() > 0) {
		for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (checkB.size() > 0) {
		for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (textb.size() > 0) {
		for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
}

// Set the Parent Position of this form.
void PS4Forms::Form::setParentPos(Position position) {
	_position = Position((position.getX() + _position.getX()), (position.getY() + _position.getY()));
	// Add the Parent Postion to all childs.
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (labels.size() > 0) {
		for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (button.size() > 0) {
		for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (sbar.size() > 0) {
		for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (checkB.size() > 0) {
		for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (textb.size() > 0) {
		for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			(*it)->setParentPos(position);
		}
	}
}

// Set the Borders Color.
void PS4Forms::Form::BorderColor(Color color) { _borderColor = color; }

// Get the Borders Color.
Color PS4Forms::Form::getBorderColor(void) { return _borderColor; }

// Set to use Background Color.
void PS4Forms::Form::UseBgColor(bool state) { useBgColor = state; }

// Set Color of this form.
void PS4Forms::Form::setColor(Color color) { _color = color; }

// Show Form.
void PS4Forms::Form::Show(void) {
	show = true;

	// Set all Childs to Show.
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			(*it)->Show();
		}
	}
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			(*it)->Show();
		}
	}
	if (labels.size() > 0) {
		for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
			(*it)->Show();
		}
	}
	if (button.size() > 0) {
		for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
			(*it)->Show();
		}
	}
	if (sbar.size() > 0) {
		for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
			(*it)->Show();
		}
	}
	if (checkB.size() > 0) {
		for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
			(*it)->Show();
		}
	}
	if (textb.size() > 0) {
		for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
			(*it)->Show();
		}
	}
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			(*it)->Show();
		}
	}
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			(*it)->Show();
		}
	}
}

// Blocks the code until the Form was closed, then returns.
void PS4Forms::Form::ShowForm(void) {
	show = true;

	// Set all Childs to Show.
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			(*it)->Show();
		}
	}
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			(*it)->Show();
		}
	}
	if (labels.size() > 0) {
		for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
			(*it)->Show();
		}
	}
	if (button.size() > 0) {
		for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
			(*it)->Show();
		}
	}
	if (sbar.size() > 0) {
		for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
			(*it)->Show();
		}
	}
	if (checkB.size() > 0) {
		for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
			(*it)->Show();
		}
	}
	if (textb.size() > 0) {
		for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
			(*it)->Show();
		}
	}
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			(*it)->Show();
		}
	}
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			(*it)->Show();
		}
	}

	while (!closed) {}
}

// Hide Form.
void PS4Forms::Form::Hide(void) {
	show = false;

	// Set all Childs to Hide.
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			(*it)->Hide();
		}
	}

	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			(*it)->Hide();
		}
	}
	if (labels.size() > 0) {
		for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
			(*it)->Hide();
		}
	}
	if (button.size() > 0) {
		for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
			(*it)->Hide();
		}
	}
	if (sbar.size() > 0) {
		for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
			(*it)->Hide();
		}
	}
	if (checkB.size() > 0) {
		for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
			(*it)->Hide();
		}
	}
	if (textb.size() > 0) {
		for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
			(*it)->Hide();
		}
	}
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			(*it)->Hide();
		}
	}
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			(*it)->Hide();
		}
	}
}

// Close Form.
void PS4Forms::Form::Close(void) { this->~Form(); }

// Set the SpriteRenderer.
void PS4Forms::Form::SetSprite(ssg::SpriteRenderer *sprite) {
	_sprite = sprite;

	// Set spriteRenderer for all child objects, if any.
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			(*it)->SetSprite(sprite);
		}
	}
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			(*it)->SetSprite(sprite);
		}
	}
	if (labels.size() > 0) {
		for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
			(*it)->SetSprite(sprite);
		}
	}
	if (button.size() > 0) {
		for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
			(*it)->SetSprite(sprite);
		}
	}
	if (sbar.size() > 0) {
		for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
			(*it)->SetSprite(sprite);
		}
	}
	if (checkB.size() > 0) {
		for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
			(*it)->SetSprite(sprite);
		}
	}
	if (textb.size() > 0) {
		for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
			(*it)->SetSprite(sprite);
		}
	}
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			(*it)->SetSprite(sprite);
		}
	}
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			(*it)->SetSprite(sprite);
		}
	}
}

// Compares the updated cursor postion against the position and size of the form and will set 'isCursorRange' to true, if the stick, cursor is within our form.
bool PS4Forms::Form::UpdateCursor(float x, float y) {
	if (IsObjectFocus(x, y, _position, _size)) focus = true;
	else focus = false;

	// Update the Cursor position for all Childs too.
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			(*it)->UpdateCursor(x, y);
		}
	}
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			(*it)->UpdateCursor(x, y);
		}
	}
	if (labels.size() > 0) {
		for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
			(*it)->UpdateCursor(x, y);
		}
	}
	if (button.size() > 0) {
		for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
			(*it)->UpdateCursor(x, y);
		}
	}
	if (sbar.size() > 0) {
		for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
			(*it)->UpdateCursor(x, y);
		}
	}
	if (checkB.size() > 0) {
		for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
			(*it)->UpdateCursor(x, y);
		}
	}
	if (textb.size() > 0) {
		for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
			(*it)->UpdateCursor(x, y);
		}
	}
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			(*it)->UpdateCursor(x, y);
		}
	}
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			(*it)->UpdateCursor(x, y);
		}
	}

	return focus;
}

// Sets label visabillity.
void PS4Forms::Form::Visible(bool Visible) {
	visible = Visible;

	if (visible) {
		// Set all Childs to Show.
		if (forms.size() > 0) {
			for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
				(*it)->Show();
			}
		}
		if (widgets.size() > 0) {
			for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
				(*it)->Show();
			}
		}
		if (labels.size() > 0) {
			for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
				(*it)->Show();
			}
		}
		if (button.size() > 0) {
			for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
				(*it)->Show();
			}
		}
		if (sbar.size() > 0) {
			for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
				(*it)->Show();
			}
		}
		if (checkB.size() > 0) {
			for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
				(*it)->Show();
			}
		}
		if (textb.size() > 0) {
			for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
				(*it)->Show();
			}
		}
		if (pictures.size() > 0) {
			for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
				(*it)->Show();
			}
		}
		if (rtbs.size() > 0) {
			for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
				(*it)->Show();
			}
		}
	} else {
		// Set all Childs to Hide.
		if (forms.size() > 0) {
			for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
				(*it)->Hide();
			}
		}
		if (widgets.size() > 0) {
			for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
				(*it)->Hide();
			}
		}
		if (labels.size() > 0) {
			for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
				(*it)->Hide();
			}
		}
		if (button.size() > 0) {
			for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
				(*it)->Hide();
			}
		}
		if (sbar.size() > 0) {
			for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
				(*it)->Hide();
			}
		}
		if (checkB.size() > 0) {
			for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
				(*it)->Hide();
			}
		}
		if (textb.size() > 0) {
			for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
				(*it)->Hide();
			}
		}
		if (pictures.size() > 0) {
			for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
				(*it)->Hide();
			}
		}
		if (rtbs.size() > 0) {
			for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
				(*it)->Hide();
			}
		}
	}
}

// Update Stuff. Check if user closed form.
void PS4Forms::Form::Update(sce::SampleUtil::Input::PadContext *controller) {
	// Is Form showen ?
	if (show & visible) {
		if (controller->isButtonPressed(ssi::kButtonCircle, ssi::kButtonEventPatternAny)) {
			show = false;
		}

		// Add update for childs.
	}
}

// Drawing the Form and all his childrens if any.
void PS4Forms::Form::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Draw the form.
	if (show & visible) {
		if (useBgColor) sprite->fillRect(graphicsContext, _position, _size, _color);

		// Shall we draw a border ?
		if (visibleBoarder) {
			// Draw a border.
			if (!focus) {
				sprite->drawRect(graphicsContext, _position, _size, WHITE);
				sprite->fillRect(graphicsContext, vecmath::Vector2(_position.getX(), _position.getY() - 0.04), vecmath::Vector2(_size.getX(), 0.04), WHITE);
			} else {
				sprite->drawRect(graphicsContext, _position, _size, _borderColor);
				sprite->fillRect(graphicsContext, vecmath::Vector2(_position.getX(), _position.getY() - 0.04), vecmath::Vector2(_size.getX(), 0.04), _borderColor);
			}
			// Draw Title into Border.
			sprite->drawDebugStringf(graphicsContext, vecmath::Vector2(_position.getX() + 0.01, _position.getY() - 0.035), 0.03, LIGHT_BLUE, _text);
		}

		// Draw all child objects, if any.
		if (drawFuncs.size() > 0) {
			for (std::vector<void (*)()>::iterator it = drawFuncs.begin(); it != drawFuncs.end(); it++) {
				(*it)();
			}
		}
		if (forms.size() > 0) {
			for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
		if (widgets.size() > 0) {
			for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
		if (labels.size() > 0) {
			for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
		if (button.size() > 0) {
			for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
		if (sbar.size() > 0) {
			for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
		if (checkB.size() > 0) {
			for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
		if (textb.size() > 0) {
			for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
		if (pictures.size() > 0) {
			for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
		if (rtbs.size() > 0) {
			for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
				(*it)->Draw(graphicsContext, sprite);
			}
		}
	}
}

// Add a external Draw function to the Applications drawing loop.
void PS4Forms::Form::Add(void (*drawEvent)()) { drawFuncs.push_back(drawEvent); }

// Add a Form Object to this Form.
void PS4Forms::Form::Add(Form *child) {
	if (child != nullptr) {
		child->parent = this;
		forms.push_back(child);
	}
}

// Add a BaseWidget Object to this Form.
void PS4Forms::Form::Add(WidgetBase *child) {
	if (child != nullptr) {
		child->setParent((WidgetBase *)this);
		widgets.push_back(child);
	}
}

// Add a Label Object to this Form.
void PS4Forms::Form::Add(Label *child) {
	if (child != nullptr) {
		child->setParentPos(_position);
		labels.push_back(child);
	}
}

// Add a Button Object to this Form.
void PS4Forms::Form::Add(Button *child) {
	if (child != nullptr) {
		child->setParentPos(_position);
		button.push_back(child);
	}
}

// Add a ScrollBar Object to this Form.
void PS4Forms::Form::Add(ScrollBar *child) {
	if (child != nullptr) {
		child->setParentPos(_position);
		sbar.push_back(child);
	}
}

// Add a CheckBox Object to this Form.
void PS4Forms::Form::Add(CheckBox *child) {
	if (child != nullptr) {
		child->setParentPos(_position);
		checkB.push_back(child);
	}
}

// Add a TextBox Object to this Form.
void PS4Forms::Form::Add(TextBox *child) {
	if (child != nullptr) {
		child->setParentPos(_position);
		textb.push_back(child);
	}
}

// Add a PictureBox Object to this Form.
void PS4Forms::Form::Add(PictureBox *child) {
	if (child != nullptr) {
		child->setParentPos(_position);
		pictures.push_back(child);
	}
}

// Add a RichTextBox Object to this Form.
void PS4Forms::Form::Add(RichTextBox *child) {
	if (child != nullptr) {
		child->setParentPos(_position);
		rtbs.push_back(child);
	}
}

// Remove a external Draw function from the Applications drawing loop.
void PS4Forms::Form::Remove(void (*drawEvent)()) {
	drawFuncs.erase(std::remove(drawFuncs.begin(), drawFuncs.end(), drawEvent), drawFuncs.end());
}

// Remove a Form Object from this Form.
void PS4Forms::Form::Remove(Form *child) {
	if (child != nullptr) {
		forms.erase(std::remove(forms.begin(), forms.end(), child), forms.end());
	}
}

// Remove a BaseWidget Object from this Form.
void PS4Forms::Form::Remove(WidgetBase *child) {
	if (child != nullptr) {
		widgets.erase(std::remove(widgets.begin(), widgets.end(), child), widgets.end());
	}
}

// Remove a Label Object from this Form.
void PS4Forms::Form::Remove(Label *child) {
	if (child != nullptr) {
		labels.erase(std::remove(labels.begin(), labels.end(), child), labels.end());
	}
}

// Remove a Button Object from this Form.
void PS4Forms::Form::Remove(Button *child) {
	if (child != nullptr) {
		button.erase(std::remove(button.begin(), button.end(), child), button.end());
	}
}

// Remove a ScrollBar Object from this Form.
void PS4Forms::Form::Remove(ScrollBar *child) {
	if (child != nullptr) {
		sbar.erase(std::remove(sbar.begin(), sbar.end(), child), sbar.end());
	}
}

// Remove a CheckBox Object from this Form.
void PS4Forms::Form::Remove(CheckBox *child) {
	if (child != nullptr) {
		checkB.erase(std::remove(checkB.begin(), checkB.end(), child), checkB.end());
	}
}

// Remove a TextBox Object from this Form.
void PS4Forms::Form::Remove(TextBox *child) {
	if (child != nullptr) {
		textb.erase(std::remove(textb.begin(), textb.end(), child), textb.end());
	}
}

// Remove a PictureBox Object from this Form.
void PS4Forms::Form::Remove(PictureBox *child) {
	if (child != nullptr) {
		pictures.erase(std::remove(pictures.begin(), pictures.end(), child), pictures.end());
	}
}

// Remove a RichTextBox Object from this Form.
void PS4Forms::Form::Remove(RichTextBox *child) {
	if (child != nullptr) {
		rtbs.erase(std::remove(rtbs.begin(), rtbs.end(), child), rtbs.end());
	}
}

// Get a Widget based on his ID.
PS4Forms::WidgetBase *PS4Forms::Form::getWidgetById(String id) {
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a Form based on his ID.
PS4Forms::Form *PS4Forms::Form::getFormById(String id) {
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a Button based on his ID.
PS4Forms::Button *PS4Forms::Form::getButtonById(String id) {
	if (button.size() > 0) {
		for (std::vector<Button *>::iterator it = button.begin(); it != button.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a ScrollBar based on his ID.
PS4Forms::ScrollBar *PS4Forms::Form::getScrollBarById(String id) {
	if (sbar.size() > 0) {
		for (std::vector<ScrollBar *>::iterator it = sbar.begin(); it != sbar.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a CheckBox based on his ID.
PS4Forms::CheckBox *PS4Forms::Form::getCheckBoxById(String id) {
	if (checkB.size() > 0) {
		for (std::vector<CheckBox *>::iterator it = checkB.begin(); it != checkB.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a TextBox based on his ID.
PS4Forms::TextBox *PS4Forms::Form::getTextBoxById(String id) {
	if (textb.size() > 0) {
		for (std::vector<TextBox *>::iterator it = textb.begin(); it != textb.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a Label based on his ID.
PS4Forms::Label *PS4Forms::Form::getLabelById(String id) {
	if (labels.size() > 0) {
		for (std::vector<Label *>::iterator it = labels.begin(); it != labels.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a PictureBox based on his ID.
PS4Forms::PictureBox *PS4Forms::Form::getPictureBoxById(String id) {
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a RichTextBox based on his ID.
PS4Forms::RichTextBox *PS4Forms::Form::getRtbById(String id) {
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}
#pragma endregion Form

/* Ready to use */
#pragma region TextViewer
/*------------------------------------------------------------------
                   TextViewer Class
------------------------------------------------------------------*/
// Instance Initializer.
PS4Forms::TextViewer::TextViewer() {
	show = focus = closed = isFocused = loading = false;
	visible = visibleBoarder = true;
	_id = strdup("");
	_text = strdup("");
	_position = Position(0.0f, 0.04f);
	_size = Size(1.0f, 0.96f);
	_color = DARK_GRAY;
	_textColor = BLACK;
	_borderColor = LIGHT_GRAY;
	rtb = new RichTextBox();
	rtb->setPosition(Position(0.01f, 0.05f));
	rtb->setSize(Size(0.98f, 0.94f));
	rtb->setId("rtb_text_viewer");
	rtb->setBackGroundColor(BLACK);
	rtb->TextColor(_YELLOW);
}

// Instance Deinitializer.
PS4Forms::TextViewer::~TextViewer() {
	// Between the closing we tell the blocking function to return. So it will have some time before the whole object get killed.
	closed = true;
	rtb->~RichTextBox();
	free(_id);
	free(_text);
}

// Set the Foregound color of the rtb.
void PS4Forms::TextViewer::TextColor(uint32_t color) { rtb->TextColor(color); }

// Get the Foreground color of the rtb.
Color PS4Forms::TextViewer::getTextColor(void) {
	uint32_t _color = rtb->getTextColor();
	return Color((_color >> 24 & 0xff) / (float)0xff, (_color >> 16 & 0xff) / (float)0xff, (_color >> 8 & 0xff) / (float)0xff, (_color & 0xff) / (float)0xff);
}

// Update stuff.
void PS4Forms::TextViewer::Update(sce::SampleUtil::Input::PadContext *controller) {
	// Is Form showen ?
	if (show & visible) {
		if (controller->isButtonPressed(ssi::kButtonCircle, ssi::kButtonEventPatternAny)) {
			show = false;
		}

		// Let rtb do her updates.
		rtb->Update(controller);
	}
}

// Drawing the Form.
void PS4Forms::TextViewer::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Draw the form.
	if (show & visible) {
		// Sample down the loading string print out. 
		static int num = 0;

		// The Form.
		sprite->fillRect(graphicsContext, _position, _size, _color);

		// Draw a border.
		if (!focus) {
			sprite->fillRect(graphicsContext, Position(0.0f, 0.0f), Size(1.0f, 0.04f), WHITE);
			sprite->drawRect(graphicsContext, Position(0.0f, 0.04f), Size(1.0f, 0.96f), WHITE);
		} else {
			sprite->fillRect(graphicsContext, Position(0.0f, 0.0f), Size(1.0f, 0.04f), _borderColor);
			sprite->drawRect(graphicsContext, Position(0.0f, 0.04f), Size(1.0f, 0.96f), _borderColor);
		}

		// Draw Title into Border.
		// ToDo: center the titel.
		sprite->drawDebugStringf(graphicsContext, Position(_position.getX() + 0.01f, _position.getY() - 0.035f), 0.03f, _textColor, _text);

		// Draw the RichTextBox.
		rtb->Draw(graphicsContext, sprite);

		// Is a File loading in process ?
		if (loading) {
			// Draw a centered box to inform the user.
			sprite->fillRect(graphicsContext, Position(0.45f, 0.475f), Size(0.1f, 0.05f), _color);
			sprite->drawRect(graphicsContext, Position(0.45f, 0.475f), Size(0.1f, 0.05f), _borderColor);			

			// Write a nice dynamically 'loading' string with walking dots.
			if (num++ % 5) {				
				sprite->drawDebugStringf(graphicsContext, Position(0.47f, 0.476f), 0.03f, _textColor, _loading);
				if (strlen(_loading) == 8) {
					_loading[8] = '.';
					_loading[9] = '\0';
				} else if (strlen(_loading) == 9) {
					_loading[9] = '.';
					_loading[10] = '\0';
				} else {
					_loading[7] = '.';
					_loading[8] = '\0';
				}
			}
		}
	}
}

// Load file into buffer and write lines to screen.
int PS4Forms::TextViewer::LoadFile(const char *path) {
	if (path == nullptr) {
		Logger::Debug("[TextViewer] No Path overloaded.\n");
		return -1;
	} else if (path[0] == '\0') {
		Logger::Debug("[TextViewer] Path of file to load is empty.\n");
		return -2;
	}

	// Try to open the file.
	FILE *fd = fopen(path, "r");
	if (fd) {
		// Get File size.
		fseek(fd, 0, SEEK_END);
		long len = ftell(fd);
		rewind(fd);

		// Allocate buffer.
		buffer = (char *)malloc(len);

		// Fill up buffer with data.
		fread(buffer, 1, len, fd);

		// Close the file stream.
		fclose(fd);

		// Tell the drawing routine to draw a user information.
		loading = true;

		// Write some lines into the rtb.
		int toRead = 0;

		// Setup a temporary buffer and clear it.
		char temp[1000];
		memset(temp, 0, 1000);

		// Is the file size to read smaller then the buffer of a reading routine ?
		if (len - 1 < 992) toRead = len - 1;
		else toRead = 992;

		// Loop over the buffer and process data.
		int buffer_pointer = 0;
		while (true) {
			// copy Data into temp buffer.
			memcpy(temp, buffer + buffer_pointer, toRead);

			// Write the temporary buffer into the RichTextBox.
			rtb->Write(temp);

			// Count the readed characters up.
			buffer_pointer += toRead;

			// Have we reached the end of the file ?
			if (buffer_pointer == len) break;

			// Is the data left to read, smaller then the buffer to fill up ?
			if (len - buffer_pointer < toRead) toRead = len - buffer_pointer;

			// Clear the buffer before using.
			memset(temp, 0, 1000);
		}

		// Tell the user that we are done with loading the file into screen buffer.
		loading = false;

		// Free mem.
		free(buffer);

		return 0;
	} else Logger::Debug("Couldn't open file for reading.\n");
	return -3;
}
#pragma endregion TextViewer

/* Ready to use */
#pragma region HexViewer
/*------------------------------------------------------------------
					  HexViewer Class
------------------------------------------------------------------*/
// Instance Initializer.
PS4Forms::HexViewer::HexViewer() {
	show = focus = closed = isFocused = loading = false;
	visible = visibleBoarder = true;
	_id = strdup("");
	_text = strdup("");
	_position = Position(0.0f, 0.04f);
	_size = Size(1.0f, 0.96f);
	_color = DARK_GRAY;
	_textColor = BLACK;
	_borderColor = LIGHT_GRAY;
	rtb = new RichTextBox();
	rtb->setPosition(Position(0.01f, 0.05f));
	rtb->setSize(Size(0.98f, 0.94f));
	rtb->setId("rtb_text_viewer");
	rtb->setBackGroundColor(BLACK);
	rtb->TextColor(_YELLOW);
	rtb->setY(rtb->getY() + 0.03f);
}

// Instance Deinitializer.
PS4Forms::HexViewer::~HexViewer() {
	// Between the closing we tell the blocking function to return. So it will have some time before the whole object get killed.
	closed = true;
	rtb->~RichTextBox();
	free(_id);
	free(_text);
}

// Drawing the Form.
void PS4Forms::HexViewer::Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) {
	// Draw the form.
	if (show & visible) {
		// Sample down the loading string print out. 
		static int num = 0;

		// The Form.
		sprite->fillRect(graphicsContext, Position(0.0f, 0.04f), Size(1.0f, 0.96f), _color);

		// Draw a border.
		if (!focus) {
			sprite->fillRect(graphicsContext, Position(0.0f, 0.0f), Size(1.0f, 0.04f), WHITE);
			sprite->drawRect(graphicsContext, Position(0.0f, 0.04f), Size(1.0f, 0.96f), WHITE);
		} else {
			sprite->fillRect(graphicsContext, Position(0.0f, 0.0f), Size(1.0f, 0.04f), _borderColor);
			sprite->drawRect(graphicsContext, Position(0.0f, 0.04f), Size(1.0f, 0.96f), _borderColor);
		}

		// Draw Title into Border.
		sprite->drawDebugStringf(graphicsContext, Position(_position.getX() + 0.01f, _position.getY() - 0.035f), 0.03f, LIGHT_BLUE, _text);

		// Draw fixed info line into rtb.
		sprite->drawDebugStringf(graphicsContext, Position(0.0f + rtb->getX(), 0.04f + rtb->getY() - 0.03f), 0.03f, WHITE, " Offset   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F   UTF-8");

		// Draw the RichTextBox.
		rtb->Draw(graphicsContext, sprite);

		// Is a File loading in process ?
		if (loading) {
			// Draw a centered box to inform the user.
			sprite->fillRect(graphicsContext, Position(0.45f, 0.475f), Size(0.1f, 0.05f), _color);
			sprite->drawRect(graphicsContext, Position(0.45f, 0.475f), Size(0.1f, 0.05f), _borderColor);

			// Write a nice dynamically 'loading' string with walking dots.
			if (num++ % 5) {
				sprite->drawDebugStringf(graphicsContext, Position(0.47f, 0.476f), 0.03f, _textColor, _loading);
				if (strlen(_loading) == 8) {
					_loading[8] = '.';
					_loading[9] = '\0';
				}
				else if (strlen(_loading) == 9) {
					_loading[9] = '.';
					_loading[10] = '\0';
				}
				else {
					_loading[7] = '.';
					_loading[8] = '\0';
				}
			}
		}
	}
}

// Load file into buffer and write lines to screen.
int PS4Forms::HexViewer::LoadFile(const char *path) {
	if (path == nullptr) {
		Logger::Debug("[TextViewer] No Path overloaded.\n");
		return -1;
	}
	else if (path[0] == '\0') {
		Logger::Debug("[TextViewer] Path of file to load is empty.\n");
		return -2;
	}

	// Try to open the file.
	FILE *fd = fopen(path, "r");
	if (fd) {
		// Get File size.
		fseek(fd, 0, SEEK_END);
		long len = ftell(fd);
		rewind(fd);

		// Allocate buffer.
		buffer = (byte *)malloc(len);

		// Fill up buffer with data.
		fread(buffer, 1, len, fd);

		// Close the file stream.
		fclose(fd);

		// Tell the drawing routine to draw a user information.
		loading = true;

		// Write some lines into the rtb.
		int toRead = 0;

		// Setup a temporary buffer and clear it.
		byte temp[16];
		char tempChar[17];
		memset(temp, 0, 16);
		memset(tempChar, 0, 17);

		// Is the file size to read smaller then the buffer of a reading routine ?
		if (len - 1 < 16) toRead = len - 1;
		else toRead = 16;

		// Format to a nice Hex Viewer Style output.
		String out;
		String hexifyed;

		// Loop over the buffer and process data.
		int buffer_pointer = 0;
		int offset = 0;
		while (true) {
			// copy Data into temp buffer.
			memcpy(temp, buffer + buffer_pointer, toRead);

			// Count the readed characters up.
			buffer_pointer += toRead;

			// Format bytes to a Hex String.
			hexifyed = SwissKnife::ToHexString(temp, toRead);			

			// Get Offset as Hex String.
			out += SwissKnife::ToHexString(&offset, 4);

			// Count offset up.
			offset += toRead;

			// Add some padding.
			out += "  ";

			// Add Hex Bytes to it.
			for (int i = 0; i < hexifyed.size(); i += 2) {
				out += hexifyed[i];
				out += hexifyed[i + 1];
				out += " ";
			}

			// Add some padding.
			out += "  ";

			// Add Bytes as String to it.			
			memcpy(tempChar, temp, toRead);
			tempChar[toRead] = '\0';
			out += tempChar;
			out += '\n';			

			// Write the temporary buffer into the RichTextBox.
			rtb->Write(out.c_str());			

			// Have we reached the end of the file ?
			if (buffer_pointer == len) break;

			// Is the data left to read, smaller then the buffer to fill up ?
			if (len - buffer_pointer < toRead) toRead = len - buffer_pointer;			

			// Clear the buffer before using.
			memset(temp, 0, 16);
			memset(tempChar, 0, 17);
		}

		// Tell the user that we are done with loading the file into screen buffer.
		loading = false;

		// Free mem.
		free(buffer);

		return 0;
	}
	else Logger::Debug("Couldn't open file for reading.\n");
	return -3;
}
#pragma endregion HexViewer