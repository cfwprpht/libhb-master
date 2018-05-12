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

#include "defines.h"

#define SCROLL_BAR_SIZE					0.025f										// Maximum lines stored in the TTY
#define SCREEN_PROPORTION				0.3f										// Proportion of the screen TTY takes up, from 0.0f to 1.0f

namespace PS4Forms {
	static bool IsObjectFocus(float x, float y, vecmath::Vector2_arg _position, vecmath::Vector2_arg _size);

	class ButtonPressed {
	public:
		ssi::Button pressed;

		ButtonPressed() {
			pressed = ssi::Button::kButtonNone;
		}
		~ButtonPressed() {}
	};

	class ThreadFunc {
	public:
		ThreadFunc() {}
		~ThreadFunc() {}
		void *(*Func)(void *) = 0;
		void Add(off_t *func);
	};

	class WidgetBase {
	private:
		bool                      show;
		bool                      focus;
		bool                      closed;
		bool                      visible;
		bool                      isFocused;
		bool                      clicked;
		char                      *_text;
		ssg::SpriteRenderer       *_sprite;
		vecmath::Vector2          _position;
		vecmath::Vector2          _size;
		vecmath::Vector4          _color;
		WidgetBase                *parent;
		std::vector<ThreadFunc *> funcs;
		ScePthread                thread0;
		ScePthread                thread1;
		ScePthread                thread2;
		ScePthread                thread3;
		ScePthread                thread4;
		ScePthread                thread5;
		ScePthread                thread6;

	public:
		void Text(const char *text);
		void Position(vecmath::Vector2_arg);
		void Color(vecmath::Vector4_arg);
		virtual void Show(void);
		void Size(vecmath::Vector2_arg size);
		virtual void Hide(void);
		virtual void Close(void);
		void AddFunc(ThreadFunc *func);
		void AddFunc(off_t *func);
		void RemoveFunc(ThreadFunc *func);
		void RemoveFunc(off_t *func);
		virtual void Visible(bool Visible);
		void XPressed(void);
		void ClickEvent(void);
		void Focused(bool state);
		virtual bool UpdateCursor(float x, float y);
		virtual void SetSprite(ssg::SpriteRenderer *sprite);
		char *getText(void);
		vecmath::Vector2_arg getSize(void);
		vecmath::Vector2_arg getPosition(void);
		vecmath::Vector4_arg getColor(void);
		void Parent(WidgetBase *);
		bool isFocus(void);
		bool isShowen(void);
		bool isVisible(void);
		virtual void Draw(ssg::GraphicsContext *graphicsContext);
	};

	class Form : WidgetBase {
	private:
		bool                      show;
		bool                      focus;
		bool                      closed;
		bool                      visible;
		bool                      isFocused;
		bool                      borderVisible;
		char                      *_text;
		Form                      *parent;
		ssg::SpriteRenderer       *_sprite;
		vecmath::Vector2          _position;
		vecmath::Vector2          _size;
		vecmath::Vector4          _color;
		vecmath::Vector4          _textColor;
		vecmath::Vector4          _borderColor;
		std::vector<Form *>       forms;
		std::vector<WidgetBase *> widgets;
		

	public:
		Form();
		Form(const char *text, vecmath::Vector2 position, vecmath::Vector2 size, vecmath::Vector4 color, vecmath::Vector4 textcolor);
		~Form();
		void TextColor(vecmath::Vector4_arg color);
		virtual void Show(void);
		virtual void ShowForm(void);
		virtual void Hide(void);
		virtual void Close(void);
		virtual void Visible(bool Visible);
		virtual void SetSprite(ssg::SpriteRenderer *sprite);
		void BorderVisible(bool state);
		void BorderColor(vecmath::Vector4_arg color);
		vecmath::Vector4_arg getTextColor(void);
		vecmath::Vector4_arg getBorderColor(void);
		virtual bool UpdateCursor(float x, float y);
		bool isBorderVisible(void);
		virtual void Draw(ssg::GraphicsContext *graphicsContext);
		void Add(Form *child);
		void Add(WidgetBase *child);
		void Remove(Form *child);
		void Remove(WidgetBase *child);
	};

	class Label {
	private:
		bool                      show;
		bool                      focus;
		bool                      visible;
		bool                      clicked;
		bool                      isFocused;
		char                      *_text;
		float                     _size;
		ssg::SpriteRenderer       *_sprite;
		vecmath::Vector2          _position;
		vecmath::Vector4          _color;
		std::vector<ThreadFunc *> funcs;
		ScePthread                thread0;
		ScePthread                thread1;
		ScePthread                thread2;
		ScePthread                thread3;
		ScePthread                thread4;
		ScePthread                thread5;
		ScePthread                thread6;

