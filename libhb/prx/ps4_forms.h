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
#include <string>
#include <stdint.h>
#include <sampleutil.h>
#include "swiss_knife.h"
#include <stdarg.h>

#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif

using namespace sce::Vectormath::Simd::Aos;
using namespace LibHomebrew;
using namespace LibHomebrew::Loot;

#define DISPLAY_WIDTH	1920
#define DISPLAY_HEIGHT	1080

///////////////////////////////
// Menu configurations
///////////////////////////////

/*#define CONFIG_CLEAR_COLOUR					0x00000000									// The menu background colour (ARGB)
#define CONFIG_ITESELECTED_COLOUR			0x00FF00FF									// Colour of text when highlighted or selected (RGBA)
#define CONFIG_ITEUNSELECTED_COLOUR			0xFF4400FF									// Colour of text when not highlighted (RGBA)
#define CONFIG_ITESELECTED_SCALE			0.035f										// Scale of selected menu item
#define CONFIG_ITEUNSELECTED_SCALE			0.025f										// Scale of unselected menu item
#define CONFIG_ITEX_MIN_CHARACTER_OFFSET	14											// Minimum character spacing
#define CONFIG_ITEX_OFFSET					0.35f * CONFIG_ITEUNSELECTED_SCALE	        // Spacing in between columns
#define CONFIG_ITEY_OFFSET					0.035f										// Spacing in between rows
#define TTY_BACKGROUND_COLOUR_FOCUSED		0x758AFFFF									// The TTY background colour, when selected (RGBA)
#define TTY_BACKGROUND_COLOUR_UNFOCUSED		0x999999FF									// The TTY background colour, when unselected (RGBA)
#define TTY_TEXT_COLOUR						0xFFFFFFFF									// The TTY text colour (RGBA) [White]
#define TTY_TEXT_COLOUR_SELECTION  			0x00000000									// The TTY text colour (RGBA) [Black]
#define TTY_TEXT_COLOUR_WARNING				0xFFFF11FF									// The TTY text colour for a warning (RGBA)
#define TTY_TEXT_COLOUR_ERROR				0xFF0000FF									// The TTY text colour for an error (RGBA)
#define TTY_TEXT_Y_OFFSET					0.04f										// The TTY text scale
#define TTY_TEXT_SCALE						0.03f										// The TTY text scale
#define TTY_MAX_CHARACTERS_PER_LINE			100											// Maximum characters per line
#define TTY_MAX_LINES						1024										// Maximum lines stored in the TTY
#define TTY_SCROLL_BAR_SIZE					0.025f										// Maximum lines stored in the TTY
#define SCREEN_PROPORTION				    0.3f										// Proportion of the screen TTY takes up, from 0.0f to 1.0f*/

namespace PS4Forms {
	enum DialogResult {
		Ok,
		Cancel,
		Yes,
		No,
		Error,
		Button1,
		Button2,
	};

	class ThreadFunc {
	public:
		ThreadFunc() {}
		~ThreadFunc() {}
		void *(*Func)(void *) = 0;
		void Add(off_t *func);
	};

	class ButtonPressed {
	public:
		ssi::Button pressed;

		ButtonPressed() {
			pressed = ssi::Button::kButtonNone;
		}
		~ButtonPressed() {}
	};

	class PRX_INTERFACE Image {
	protected:
		ssg::Texture        *_texture;
		ssg::GraphicsLoader *graphicsLoader;

	public:
		Image() {}
		Image(ssg::GraphicsLoader *loader, const char *path);
		Image(ssg::GraphicsLoader *loader, String &path);
		~Image();
		void setGraphicsLoader(ssg::GraphicsLoader *loader);
		int loadTexture(const String &resourceName);
		int loadTexture(const char *resourceName);
		void unloadTexture(void);
		ssg::Texture *getTexture(void);
	};

	class PRX_INTERFACE WidgetBase {
	protected:
		bool                      show;
		bool                      focus;
		bool                      closed;
		bool                      visible;
		bool                      isFocused;
		bool                      clicked;
		char                      *_id;
		ssg::SpriteRenderer       *_sprite;
		Position                  _position;
		Size                      _size;
		Color                     _color;
		WidgetBase                *parent;
		std::vector<WidgetBase *> widgets;
		std::vector<ThreadFunc *> funcs;
		ScePthread                thread0;
		ScePthread                thread1;
		ScePthread                thread2;
		ScePthread                thread3;
		ScePthread                thread4;
		ScePthread                thread5;
		ScePthread                thread6;

