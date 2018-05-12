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

#include <algorithm>
#include "ps4_forms.h"

/*-----------------------------------------------------------------
                 PS4Forms Namespace
				  Static Functions
------------------------------------------------------------------*/
// Calculate if the cursor/stick is within the Range of this object.
bool PS4Forms::IsObjectFocus(float x, float y, vecmath::Vector2_arg _position, vecmath::Vector2_arg _size) {
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


/*-----------------------------------------------------------------

-------------------------------------------------------------------*/
// Get Text aka Title of this form.
char *PS4Forms::WidgetBase::getText(void) { return strdup(_text); }

// Set Text aka Title of this form.
void PS4Forms::WidgetBase::Text(const char *text) { delete _text; _text = strdup(text); }

// Set Position of this form.
void PS4Forms::WidgetBase::Position(vecmath::Vector2_arg position) { _position = position; }

// Get Position of this form.
vecmath::Vector2_arg PS4Forms::WidgetBase::getPosition(void) { return _position; }

// Set Size of this form.
void PS4Forms::WidgetBase::Size(vecmath::Vector2_arg size) { _size = size; }

// Get Size of this form.
vecmath::Vector2_arg PS4Forms::WidgetBase::getSize(void) { return _size; }

// Set Color of this form.
void PS4Forms::WidgetBase::Color(vecmath::Vector4_arg color) { _color = color; }

// Get Color of this form.
vecmath::Vector4_arg PS4Forms::WidgetBase::getColor(void) { return _color; }

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
	/*if (IsObjectFocus(x, y, _position, vecmath::Vector2(_size, strlen(_text)))) focus = true;
	else focus = false;*/
	return focus;
}

// Set the SpriteRenderer.
void PS4Forms::WidgetBase::SetSprite(ssg::SpriteRenderer *sprite) { _sprite = sprite; }

// Get Cursor/stick condition for this form. True means cursor, stick is within the form. False means it is not.
bool PS4Forms::WidgetBase::isFocus(void) { return focus; }

// Get actual show state of this Form. True means the Form is showen, false means the form is hiding.
bool PS4Forms::WidgetBase::isShowen(void) { return show; }

// Set parent of a child.
void PS4Forms::WidgetBase::Parent(WidgetBase *widget) { parent = widget; }

// Drawing the Form and all his childrens if any.
void PS4Forms::WidgetBase::Draw(ssg::GraphicsContext *graphicsContext) { }