	public:
		Label();
		Label(const char *text, vecmath::Vector2 position, float size, vecmath::Vector4 color);
		~Label();
		void Text(const char *text);
		void Position(vecmath::Vector2_arg);
		void Color(vecmath::Vector4_arg);
		void Show(void);
		void Size(float size);
		void Hide(void);
		void Close(void);
		void AddFunc(ThreadFunc *func);
		void AddFunc(off_t *func);
		void RemoveFunc(ThreadFunc *func);
		void RemoveFunc(off_t *func);
		void Visible(bool Visible);
		void XPressed(void);
		void ClickEvent(void);
		void Focused(bool state);
		void SetSprite(ssg::SpriteRenderer *sprite);
		char *getText(void);
		float getSize(void);
		vecmath::Vector2_arg getPosition(void);
		vecmath::Vector4_arg getColor(void);
		bool UpdateCursor(float x, float y);
		bool isFocus(void);
		bool isShowen(void);
		bool isVisible(void);
		void Draw(ssg::GraphicsContext *graphicsContext);
	};

	class TextBox {
	private:
		bool                      show;
		bool                      focus;
		bool                      visible;
		bool                      clicked;
		bool                      isFocused;
		char                      *_text;
		float                     _textSize;
		ssg::SpriteRenderer       *_sprite;
		vecmath::Vector2          _position;
		vecmath::Vector2          _size;
		vecmath::Vector4          _color;
		vecmath::Vector4          _textColor;
		vecmath::Vector4          _borderColor;
		std::vector<ThreadFunc *> funcs;
		ScePthread                thread0;
		ScePthread                thread1;
		ScePthread                thread2;
		ScePthread                thread3;
		ScePthread                thread4;
		ScePthread                thread5;
		ScePthread                thread6;

	public:
		TextBox();
		TextBox(const char *text, vecmath::Vector2 position, vecmath::Vector2 size, float textSize, vecmath::Vector4 color, vecmath::Vector4 textColor);
		~TextBox();
		void Text(const char *text);
		void Position(vecmath::Vector2_arg);
		void Color(vecmath::Vector4_arg);
		void TextColor(vecmath::Vector4_arg color);
		void Show(void);		
		void Size(vecmath::Vector2_arg size);
		void Textize(float size);
		void Hide(void);
		void Close(void);
		void AddFunc(ThreadFunc *func);
		void AddFunc(off_t *func);
		void RemoveFunc(ThreadFunc *func);
		void RemoveFunc(off_t *func);
		void Visible(bool Visible);
		void XPressed(void);
		void ClickEvent(void);
		void Focused(bool state);
		void BorderColor(vecmath::Vector4_arg color);
		void SetSprite(ssg::SpriteRenderer *sprite);
		char *getText(void);
		float getTextSize(void);
		vecmath::Vector2_arg getSize(void);
		vecmath::Vector2_arg getPosition(void);
		vecmath::Vector4_arg getColor(void);
		vecmath::Vector4_arg getTextColor(void);
		vecmath::Vector4_arg getBorderColor(void);
		bool UpdateCursor(float x, float y);
		bool isFocus(void);
		bool isShowen(void);
		bool isVisible(void);
		void Draw(ssg::GraphicsContext *graphicsContext);
	};	

	class Image {
	public:
		enum HAlign {
			kHAlignLeft,
			kHAlignRight,
			kHAlignCenter
		};
		enum VAlign {
			kVAlignTop,
			kVAlignBottom,
			kVAlignCenter
		};
		enum FillMode {
			kFillModeDotByDot,
			kFillModeExpand,
			kFillModeVExpand,
			kFillModeHExpand
		};

	private:
		bool                      show;
		bool                      focus;
		bool                      visible;
		bool                      clicked;
		bool                      isFocused;
		vecmath::Vector2          _position;
		vecmath::Vector2          _size;
		ssg::Texture              *_texture;
		ssg::SpriteRenderer       *_sprite;
		HAlign                    _hAlign;
		VAlign                    _vAlign;
		FillMode                  _fillMode;
	
	public:
		Image();
		~Image() {}
		void Position(vecmath::Vector2_arg);
		void Show(void);
		void Size(vecmath::Vector2_arg size);
		void Hide(void);
		void Close(void);
		void Visible(bool Visible);
		void Focused(bool state);
		vecmath::Vector2_arg getSize(void);
		vecmath::Vector2_arg getPosition(void);
		bool UpdateCursor(float x, float y);
		bool isFocus(void);
		bool isShowen(void);
		bool isVisible(void);
		void draw(sce::SampleUtil::Graphics::GraphicsContext *context);
		void SetSprite(ssg::SpriteRenderer *sprite);
		void setTexture(sce::SampleUtil::Graphics::Texture *texture);
		void setHAlign(HAlign hAlign);
		void setVAlign(VAlign vAlign);
		void setFillMode(FillMode fillMode);	
	};

	class PictureBox {
	private:


	public:

	};

	class RichTextBox {
	private:


	public:

	};		

	class Button {
	private:
		bool                      show;
		bool                      focus;
		bool                      visible;
		bool                      clicked;
		bool                      isFocused;
		char                      *_text;
		float                     charsize;
		ssg::SpriteRenderer       *_sprite;
		vecmath::Vector2          _position;
		vecmath::Vector2          _size;
		vecmath::Vector4          _color;
		vecmath::Vector4          _textColor;
		vecmath::Vector4          _borderColor;
		std::vector<ThreadFunc *> funcs;
		ScePthread                thread0;
		ScePthread                thread1;
		ScePthread                thread2;
		ScePthread                thread3;
		ScePthread                thread4;
		ScePthread                thread5;
		ScePthread                thread6;