	public:
		WidgetBase();
		WidgetBase(const char *id, Position position, Size size, Color color);
		~WidgetBase();
		void setId(const char *text);
		virtual void setPosition(Position _position);
		virtual void setParentPos(Position parentPos);
		void setColor(Color _color);
		virtual void Show(void);
		virtual void setSize(Size size);
		virtual void Hide(void);
		virtual void Close(void);
		void AddFunc(ThreadFunc *func);
		void AddFunc(off_t *func);
		void RemoveFunc(ThreadFunc *func);
		void RemoveFunc(off_t *func);
		virtual void Visible(bool Visible);
		void XPressed(void);
		virtual void ClickEvent(void);
		void Focused(bool state);
		virtual bool UpdateCursor(float x, float y);
		virtual void SetSprite(ssg::SpriteRenderer *sprite);
		char *getId(void);
		Size getSize(void);
		Position getPosition(void);
		Color getColor(void);
		void setParent(WidgetBase *widget);
		bool isFocus(void);
		bool isShowen(void);
		bool isVisible(void);
		virtual void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite);
		void Add(WidgetBase *child);
		void Remove(WidgetBase *child);
		WidgetBase *getWidgetById(String id);
	};	
	
	class PRX_INTERFACE Label : public WidgetBase {
	protected:		
		char                      *_text;    // Label Text to Draw.
		float                     _size;
		Color                     _textColor;

	public:
		Label();
		Label(const char *id, const char *text, Position position, float size, Color color);
		~Label();
		void setText(const char *text);
		char *getText(void);
		void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) override;
	};

	class PRX_INTERFACE TextBox : public WidgetBase {
	protected:
		char                      *_text;    // Box Text to Draw.
		float                     _textSize;		
		Color                     _textColor;
		Color                     _borderColor;

	public:
		TextBox();
		TextBox(const char *id, const char *text, Position position, Size size, float textSize, Color color, Color textColor);
		~TextBox();
		void setText(const char *text);
		char *getText(void);
		void setTextColor(Color color);
		void setTextize(float size);
		void BorderColor(Color color);
		float getTextSize(void);
		Color getTextColor(void);
		Color getBorderColor(void);
		virtual void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) override;
	};	

	class PRX_INTERFACE PictureBox : public WidgetBase {
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

	protected:
		ssg::Texture              *_texture;
		HAlign                    _hAlign;
		VAlign                    _vAlign;
		FillMode                  _fillMode;
	
	public:
		PictureBox();
		~PictureBox() {}
		void Draw(ssg::GraphicsContext *context, ssg::SpriteRenderer *sprite) override;
		void addImage(ssg::Texture *texture);
		void addImage(Image *image);
		void setHAlign(HAlign hAlign);
		void setVAlign(VAlign vAlign);
		void setFillMode(FillMode fillMode);
	};

	class PRX_INTERFACE TTY {
	public:
		// Color a specific amount of chars within a line.
		struct moreColor {
			uint32_t color;
			uint32_t num;
		};

		struct TtyItem {
			char	               buffer[100];
			bool	               canOverwrite;
			std::vector<moreColor> colorize;

			TtyItem() : canOverwrite(false) {
				memset(buffer, 0x0, 100);
			}
		};

		static uint32_t tty_text_color;				      // The TTY text colour (RGBA) [White]
		static uint32_t tty_text_color_selection;		  // The TTY text colour (RGBA) [Black]
		static uint32_t tty_text_color_warning;		      // The TTY text colour for a warning (RGBA)
		static uint32_t tty_text_color_error;			  // The TTY text colour for an error (RGBA)	

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
		//static void toggleFocus() { _isFocused = !_isFocused; }

		// Is TTY in focus ?
		//static bool isFocused() { return _isFocused; }

		// Get current selected line.
		static char *getSelectedLine(void);

		// Get current selected line indexer.
		static unsigned int getSelectedLineIndex(void);

		// Set current selected line.
		static void setSelectedLine(char *line);

		// Set current selected line indexer.
		static void setSelectedLineIndex(unsigned int line);

		// Set to use Line Highlighting or not.
		static void useLineHighlighting(bool condition);

		// Deletes the last line from the buffer.
		static void deleteLastLine(void);

		// Get the maximum lines.
		static int getLinesCount(void);

		// Reload the maximum lines to draw after a position and/or a font size change.
		static void reloadMaxLinesDrawNum(void);

		// Get the Font Size.
		static float getFontSize(void);

		// Set the Font Size.
		static void setFontSize(float size);

		// Set the Y Coordinate.
		static void setY(float position);

		// Set the X Coordinate.
		static void setX(float position);

		// Get the Y Coordinate.
		static float getY(void);

		// Set the X Coordinate.
		static float getX(void);

		// Clear the TTY
		static void clear();

	private:
		static float tty_text_scale;
		static float config_iteselected_scale;			  // Scale of selected menu item
		static float config_iteunselected_scale;	      // Scale of unselected menu item
		static int config_itex_min_character_offset;	  // Minimum character spacing
		static float config_itex_offset;                  // Spacing in between columns
		static float config_itey_offset;				  // Spacing in between rows			
		static int tty_max_charachters_per_line;		  // Maximum characters per line
		static int tty_max_lines;						  // Maximum lines stored in the TTY
		static float tty_scroll_bar_size;				  // Maximum lines stored in the TTY
		static float screen_proportion;				      // Proportion of the screen TTY takes up, from 0.0f to 1.0f

		// Buffer cotaining all TTY
		static TtyItem buffer[1000];

		// Index in TTY array that we can write to next. Wraps around when TTY_MAX_LINES is reached
		static uint32_t writePos;

		// Index in TTY that should be drawn first
		static int32_t HeadDrawPos;

		// Total number of lines written to TTY
		static uint32_t totalLinesWritten;

		// Current Selected Line.
		static uint32_t selectedLineIndex;

		// Maximum amount of lines that can be drawn on screen
		static int32_t maxLinesDrawNum;

		// Is the TTY in focus ?
		//static bool _isFocused;

		// Vector Color of the Selected text.
		static Color selection;

		// RGBA Color of the selected text.
		static uint32_t black;

		// The size of the selection bar to draw.
		static Size selectionSize;

		// Determine to use line highlighting or not.
		static bool _useLineHighlighting;

		// X Position Coordinate for the TTY output.
		static float xcoordinate;

		// Y Position Coordinate for the TTY output.
		static float ycoordinate;

		// Font size.
		static float fontSize;
	};

	class PRX_INTERFACE TTYD {
	public:
		// Color a specific amount of chars within a line.
		struct moreColor {
			uint32_t color;
			uint32_t num;
		};

		struct TtyItem {
			char	               buffer[100];
			bool	               canOverwrite;
			std::vector<moreColor> colorize;

			TtyItem() : canOverwrite(false) {
				memset(buffer, 0x0, 100);
			}
		};		

		TTYD();
		~TTYD();
		void onScreenPrintf(const char *text, ...);

		// Adds text to the buffer with a certain colour
		void onScreenPrintf(const uint32_t &colour, const char *text, ...);

		void update(sce::SampleUtil::Input::PadContext * controller);

		// Renders strings to screen
		void render(sce::SampleUtil::Graphics::SpriteRenderer * renderer, sce::SampleUtil::Graphics::GraphicsContext * context);

		// Toggles TTY focus
		//void toggleFocus() { _isFocused = !_isFocused; }

		// Is TTY in focus ?
		//bool isFocused() { return _isFocused; }
		char *getSelectedLine(void);
		unsigned int getSelectedLineIndex(void);
		void setSelectedLine(char *line);
		void setSelectedLineIndex(unsigned int line);
		void useLineHighlighting(bool condition);
		void deleteLastLine(void);
		int getLinesCount(void);
		void reloadMaxLinesDrawNum(void);
		void DoScrollWithNewLine(bool state);
		float getFontSize(void);
		void setFontSize(float size);
		void TextColor(uint32_t color);
		uint32_t getTextColor(void);

		// Set the Y Coordinate.
		void setY(float position);

		// Set the X Coordinate.
		void setX(float position);

		// Get the Y Coordinate.
		float getY(void);

		// Set the X Coordinate.
		float getX(void);

		// Clear the TTY
		void clear();

	private:
		// The TTY text scale
		float tty_text_scale;
		float config_iteselected_scale;			  // Scale of selected menu item
		float config_iteunselected_scale;	      // Scale of unselected menu item
		int config_itex_min_character_offset;	  // Minimum character spacing
		float config_itex_offset;                 // Spacing in between columns
		float config_itey_offset;				  // Spacing in between rows
		uint32_t tty_text_color;				  // The TTY text colour (RGBA) [White]
		uint32_t tty_text_color_selection;		  // The TTY text colour (RGBA) [Black]
		uint32_t tty_text_color_warning;		  // The TTY text colour for a warning (RGBA)
		uint32_t tty_text_color_error;			  // The TTY text colour for an error (RGBA)		
		int tty_max_charachters_per_line;		  // Maximum characters per line
		int tty_max_lines;						  // Maximum lines stored in the TTY
		float tty_scroll_bar_size;				  // Maximum lines stored in the TTY
		float screen_proportion;				  // Proportion of the screen TTY takes up, from 0.0f to 1.0f

		// Buffer cotaining all TTY
		std::vector<TtyItem> buffer;

		// Index in TTY array that we can write to next. Wraps around when TTY_MAX_LINES is reached
		uint32_t writePos;

		// Index in TTY that should be drawn first
		int32_t HeadDrawPos;

		// Total number of lines written to TTY
		uint32_t totalLinesWritten;

		// Current Selected Line.
		uint32_t selectedLineIndex;

		// Maximum amount of lines that can be drawn on screen
		int32_t maxLinesDrawNum;

		// Is the TTY in focus ?
		//bool _isFocused;

		// Vector Color of the Selected text.
		Color selection;

		// RGBA Color of the selected text.
		uint32_t black;

		// The size of the selection bar to draw.
		Size selectionSize;

		// Determine to use line highlighting or not.
		bool _useLineHighlighting;

		// X Position Coordinate for the TTY output.
		float xcoordinate;

		// Y Position Coordinate for the TTY output.
		float ycoordinate;

		// Determines if the 'HeadDrawPosition' should be counted up when there is a new line. (Scrolls down till last maximum on screen lines)
		bool scrollWithNewLine;
	};

	class PRX_INTERFACE RichTextBox : public WidgetBase {
	protected:		
		TTYD                      *ttyd;

	public:
		RichTextBox();
		~RichTextBox();
		void setBackGroundColor(Color _color);
		Color getBackGroundColor(void);
		void setPosition(Position _position) override;
		void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) override;
		void Update(sce::SampleUtil::Input::PadContext *controller);
		void Write(const char *format, ...);
		void WriteLine(const char *format, ...);
		void LineBreak(void);
		char *getSelectedLine(void);
		unsigned int getSelectedLineIndex(void);
		void setSelectedLine(char *line);
		void setSelectedLineIndex(unsigned int line);
		void useLineHighlighting(bool condition);
		void deleteLastLine(void);
		int GetLinesCount(void);
		void reloadMaxLinesDrawNum(void);
		float getFontSize(void);
		void setFontSize(float size);
		void TextColor(uint32_t color);
		uint32_t getTextColor(void);
		void setY(float position);
		void setX(float position);
		float getY(void);
		float getX(void);
		void Clear(void);
	};

	class PRX_INTERFACE Button : public WidgetBase {
	protected:		
		char                      *_text;
		float                     charsize;		
		Color                     _textColor;
		Color                     _borderColor;		

	public:
		PictureBox                *img;

		Button();
		Button(const char *id, const char *text, float charsize, Position position, Size size, Color color, Color textcolor);
		~Button();
		void setText(const char *text);
		char *getText(void);
		void TextColor(Color color);
		void BorderColor(Color color);
		Color getTextColor(void);
		Color getBorderColor(void);
		void setPosition(Position position) override;
		void setSize(Size size) override;
		void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) override;
	};

	class PRX_INTERFACE ScrollBar : public WidgetBase {
	protected:		
		float               x;
		float               y;
		int32_t             head;                  // Index in TTY that should be drawn first.	
		uint32_t            linesWritten;          // Total number of lines written to TTY.
		int32_t             maxLines;              // Maximum amount of lines that can be drawn on screen.		
		vecmath::Vector2    _position;
		vecmath::Vector2    _size;
		vecmath::Vector2    _size0;
		vecmath::Vector4    _color;
		vecmath::Vector4    _borderColor;
		float               tty_scroll_bar_size;			  // Maximum lines stored in the TTY
		float               screen_proportion;				  // Proportion of the screen TTY takes up, from 0.0f to 1.0f

	public:
		ScrollBar();
		ScrollBar(Position position, Color color);
		~ScrollBar() {}
			void InitX(float x);
		void Head(int32_t num);
		void LinesWritten(uint32_t num);
		void MaxLines(int32_t num);
		void BorderColor(Color color);
		Color getBorderColor(void);
		int32_t getHead(void);
		uint32_t getLinesWritten(void);
		int32_t getMaxLines(void);
			void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) override;
	};

	class PRX_INTERFACE CheckBox : public WidgetBase {
	protected:		
		char                *_text;
		bool                checked;
		Color               _textColor;
		Color               _borderColor;

	public:
		CheckBox();
		CheckBox(const char *id, const char *text, Position position, Color color, Color textcolor);
		~CheckBox();
		void setText(const char *text);
		char *getText(void);
		void TextColor(Color color);
		void BorderColor(Color color);
		Color getTextColor(void);
		Color getBorderColor(void);
		void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) override;
	};

	class PRX_INTERFACE Form : public WidgetBase {
	protected:
		bool                      visibleBoarder;
		bool                      useBgColor;
		char                      *_text;             // Border Text.
		Form                      *parent;
		Color                     _textColor;
		Color                     _borderColor;
		std::vector<Form *>       forms;
		std::vector<PictureBox *> pictures;
		std::vector<Button *>     button;
		std::vector<ScrollBar *>  sbar;
		std::vector<CheckBox *>   checkB;
		std::vector<Label *>      labels;
		std::vector<TextBox *>    textb;
		std::vector<RichTextBox*> rtbs;
		std::vector<void (*)()>   drawFuncs;

	public:
		Form();
		Form(const char *id, const char *text, Position position, Size size, Color color, Color textcolor);
		~Form();
		void setPosition(Position _position) override;
		void setParentPos(Position parentPos)  override;
		void setText(const char *text);
		char *getText(void);
		virtual void TextColor(Color color);
		void setColor(Color color);
		void Show(void) override;
		void ShowForm(void);
		void Hide(void) override;
		void Close(void) override;
		void Visible(bool Visible) override;
		void BorderVisible(bool state);
		void BorderColor(Color color);
		void UseBgColor(bool state);
		virtual Color getTextColor(void);
		Color getBorderColor(void);
		bool UpdateCursor(float x, float y) override;
		void SetSprite(ssg::SpriteRenderer *sprite) override;
		virtual void Update(sce::SampleUtil::Input::PadContext *controller);
		virtual void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite)  override;
		void Add(void (*drawEvent)());
		void Add(Form *child);
		void Add(WidgetBase *child);
		void Add(PictureBox *child);
		void Add(Label *child);
		void Add(Button *child);
		void Add(ScrollBar *child);
		void Add(CheckBox *child);
		void Add(TextBox *child);
		void Add(RichTextBox *child);
		void Remove(void (*drawEvent)());
		void Remove(Form *child);
		void Remove(WidgetBase *child);
		void Remove(PictureBox *child);
		void Remove(Label *child);
		void Remove(Button *child);
		void Remove(ScrollBar *child);
		void Remove(CheckBox *child);
		void Remove(TextBox *child);
		void Remove(RichTextBox *child);
		WidgetBase  *getWidgetById(String id);
		Form        *getFormById(String id);
		Label       *getLabelById(String id);
		Button      *getButtonById(String id);
		ScrollBar   *getScrollBarById(String id);
		CheckBox    *getCheckBoxById(String id);
		TextBox     *getTextBoxById(String id);
		PictureBox  *getPictureBoxById(String id);
		RichTextBox *getRtbById(String id);
	};

	class PRX_INTERFACE TextViewer : public Form {
	protected:
		char                      *buffer;
		char                      _loading[11] = { 'l', 'o', 'a', 'd', 'i', 'n', 'g', '.', '\0', };
		bool                      loading;

	public:
		RichTextBox               *rtb;

		TextViewer();
		~TextViewer();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-virtual"
		void TextColor(uint32_t color);
		void Update(sce::SampleUtil::Input::PadContext *controller) override;
#pragma clang diagnostic pop
		Color getTextColor(void) override;
		virtual void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) override;
		int LoadFile(const char *path);
	};

	class PRX_INTERFACE HexViewer : public TextViewer {
	protected:
		byte                  *buffer;

	public:
		HexViewer();
		~HexViewer();
		void Draw(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *sprite) override;
		int LoadFile(const char *path);
	};
}