/*-----------------------------------------------------------------
                 Label Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::Label::Label() {
	show = focus = isFocused = false;
	visible = true;
	_text = strdup("");
	_position = vecmath::Vector2(0, 0);
	_size = CHAR_N;
	_color = WHITE;
}

// Class Initializer.
PS4Forms::Label::Label(const char *text, vecmath::Vector2 position, float size, vecmath::Vector4 color) {
	show = focus = isFocused = false;
	visible = true;
	_text = strdup(text);
	_position = position;
	_size = size;
	_color = color;
}

// Class Deinitializer.
PS4Forms::Label::~Label() { delete _text; }

// Set Text aka Title of this form.
void PS4Forms::Label::Text(const char *text) { delete _text; _text = strdup(text); }

// Get Text aka Title of this form.
char *PS4Forms::Label::getText(void) { return strdup(_text); }

// Set Position of this form.
void PS4Forms::Label::Position(vecmath::Vector2_arg position) { _position = position; }

// Get Position of this form.
vecmath::Vector2_arg PS4Forms::Label::getPosition(void) { return _position; }

// Set Size of this form.
void PS4Forms::Label::Size(float size) { _size = size; }

// Get Size of this form.
float PS4Forms::Label::getSize(void) { return _size; }

// Set Color of this form.
void PS4Forms::Label::Color(vecmath::Vector4_arg color) { _color = color; }

// Get Color of this form.
vecmath::Vector4_arg PS4Forms::Label::getColor(void) { return _color; }

// Show Form.
void PS4Forms::Label::Show(void) { show = true; }

// Add a function Thread to the funcs vector.
void PS4Forms::Label::AddFunc(ThreadFunc *func) { if (func != nullptr) funcs.push_back(func); }

// Add a function Thread to the funcs vector.
void PS4Forms::Label::AddFunc(off_t *func) {
	// Initialize a new ThreadFunc class object.
	ThreadFunc *newFunc = new ThreadFunc();

	// Mount function to the symbol.
	newFunc->Func = (void *(*)(void *))func;

	// Add new psydo click event to the list.
	funcs.push_back(newFunc);
}

// Remove a function Thread from the funcs vector.
void PS4Forms::Label::RemoveFunc(ThreadFunc *func) {
	if (func != nullptr) {
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if ((*it) == func) (*it)->~ThreadFunc();
			funcs.erase(it);
		}
	}
}

// Remove a function Thread from the funcs vector.
void PS4Forms::Label::RemoveFunc(off_t *func) {
	if (func != nullptr) {
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if ((*it)->Func == (void *(*)(void *))func) (*it)->~ThreadFunc();
			funcs.erase(it);
		}
	}
}

// Sets label visabillity.
void PS4Forms::Label::Visible(bool Visible) { visible = Visible; }

// Gets label actual visible state.
bool PS4Forms::Label::isVisible(void) { return visible; }

// Psydo Event. Will set clicked flag to true if object hase focus.
void PS4Forms::Label::XPressed(void) {
	if (focus) clicked = true;
	else clicked = false;
}

// Another Psydo Event. Will run all functions within the funcs vector aslong their Thread is not running.
void PS4Forms::Label::ClickEvent(void) {
	if (clicked) {
		int counter = 0;
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if (counter == 0) {
				if (thread0 == NULL) {
					scePthreadCreate(&thread0, NULL, (*it)->Func, NULL, "LabelClickEvent0");
				}
			} else if (counter == 1) {
				if (thread1 == NULL) {
					scePthreadCreate(&thread1, NULL, (*it)->Func, NULL, "LabelClickEvent1");
				}
			} else if (counter == 2) {
				if (thread2 == NULL) {
					scePthreadCreate(&thread2, NULL, (*it)->Func, NULL, "LabelClickEvent2");
				}
			} else if (counter == 3) {
				if (thread3 == NULL) {
					scePthreadCreate(&thread3, NULL, (*it)->Func, NULL, "LabelClickEvent3");
				}
			} else if (counter == 4) {
				if (thread4 == NULL) {
					scePthreadCreate(&thread4, NULL, (*it)->Func, NULL, "LabelClickEvent4");
				}
			} else if (counter == 5) {
				if (thread5 == NULL) {
					scePthreadCreate(&thread5, NULL, (*it)->Func, NULL, "LabelClickEvent5");
				}
			} else if (counter == 6) {
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
void PS4Forms::Label::Hide(void) { show = false; }

// Close Form.
void PS4Forms::Label::Close(void) { this->~Label(); }

// Set this Object to master focus. Will be drawen at last, so displayed as first.
void PS4Forms::Label::Focused(bool focused) { isFocused = focused; }

// Set the SpriteRenderer.
void PS4Forms::Label::SetSprite(ssg::SpriteRenderer *sprite) { _sprite = sprite; }

// Compares the updated cursor postion against the position and size of the form and will set 'isCursorRange' to true, if the stick, cursor is within our form.
bool PS4Forms::Label::UpdateCursor(float x, float y) {
	if (IsObjectFocus(x, y, _position, vecmath::Vector2(_size, strlen(_text)))) focus = true;
	else focus = false;
	return focus;
}

// Get Cursor/stick condition for this form. True means cursor, stick is within the form. False means it is not.
bool PS4Forms::Label::isFocus(void) { return focus; }

// Get actual show state of this Form. True means the Form is showen, false means the form is hiding.
bool PS4Forms::Label::isShowen(void) { return show; }

// Drawing the Form and all his childrens if any.
void PS4Forms::Label::Draw(ssg::GraphicsContext *graphicsContext) {
	// Draw the label.
	if (show & visible) _sprite->drawDebugStringf(graphicsContext, _position, _size, _color, _text);
}


/*------------------------------------------------------------------
                  TextBox Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::TextBox::TextBox() {
	show = focus = isFocused = false;
	visible = true;
	_text = strdup("");
	_position = vecmath::Vector2(0, 0);
	_size = vecmath::Vector2(0.03, 0.3);
	_textSize = CHAR_N;
	_textColor = BLACK;
	_color = WHITE;
}

// Class Initializer.
PS4Forms::TextBox::TextBox(const char *text, vecmath::Vector2 position, vecmath::Vector2 size, float textSize, vecmath::Vector4 color, vecmath::Vector4 textColor) {
	show = focus = isFocused = false;
	visible = true;
	_text = strdup(text);
	_position = position;
	_size = size;
	_textSize = textSize;
	_textColor = textColor;
	_color = color;
}

// Class Deinitializer.
PS4Forms::TextBox::~TextBox() { delete _text; }

// Set Text aka Title of this form.
void PS4Forms::TextBox::Text(const char *text) { delete _text; _text = strdup(text); }

// Get Text aka Title of this form.
char *PS4Forms::TextBox::getText(void) { return strdup(_text); }

// Set Position of this form.
void PS4Forms::TextBox::Position(vecmath::Vector2_arg position) { _position = position; }

// Get Position of this form.
vecmath::Vector2_arg PS4Forms::TextBox::getPosition(void) { return _position; }

// Set Size of this form.
void PS4Forms::TextBox::Size(vecmath::Vector2_arg size) { _size = size; }

// Get Size of this form.
vecmath::Vector2_arg PS4Forms::TextBox::getSize(void) { return _size; }

// Set Color of this form.
void PS4Forms::TextBox::Color(vecmath::Vector4_arg color) { _color = color; }

// Get Color of this form.
vecmath::Vector4_arg PS4Forms::TextBox::getColor(void) { return _color; }

// Set Titel Color of this form.
void PS4Forms::TextBox::TextColor(vecmath::Vector4_arg color) { _textColor = color; }

// Set the Borders Color.
void PS4Forms::TextBox::BorderColor(vecmath::Vector4_arg color) { _borderColor = color; }

// Get the Borders Color.
vecmath::Vector4_arg PS4Forms::TextBox::getBorderColor(void) { return _borderColor; }

// Get Titel Color of this form.
vecmath::Vector4_arg PS4Forms::TextBox::getTextColor(void) { return _textColor; }

// Set text size.
void PS4Forms::TextBox::Textize(float size) { _textSize = size; }

// Get text size.
float PS4Forms::TextBox::getTextSize(void) { return _textSize; }

// Show Form.
void PS4Forms::TextBox::Show(void) { show = true; }

// Add a function Thread to the funcs vector.
void PS4Forms::TextBox::AddFunc(ThreadFunc *func) { if (func != nullptr) funcs.push_back(func); }

// Add a function Thread to the funcs vector.
void PS4Forms::TextBox::AddFunc(off_t *func) {
	// Initialize a new ThreadFunc class object.
	ThreadFunc *newFunc = new ThreadFunc();

	// Mount function to the symbol.
	newFunc->Func = (void *(*)(void *))func;

	// Add new psydo click event to the list.
	funcs.push_back(newFunc);
}

// Remove a function Thread from the funcs vector.
void PS4Forms::TextBox::RemoveFunc(ThreadFunc *func) {
	if (func != nullptr) {
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if ((*it) == func) (*it)->~ThreadFunc();
			funcs.erase(it);
		}
	}
}

// Remove a function Thread from the funcs vector.
void PS4Forms::TextBox::RemoveFunc(off_t *func) {
	if (func != nullptr) {
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if ((*it)->Func == (void *(*)(void *))func) (*it)->~ThreadFunc();
			funcs.erase(it);
		}
	}
}

// Sets label visabillity.
void PS4Forms::TextBox::Visible(bool Visible) { visible = Visible; }

// Gets label actual visible state.
bool PS4Forms::TextBox::isVisible(void) { return visible; }

// Psydo Event. Will set clicked flag to true if object hase focus.
void PS4Forms::TextBox::XPressed(void) {
	if (focus) clicked = true;
	else clicked = false;
}

// Another Psydo Event. Will run all functions within the funcs vector aslong their Thread is not running.
void PS4Forms::TextBox::ClickEvent(void) {
	if (clicked) {
		int counter = 0;
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if (counter == 0) {
				if (thread0 == NULL) {
					scePthreadCreate(&thread0, NULL, (*it)->Func, NULL, "LabelClickEvent0");
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
void PS4Forms::TextBox::Hide(void) { show = false; }

// Close Form.
void PS4Forms::TextBox::Close(void) { this->~TextBox(); }

// Set this Object to master focus. Will be drawen at last, so displayed as first.
void PS4Forms::TextBox::Focused(bool focused) { isFocused = focused; }

// Set the SpriteRenderer.
void PS4Forms::TextBox::SetSprite(ssg::SpriteRenderer *sprite) { _sprite = sprite; }

// Compares the updated cursor postion against the position and size of the form and will set 'isCursorRange' to true, if the stick, cursor is within our form.
bool PS4Forms::TextBox::UpdateCursor(float x, float y) {
	if (IsObjectFocus(x, y, _position, vecmath::Vector2(_textSize, strlen(_text)))) focus = true;
	else focus = false;
	return focus;
}

// Get Cursor/stick condition for this form. True means cursor, stick is within the form. False means it is not.
bool PS4Forms::TextBox::isFocus(void) { return focus; }

// Get actual show state of this Form. True means the Form is showen, false means the form is hiding.
bool PS4Forms::TextBox::isShowen(void) { return show; }

// Drawing the Form and all his childrens if any.
void PS4Forms::TextBox::Draw(ssg::GraphicsContext *graphicsContext) {
	// Draw the label.
	if (show & visible) {
		_sprite->fillRect(graphicsContext, _position, _size, WHITE);
		_sprite->drawRect(graphicsContext, _position, _size, BLACK);
		_sprite->drawDebugStringf(graphicsContext, vecmath::Vector2(_position.getX() + 0.01, _position.getY() + 0.01), _textSize, _color, _text);
	}
}


/*------------------------------------------------------------------
                  Image Class
-------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::Image::Image() {
	show = focus = clicked = isFocused = false;
	visible = true;
}

// Sets label visabillity.
void PS4Forms::Image::Visible(bool Visible) { visible = Visible; }

// Gets label actual visible state.
bool PS4Forms::Image::isVisible(void) { return visible; }

// Show Form.
void PS4Forms::Image::Show(void) { show = true; }

// Set Position of this form.
void PS4Forms::Image::Position(vecmath::Vector2_arg position) { _position = position; }

// Get Position of this form.
vecmath::Vector2_arg PS4Forms::Image::getPosition(void) { return _position; }

// Set Size of this form.
void PS4Forms::Image::Size(vecmath::Vector2_arg size) { _size = size; }

// Get Size of this form.
vecmath::Vector2_arg PS4Forms::Image::getSize(void) { return _size; }

// Set Texture of this instance.
void PS4Forms::Image::setTexture(sce::SampleUtil::Graphics::Texture *texture) { _texture = texture; }

// Set H Allign.
void PS4Forms::Image::setHAlign(HAlign hAlign) { _hAlign = hAlign; }

// Set V Allign.
void PS4Forms::Image::setVAlign(VAlign vAlign) { _vAlign = vAlign; }

// Set Fill mode.
void PS4Forms::Image::setFillMode(FillMode fillMode) { _fillMode = fillMode; }

// Hide Form.
void PS4Forms::Image::Hide(void) { show = false; }

// Close Form.
void PS4Forms::Image::Close(void) { this->~Image(); }

// Set this Object to master focus. Will be drawen at last, so displayed as first.
void PS4Forms::Image::Focused(bool focused) { isFocused = focused; }

// Set the SpriteRenderer.
void PS4Forms::Image::SetSprite(ssg::SpriteRenderer *sprite) { _sprite = sprite; }

// Compares the updated cursor postion against the position and size of the form and will set 'isCursorRange' to true, if the stick, cursor is within our form.
bool PS4Forms::Image::UpdateCursor(float x, float y) {
	if (IsObjectFocus(x, y, _position, vecmath::Vector2(0, 0))) focus = true;
	else focus = false;
	return focus;
}

// Get Cursor/stick condition for this form. True means cursor, stick is within the form. False means it is not.
bool PS4Forms::Image::isFocus(void) { return focus; }

// Get actual show state of this Form. True means the Form is showen, false means the form is hiding.
bool PS4Forms::Image::isShowen(void) { return show; }

// Draw the image to the screen.
void PS4Forms::Image::draw(sce::SampleUtil::Graphics::GraphicsContext *context) {
	if (_texture == NULL) {
		//printf("texture is null : %s(%p)\n", getId().c_str(), this);
		return;
	}
	if ((_texture->getWidth() == 0) || (_texture->getHeight() == 0)) {
		//printf("texture size is zero\n");
		return;
	}
	vecmath::Vector2 windowPosition = _position;
	vecmath::Vector2 windowSize = _size;
	vecmath::Vector2 windowCenterPoition = windowPosition + 0.5 * windowSize;
	vecmath::Vector2 rtSize(context->getCurrentRenderTarget()->getWidth(), context->getCurrentRenderTarget()->getHeight());
	vecmath::Vector2 texSize(_texture->getWidth(), _texture->getHeight());

	float aspectRatio = rtSize.getX() / rtSize.getY();
	float aspectRatioInv = rtSize.getY() / rtSize.getX();
	vecmath::Vector2 drawSize;
	switch (_fillMode) {
	case kFillModeDotByDot:
		drawSize = divPerElem(texSize, rtSize);
		break;
	case kFillModeExpand:
		drawSize = windowSize;
		break;
	case kFillModeVExpand:
		drawSize = vecmath::Vector2(aspectRatioInv * windowSize.getY() * texSize.getX() / texSize.getY(), windowSize.getY());
		break;
	case kFillModeHExpand:
		drawSize = vecmath::Vector2(windowSize.getX(), aspectRatio * windowSize.getX() * texSize.getY() / texSize.getX());
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

	_sprite->drawTexture(context, vecmath::Vector2(drawX, drawY), drawSize, _texture);
}


/*------------------------------------------------------------------
                  PictureBox Class
-------------------------------------------------------------------*/