	public:
		Button();
		Button(const char *text, float charsize, vecmath::Vector2 position, vecmath::Vector2 size, vecmath::Vector4 color, vecmath::Vector4 textcolor);
		~Button();
		void Text(const char *text);
		void Position(vecmath::Vector2_arg position);
		void Size(vecmath::Vector2_arg size);
		void Color(vecmath::Vector4_arg color);
		void TextColor(vecmath::Vector4_arg color);
		void Show(void);
		void Hide(void);
		void Close(void);
		void AddFunc(ThreadFunc *func);
		void AddFunc(off_t *func);
		void RemoveFunc(ThreadFunc *func);
		void RemoveFunc(off_t *func);
		void Visible(bool Visible);
		void XPressed(void);
		void ClickEvent(void);
		void Focused(bool state);
		void BorderColor(vecmath::Vector4_arg color);
		void SetSprite(ssg::SpriteRenderer *sprite);
		char *getText(void);		
		vecmath::Vector2_arg getSize(void);
		vecmath::Vector2_arg getPosition(void);
		vecmath::Vector4_arg getColor(void);
		vecmath::Vector4_arg getTextColor(void);
		vecmath::Vector4_arg getBorderColor(void);
		bool UpdateCursor(float x, float y);
		bool isFocus(void);
		bool isShowen(void);
		bool isVisible(void);
		void Draw(ssg::GraphicsContext *graphicsContext);
	};

	class ScrollBar {
	private:
		bool                show;
		bool                focus;
		bool                visible;
		bool                isFocused;
		float               x;
		float               y;
		int32_t             head;                  // Index in TTY that should be drawn first.	
		uint32_t            linesWritten;          // Total number of lines written to TTY.
		int32_t             maxLines;              // Maximum amount of lines that can be drawn on screen.
		ssg::SpriteRenderer *_sprite;
		vecmath::Vector2    _position;
		vecmath::Vector2    _size;
		vecmath::Vector2    _size0;
		vecmath::Vector4    _color;
		vecmath::Vector4    _borderColor;

	public:
		ScrollBar();
		ScrollBar(vecmath::Vector2_arg position, vecmath::Vector4 color);
		~ScrollBar() {}
		void Position(vecmath::Vector2_arg position);
		void Color(vecmath::Vector4_arg color);
		void Show(void);
		void Size(vecmath::Vector2_arg size);
		void Hide(void);
		void Close(void);
		void Visible(bool Visible);
		void Focus(bool focus);
		void InitX(float x);
		void Head(int32_t num);
		void LinesWritten(uint32_t num);
		void MaxLines(int32_t num);
		void BorderColor(vecmath::Vector4_arg color);
		void SetSprite(ssg::SpriteRenderer *sprite);
		vecmath::Vector2_arg getSize(void);
		vecmath::Vector2_arg getPosition(void);
		vecmath::Vector4_arg getColor(void);
		vecmath::Vector4_arg getBorderColor(void);
		int32_t getHead(void);
		uint32_t getLinesWritten(void);
		int32_t getMaxLines(void);
		bool isFocus(void);
		bool isShowen(void);
		bool isVisible(void);
		void Draw(ssg::GraphicsContext *graphicsContext);
	};

	class CheckBox {
	private:
		bool                show;
		bool                focus;
		bool                checked;
		bool                visible;
		bool                clicked;
		bool                isFocused;
		char                *_text;
		ssg::SpriteRenderer *_sprite;
		vecmath::Vector4    _textColor;
		vecmath::Vector2    _position;
		vecmath::Vector2    _size;
		vecmath::Vector4    _color;
		vecmath::Vector4    _borderColor;

	public:
		CheckBox();
		CheckBox(const char *text, vecmath::Vector2 position, vecmath::Vector4 color, vecmath::Vector4 textcolor);
		~CheckBox();
		void Text(const char *text);
		void Position(vecmath::Vector2_arg position);
		void Color(vecmath::Vector4_arg color);
		void TextColor(vecmath::Vector4_arg color);
		void Show(void);
		void Hide(void);
		void Close(void);
		void Visible(bool Visible);
		void XPressed(void);
		bool Checked(void);
		void Focused(bool state);
		void ClickEvent(void);
		void BorderColor(vecmath::Vector4_arg color);
		void SetSprite(ssg::SpriteRenderer *sprite);
		char *getText(void);
		vecmath::Vector2_arg getPosition(void);
		vecmath::Vector4_arg getColor(void);
		vecmath::Vector4_arg getTextColor(void);
		vecmath::Vector4_arg getBorderColor(void);
		bool UpdateCursor(float x, float y);
		bool isFocus(void);
		bool isShowen(void);
		bool isVisible(void);
		void Draw(ssg::GraphicsContext *graphicsContext);
	};
}