/*------------------------------------------------------------------
                  RichTextBox Class
-------------------------------------------------------------------*/




/*------------------------------------------------------------------
                  Button Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::Button::Button() {
	show = focus = isFocused = false;
	visible = true;
	charsize = CHAR_S;
	_text = strdup("");
	_position = vecmath::Vector2(0, 0);
	_size = vecmath::Vector2(0.05, 0.10);
	_color = BLUE;
	_borderColor = LIGHT_GRAY;
	_textColor = BLACK;
}

// Class Initializer.
PS4Forms::Button::Button(const char *text, float _charsize, vecmath::Vector2 position, vecmath::Vector2 size, vecmath::Vector4 color, vecmath::Vector4 textcolor) {
	show = focus = isFocused = false;
	visible = true;
	charsize = _charsize;
	_text = strdup(text);
	_position = position;
	_size = size;
	_color = color;
	_textColor = textcolor;
	_borderColor = LIGHT_GRAY;
}

// Class Deinitializer.
PS4Forms::Button::~Button() { delete _text; }

// Set Text aka Title of this form.
void PS4Forms::Button::Text(const char *text) { delete _text; _text = strdup(text); }

// Get Text aka Title of this form.
char *PS4Forms::Button::getText(void) { return strdup(_text); }

// Set Position of this form.
void PS4Forms::Button::Position(vecmath::Vector2_arg position) { _position = position; }

// Get Position of this form.
vecmath::Vector2_arg PS4Forms::Button::getPosition(void) { return _position; }

// Set Size of this form.
void PS4Forms::Button::Size(vecmath::Vector2_arg size) { _size = size; }

// Get Size of this form.
vecmath::Vector2_arg PS4Forms::Button::getSize(void) { return _size; }

// Set Color of this form.
void PS4Forms::Button::Color(vecmath::Vector4_arg color) { _color = color; }

// Get Color of this form.
vecmath::Vector4_arg PS4Forms::Button::getColor(void) { return _color; }

// Set Titel Color of this form.
void PS4Forms::Button::TextColor(vecmath::Vector4_arg color) { _textColor = color; }

// Set the Borders Color.
void PS4Forms::Button::BorderColor(vecmath::Vector4_arg color) { _borderColor = color; }

// Get the Borders Color.
vecmath::Vector4_arg PS4Forms::Button::getBorderColor(void) { return _borderColor; }

// Get Titel Color of this form.
vecmath::Vector4_arg PS4Forms::Button::getTextColor(void) { return _textColor; }

// Add a function Thread to the funcs vector.
void PS4Forms::Button::AddFunc(ThreadFunc *func) { if (func != nullptr) funcs.push_back(func); }

// Add a function Thread to the funcs vector.
void PS4Forms::Button::AddFunc(off_t *func) {
	// Initialize a new ThreadFunc class object.
	ThreadFunc *newFunc = new ThreadFunc();

	// Mount function to the symbol.
	newFunc->Func = (void *(*)(void *))func;

	// Add new psydo click event to the list.
	funcs.push_back(newFunc);
}

// Remove a function Thread from the funcs vector.
void PS4Forms::Button::RemoveFunc(ThreadFunc *func) {
	if (func != nullptr) {
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if ((*it) == func) (*it)->~ThreadFunc();
			funcs.erase(it);
		}
	}
}

// Remove a function Thread from the funcs vector.
void PS4Forms::Button::RemoveFunc(off_t *func) {
	if (func != nullptr) {
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if ((*it)->Func == (void *(*)(void *))func) (*it)->~ThreadFunc();
			funcs.erase(it);
		}
	}
}

// Sets label visabillity.
void PS4Forms::Button::Visible(bool Visible) { visible = Visible; }

// Gets label actual visible state.
bool PS4Forms::Button::isVisible(void) { return visible; }

// Psydo Event. Will set clicked flag to true if object hase focus.
void PS4Forms::Button::XPressed(void) {
	if (focus) clicked = true;
	else clicked = false;
}

// Another Psydo Event. Will run all functions within the funcs vector aslong their Thread is not running.
void PS4Forms::Button::ClickEvent(void) {
	if (clicked) {
		int counter = 0;
		for (std::vector<ThreadFunc *>::iterator it = funcs.begin(); it != funcs.end(); it++) {
			if (counter == 0) {
				if (thread0 == NULL) {
					scePthreadCreate(&thread0, NULL, (*it)->Func, NULL, "LabelClickEvent0");
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

// Show Form.
void PS4Forms::Button::Show(void) { show = true; }

// Hide Form.
void PS4Forms::Button::Hide(void) { show = false; }

// Close Form.
void PS4Forms::Button::Close(void) { this->~Button(); }

// Set this Object to master focus. Will be drawen at last, so displayed as first.
void PS4Forms::Button::Focused(bool focused) { isFocused = focused; }

// Set the SpriteRenderer.
void PS4Forms::Button::SetSprite(ssg::SpriteRenderer *sprite) { _sprite = sprite; }

// Compares the updated cursor postion against the position and size of the form and will set 'isCursorRange' to true, if the stick, cursor is within our form.
bool PS4Forms::Button::UpdateCursor(float x, float y) {
	if (IsObjectFocus(x, y, _position, _size)) focus = true;
	else focus = false;
	return focus;
}

// Get Cursor/stick condition for this form. True means cursor, stick is within the form. False means it is not.
bool PS4Forms::Button::isFocus(void) { return focus; }

// Get actual show state of this Form. True means the Form is showen, false means the form is hiding.
bool PS4Forms::Button::isShowen(void) { return show; }

// Drawing the Form and all his childrens if any.
void PS4Forms::Button::Draw(ssg::GraphicsContext *graphicsContext) {	
	// Shall we draw ?
	if (show & visible) {
		// Draw the form.
		if (!focus) _sprite->fillRect(graphicsContext, _position, _size, _color);
		else _sprite->fillRect(graphicsContext, _position, _size, WHITE);

		// Draw Border.
		if (!focus) _sprite->drawRect(graphicsContext, _position, _size, _borderColor);
		else _sprite->drawRect(graphicsContext, _position, _size, BLACK);

		// Draw Text into the button.
		_sprite->drawDebugStringf(graphicsContext, vecmath::Vector2(_position.getX() + 0.01, _position.getY() + charsize + 0.005), charsize, _textColor, _text);
	}
}


/*------------------------------------------------------------------
                  ScrollBar Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::ScrollBar::ScrollBar() {
	show = focus = isFocused = false;
	visible = true;
	_position = vecmath::Vector2(0, 0);
	_size0 = vecmath::Vector2(0.001f, SCREEN_PROPORTION);
	_size = vecmath::Vector2(SCROLL_BAR_SIZE, SCROLL_BAR_SIZE);
	y = 1.0f - SCREEN_PROPORTION;
	_color = BLUE;
	_borderColor = LIGHT_GRAY;
	head = 0;	
	linesWritten = 0;
	maxLines = 0;
}

// Class Initializer.
PS4Forms::ScrollBar::ScrollBar(vecmath::Vector2_arg position, vecmath::Vector4 color) {
	show = focus = isFocused = false;
	visible = true;
	_position = position;
	_size0 = vecmath::Vector2(0.001f, SCREEN_PROPORTION);
	_size = vecmath::Vector2(SCROLL_BAR_SIZE, SCROLL_BAR_SIZE);
	y = 1.0f - SCREEN_PROPORTION;
	_color = color;
	_borderColor = LIGHT_GRAY;
	head = 0;
	linesWritten = 0;
	maxLines = 0;
}

// Set Position of this form.
void PS4Forms::ScrollBar::Position(vecmath::Vector2_arg position) { _position = position; }

// Get Position of this form.
vecmath::Vector2_arg PS4Forms::ScrollBar::getPosition(void) { return _position; }

// Set Size of this form.
void PS4Forms::ScrollBar::Size(vecmath::Vector2_arg size) { _size = size; }

// Get Size of this form.
vecmath::Vector2_arg PS4Forms::ScrollBar::getSize(void) { return _size; }

// Set Color of this form.
void PS4Forms::ScrollBar::Color(vecmath::Vector4_arg color) { _color = color; }

// Get Color of this form.
vecmath::Vector4_arg PS4Forms::ScrollBar::getColor(void) { return _color; }

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
void PS4Forms::ScrollBar::BorderColor(vecmath::Vector4_arg color) { _borderColor = color; }

// Get the Borders Color.
vecmath::Vector4_arg PS4Forms::ScrollBar::getBorderColor(void) { return _borderColor; }

// Sets label visabillity.
void PS4Forms::ScrollBar::Visible(bool Visible) { visible = Visible; }

// Gets label actual visible state.
bool PS4Forms::ScrollBar::isVisible(void) { return visible; }

// Set Focus for the scrollbat.
void PS4Forms::ScrollBar::Focus(bool _focus) { focus = _focus; }

// Set base x position.
void PS4Forms::ScrollBar::InitX(float _x) { x = _x; }

// Show Form.
void PS4Forms::ScrollBar::Show(void) { show = true; }

// Hide Form.
void PS4Forms::ScrollBar::Hide(void) { show = false; }

// Close Form.
void PS4Forms::ScrollBar::Close(void) { this->~ScrollBar(); }

// Set the SpriteRenderer.
void PS4Forms::ScrollBar::SetSprite(ssg::SpriteRenderer *sprite) { _sprite = sprite; }

// Get Cursor/stick condition for this form. True means cursor, stick is within the form. False means it is not.
bool PS4Forms::ScrollBar::isFocus(void) { return focus; }

// Get actual show state of this Form. True means the Form is showen, false means the form is hiding.
bool PS4Forms::ScrollBar::isShowen(void) { return show; }

// Drawing the Form and all his childrens if any.
void PS4Forms::ScrollBar::Draw(ssg::GraphicsContext *graphicsContext) {
	// Draw the form.
	if (show & visible) {
		if (focus) {
			float _y = (y + ((((float)head / (float)(linesWritten - maxLines)) * SCREEN_PROPORTION) * (1.0f - SCROLL_BAR_SIZE*2.0f)));
			_sprite->fillRect(graphicsContext, vecmath::Vector2(x, y), _size0, _color);
			_sprite->fillRect(graphicsContext, vecmath::Vector2(x + SCROLL_BAR_SIZE, y), _size0, _color);
			_sprite->fillRect(graphicsContext, vecmath::Vector2(x, _y), _size, _color);
		}
	}
}


/*------------------------------------------------------------------
                  CheckBox Class
------------------------------------------------------------------*/
// Class initializer.
PS4Forms::CheckBox::CheckBox() {
	show = focus = isFocused = false;
	visible = true;
	_text = strdup("");
	_position = vecmath::Vector2(0, 0);
	_size = vecmath::Vector2(0.02, 0.02);
	_color = WHITE;
	_textColor = BLACK;
	_borderColor = LIGHT_GRAY;
}

// Class initializer.
PS4Forms::CheckBox::CheckBox(const char *text, vecmath::Vector2 position, vecmath::Vector4 color, vecmath::Vector4 textcolor) {
	show = focus = isFocused = false;
	visible = true;
	_text = strdup(text);
	_position = position;
	_size = vecmath::Vector2(0.02, 0.02);
	_color = color;
	_textColor = textcolor;
	_borderColor = LIGHT_GRAY;
}

// Class deinitializer.
PS4Forms::CheckBox::~CheckBox() { delete _text; }

// Set Text aka Title of this form.
void PS4Forms::CheckBox::Text(const char *text) { delete _text; _text = strdup(text); }

// Get Text aka Title of this form.
char *PS4Forms::CheckBox::getText(void) { return strdup(_text); }

// Set Position of this form.
void PS4Forms::CheckBox::Position(vecmath::Vector2_arg position) { _position = position; }

// Get Position of this form.
vecmath::Vector2_arg PS4Forms::CheckBox::getPosition(void) { return _position; }

// Set Color of this form.
void PS4Forms::CheckBox::Color(vecmath::Vector4_arg color) { _color = color; }

// Get Color of this form.
vecmath::Vector4_arg PS4Forms::CheckBox::getColor(void) { return _color; }

// Set Titel Color of this form.
void PS4Forms::CheckBox::TextColor(vecmath::Vector4_arg color) { _textColor = color; }

// Get Titel Color of this form.
vecmath::Vector4_arg PS4Forms::CheckBox::getTextColor(void) { return _textColor; }

// Set the Borders Color.
void PS4Forms::CheckBox::BorderColor(vecmath::Vector4_arg color) { _borderColor = color; }

// Get the Borders Color.
vecmath::Vector4_arg PS4Forms::CheckBox::getBorderColor(void) { return _borderColor; }

// Sets label visabillity.
void PS4Forms::CheckBox::Visible(bool Visible) { visible = Visible; }

// Gets label actual visible state.
bool PS4Forms::CheckBox::isVisible(void) { return visible; }

// Show Form.
void PS4Forms::CheckBox::Show(void) { show = true; }

// Hide Form.
void PS4Forms::CheckBox::Hide(void) { show = false; }

// Close Form.
void PS4Forms::CheckBox::Close(void) { this->~CheckBox(); }

// Psydo Event. Will set clicked flag to true if object hase focus.
void PS4Forms::CheckBox::XPressed(void) {
	if (focus) clicked = true;
	else clicked = false;
}

// Set this Object to master focus. Will be drawen at last, so displayed as first.
void PS4Forms::CheckBox::Focused(bool focused) { isFocused = focused; }

// Set the SpriteRenderer.
void PS4Forms::CheckBox::SetSprite(ssg::SpriteRenderer *sprite) { _sprite = sprite; }

// Return checked state.
bool PS4Forms::CheckBox::Checked(void) { return checked; }

// Another Psydo Event. Will set checked state.
void PS4Forms::CheckBox::ClickEvent(void) {
	if (clicked) {
		if (checked) checked = false;
		else checked = true;
		clicked = false;
	}
}

// Compares the updated cursor postion against the position and size of the form and will set 'isCursorRange' to true, if the stick, cursor is within our form.
bool PS4Forms::CheckBox::UpdateCursor(float x, float y) {
	if (IsObjectFocus(x, y, _position, _size)) focus = true;
	else focus = false;
	return focus;
}

// Get Cursor/stick condition for this form. True means cursor, stick is within the form. False means it is not.
bool PS4Forms::CheckBox::isFocus(void) { return focus; }

// Get actual show state of this Form. True means the Form is showen, false means the form is hiding.
bool PS4Forms::CheckBox::isShowen(void) { return show; }

// Drawing the Form and all his childrens if any.
void PS4Forms::CheckBox::Draw(ssg::GraphicsContext *graphicsContext) {
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


/*------------------------------------------------------------------
                  Form Class
------------------------------------------------------------------*/
// Class Initializer.
PS4Forms::Form::Form() {
	show = focus = closed = isFocused = false;
	visible = true;
	_text = strdup("");
	_position = vecmath::Vector2(0, 0);
	_size = vecmath::Vector2(0, 0);
	_color = DARK_GRAY;
	_textColor = BLACK;
	_borderColor = LIGHT_GRAY;
}

// Class Initializer.
PS4Forms::Form::Form(const char *text, vecmath::Vector2 position, vecmath::Vector2 size, vecmath::Vector4 color, vecmath::Vector4 textcolor) {
	show = focus = closed = isFocused = false;
	visible = true;
	_text      = strdup(text);
	_position  = position;
	_size      = size;
	_color     = color;
	_textColor = textcolor;
	_borderColor = LIGHT_GRAY;
}

// Class Deinitializer.
PS4Forms::Form::~Form() {
	delete _text;

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
}

// Set Titel Color of this form.
void PS4Forms::Form::TextColor(vecmath::Vector4_arg color) { _textColor = color; }

// Get Titel Color of this form.
vecmath::Vector4_arg PS4Forms::Form::getTextColor(void) { return _textColor; }

// Set the Borders Color.
void PS4Forms::Form::BorderColor(vecmath::Vector4_arg color) { _borderColor = color; }

// Get the Borders Color.
vecmath::Vector4_arg PS4Forms::Form::getBorderColor(void) { return _borderColor; }

// Show Form.
void PS4Forms::Form::Show(void) {
	show = true;

	// Set all Childs to Show.
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
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
	} else {
		// Set all Childs to Hide.
		if (forms.size() > 0) {
			for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
				(*it)->Hide();
			}
		}
	}
}

// Drawing the Form and all his childrens if any.
void PS4Forms::Form::Draw(ssg::GraphicsContext *graphicsContext) {
	// Draw the form.
	if (show & visible) {
		_sprite->fillRect(graphicsContext, _position, _size, _color);

		// Shall we draw a border ?
		if (borderVisible) {
			// Draw a border.
			if (!focus) {
				_sprite->drawRect(graphicsContext, _position, _size, WHITE);
				_sprite->fillRect(graphicsContext, vecmath::Vector2(_position.getX(), _position.getY() - 0.04), vecmath::Vector2(0.04, 0.3), WHITE);
			} else {
				_sprite->drawRect(graphicsContext, _position, _size, _borderColor);
				_sprite->fillRect(graphicsContext, vecmath::Vector2(_position.getX(), _position.getY() - 0.04), vecmath::Vector2(0.04, 0.3), _borderColor);
			}
		}
		
		// Draw Title into Border.
		_sprite->drawDebugStringf(graphicsContext, vecmath::Vector2(_position.getX() + 0.01, _position.getY() - 0.035), 0.03, LIGHT_BLUE, _text);

		// Draw all child objects, if any.
		if (forms.size() > 0) {
			for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
				(*it)->Draw(graphicsContext);
			}
		}
	}
}

// Add a Label Object to this Form.
void PS4Forms::Form::Add(Form *child) {
	if (child != nullptr) {
		child->parent = this;
		forms.push_back(child);
	}
}

// Add a Label Object to this Form.
void PS4Forms::Form::Add(WidgetBase *child) {
	if (child != nullptr) {
		child->Parent((WidgetBase *)this);
		widgets.push_back(child);
	}
}

// Remove a TextBox Object from this Form.
void PS4Forms::Form::Remove(Form *child) {
	if (child != nullptr) {
		forms.erase(std::remove(forms.begin(), forms.end(), child), forms.end());
	}
}

// Remove a TextBox Object from this Form.
void PS4Forms::Form::Remove(WidgetBase *child) {
	if (child != nullptr) {
		widgets.erase(std::remove(widgets.begin(), widgets.end(), child), widgets.end());
	}
}