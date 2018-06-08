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

#include "application.h"
#include <algorithm>

// AV Player.
common::Util::AvPlayer player;
ssi::Button LibHomebrew::Application::input = ssi::kButtonNone;

// Resources
ResourceManager resManager;
Config LibHomebrew::Application::conf;

// Console informations.
static char *sandBoxDirectory;
static char messageBuffer[1045];
static char titleBuffer[64];
static char timeBuffer[16];
static char timeBuffer2[16];
static bool verbose   = false;
static bool isPlaying = false;

// Static Application variables.
char *LibHomebrew::Application::title;
char *LibHomebrew::Application::banner;
char *LibHomebrew::Application::videoPath;
bool LibHomebrew::Application::useTime       = false;
bool LibHomebrew::Application::multiLine     = false;
bool LibHomebrew::Application::useScreenShot = false;
bool LibHomebrew::Application::useCursor     = false;
bool LibHomebrew::Application::useVid        = false;
bool LibHomebrew::Application::useTitle      = false;
bool LibHomebrew::Application::useBanner     = false;
Position LibHomebrew::Application::titlePos  = Position(0.0, 0.0);
Position LibHomebrew::Application::timePos   = Position(0.0, 0.0);
Position LibHomebrew::Application::bannerPos = Position(0.0, 0.0);
Position LibHomebrew::Application::shaderPos = Position(0.0, 0.0);
Color LibHomebrew::Application::titleColor   = LIGHT_BLUE;;
Color LibHomebrew::Application::timeColor    = RED2;
Color LibHomebrew::Application::bannerColor  = YELLOW;
float LibHomebrew::Application::titleSize    = CHAR_N;
float LibHomebrew::Application::timeSize     = CHAR_S;
float LibHomebrew::Application::bannerSize   = 0.08;

// Effect fun.
float posX;
float posY;
float _posX;
float _posY;
static int count;
int bannerCount;
int pauseCount;
int blinked;
static int interval       = 80;
static int intervalShowen = 3;
static bool bannerEffect  = true;

size_t sceLibcHeapSize = 256 * 1024 * 1024;

// The user Main Entry.
void *(*UsrEntry)(void*) = 0;

// The Thread for the User main Entry.
ScePthread usrMainEntry;
void RunUserEntry(void) {
	if (scePthreadCreate(&usrMainEntry, NULL, UsrEntry, NULL, "User Entry") != SCE_OK) {
		if (verbose) Console::WriteError("Couldn't run User Application Code !\n");
	}
}

int LibHomebrew::Application::UserInfo::initialize() {
	//pad open
	int ret = createPadContext(&padContext, userId, 0, 0, NULL);
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);
	return ret;
}

int LibHomebrew::Application::UserInfo::finalize() {
	//pad close
	if (padContext != NULL) {
		sce::SampleUtil::destroy(padContext);
	}
	return SCE_OK;
}

int LibHomebrew::Application::UserInfo::update(ssg::GraphicsContext *graphicsContext) {
	if (padContext->update() >= SCE_OK) {
		// Update Left Stick.
		sce::Vectormath::Simd::Aos::Vector2 leftStick = padContext->getLeftStick();
		posY += leftStick.getY() * 0.01;
		posX += leftStick.getX() * 0.005;

		// Compare new position against the old one and count up if they are same, aka stick didn't move.
		// Cursor and button navi effect.
		if (posY == _posY) {
			if (posX == _posX) {
				if (count != 300) count++;
			} else count = 0;
		} else count = 0;
		_posY = posY;
		_posX = posX;

		// Are we out of range ?
		if (posX < 0.0) posX = 0.0;
		if (posX > 1.0) posX = 1.0;
		if (posY < 0.0) posY = 0.0;
		if (posY > 1.0) posY = 1.0;

		// Let TTY update his buttons.
		if (multiLine) TTY::update(padContext);

		// Update Clock.
		if (useTime) {
			memset(timeBuffer, 0, sizeof(timeBuffer));
			memset(timeBuffer2, 0, sizeof(timeBuffer2));
			time_t t;
			Time *tm;
			time(&t);
			tm = localtime(&t);
			strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", tm);
			strftime(timeBuffer2, sizeof(timeBuffer2), "%H_%M_%S", tm);
		}

		// Update All Forms and objects.
		/* Not implemented yet */

		// Do Buttons.
		if (padContext->isButtonDown(ssi::kButtonUp, ssi::kButtonEventPatternAny)) {

		} else if (padContext->isButtonDown(ssi::kButtonDown, ssi::kButtonEventPatternAny)) {

		} else if (padContext->isButtonDown(ssi::kButtonLeft, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonLeft;
			Console::SetUserInput(ssi::kButtonLeft);
		} else if (padContext->isButtonDown(ssi::kButtonRight, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonRight;
			Console::SetUserInput(ssi::kButtonRight);
		} else if (padContext->isButtonPressed(ssi::kButtonSquare, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonSquare;
			Console::SetUserInput(ssi::kButtonSquare);
		} else if (padContext->isButtonPressed(ssi::kButtonTriangle, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonTriangle;
			Console::SetUserInput(ssi::kButtonTriangle);
			if (useScreenShot) {
				String out = SwissKnife::GetUsb();
				String time(timeBuffer2);
				out += "screenshot_";
				out += time;
				out += ".bmp";
				graphicsContext->saveBackBufferAsBmp(out.c_str());
				FILE *fd = fopen(out.c_str(), "r");
				if (fd) Console::WriteLine("Screen Shot saved to usb.\n");
				else Console::WriteError("Couldn't save Screen Shot to usb.\n");
			}
		} else if (padContext->isButtonPressed(ssi::kButtonCircle, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonCircle;
			Console::SetUserInput(ssi::kButtonCircle);
		} else if (padContext->isButtonPressed(ssi::kButtonCross, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonCross;
			Console::SetUserInput(ssi::kButtonCross);
			if (useVid & isPlaying) {
				if (player.isPlaying()) {
					player.stop();
					isPlaying = false;
				}
			}
		} else if (padContext->isButtonPressed(ssi::kButtonR1, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonR1;
			Console::SetUserInput(ssi::kButtonR1);
		} else if (padContext->isButtonPressed(ssi::kButtonR2, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonR2;
			Console::SetUserInput(ssi::kButtonR2);
		} else if (padContext->isButtonPressed(ssi::kButtonR3, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonR3;
			Console::SetUserInput(ssi::kButtonR3);
			TTY::clear();
		} else if (padContext->isButtonPressed(ssi::kButtonL1, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonL1;
			Console::SetUserInput(ssi::kButtonL1);
		} else if (padContext->isButtonPressed(ssi::kButtonL2, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonL2;
			Console::SetUserInput(ssi::kButtonL2);
		} else if (padContext->isButtonPressed(ssi::kButtonL3, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonL3;
			Console::SetUserInput(ssi::kButtonL3);
		} else if (padContext->isButtonPressed(ssi::kButtonTouchPad, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonTouchPad;
			Console::SetUserInput(ssi::kButtonTouchPad);
		} else if (padContext->isButtonPressed(ssi::kButtonOptions, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonOptions;
			Console::SetUserInput(ssi::kButtonOptions);
		} else {
			input = ssi::kButtonNone;
			Console::SetUserInput(ssi::kButtonNone);
		}		
	}
	return SCE_OK;
}

int LibHomebrew::Application::UserInfo::setColor(SceUserServiceUserColor _userColor) {
	//E This sample uses Web colors as color definition.
	//J このサンプルでは Web カラーの定義を使います。
	switch (_userColor) {
	case SCE_USER_SERVICE_USER_COLOR_BLUE:
		userColor = vecmath::Vector4(0.f, 0.f, 1.f, 1.f); break;
	case SCE_USER_SERVICE_USER_COLOR_RED:
		userColor = vecmath::Vector4(1.f, 0.f, 0.f, 1.f); break;
	case SCE_USER_SERVICE_USER_COLOR_GREEN:
		userColor = vecmath::Vector4(0.f, 0.5f, 0.f, 1.f); break;
	case SCE_USER_SERVICE_USER_COLOR_PINK:
		userColor = vecmath::Vector4(1.f, 192.f / 255.f, 203.f / 255.f, 1.f); break;
	}
	return SCE_OK;
}

void LibHomebrew::Application::UserInfo::render(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *spriteRenderer) {
	if (useCursor) {
		if (count != 200) {
			static const float objectWith = 0.005;
			static const float objectHeight = 0.005;

			spriteRenderer->fillRect(graphicsContext,
				vecmath::Vector2(posX - objectWith / 2.0, posY - objectHeight / 2.0),
				vecmath::Vector2(objectWith, objectHeight),
				userColor);
		}
	}
}

int LibHomebrew::Application::initialize(void) {
	int ret = 0;
	(void)ret;

	//E Initialize SampleUtil. Graphics feature and SpriteRenderer feature of SampleUtil are enabled.
	//J SampleUtilの初期化。SampleUtilのGraphics機能とSpriteRenderer機能を有効化。
	ret = initializeUtil(kFunctionFlagGraphics | kFunctionFlagSpriteRenderer | kFunctionFlagFios2 | kFunctionFlagAudio);
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	//E Initialize UserService.
	//J UserService の初期化。
	ret = sceUserServiceInitialize(NULL);
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	// Create a internal Renderer.
	sprite = NULL;
	ssg::GraphicsContext *graphicsContext = getGraphicsContext();
	ssg::createSpriteRenderer(&sprite, graphicsContext);
	uint32_t width = graphicsContext->getNextRenderTarget()->getWidth();
	uint32_t height = graphicsContext->getNextRenderTarget()->getHeight();
	sprite->setRenderTargetSize(Size(width, height));

	// Initialize Resource Manager.
	conf.useArchive = false;
	resManager.verbose = true;
	resManager.initialize(Graphics(), sprite, &conf, DISPLAY_WIDTH, DISPLAY_HEIGHT);

	// Initialize AV Player.
	ret = player.initialize(Graphics(), sprite, getAudioContext(), &resManager.directMemoryHeap);
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	// Clear Buffer before using.
	memset(messageBuffer, 0, sizeof(messageBuffer));
	sandBoxDirectory = new char[64];
	memset(sandBoxDirectory, 0, 64);
	memset(titleBuffer, 0, sizeof(titleBuffer));
	memset(timeBuffer, 0, sizeof(timeBuffer));
	memset(timeBuffer2, 0, sizeof(timeBuffer2));

	// Generate Titel string with version.
	if (useTitle) {
		snprintf(titleBuffer, sizeof(titleBuffer), "********* %s *********", title);
		titlePos = Position(getCenteredPosX(strlen(titleBuffer)), MARGIN_Y);
	}

	// Set start and close flags.
	start = true;
	close = false;

	// Get Freedom for this Process.
	int uid = Sys::getuid();
	if (uid != 0) Proc::Freedom();

	// Resolve Current Directory.


	// Clear SplashScreen.
	sceSystemServiceHideSplashScreen();

	return SCE_OK;
}

//E Update the application logic. Call updateUtil() at the beginning.
//J アプリケーションのロジックの更新を行います。updateUtil()は冒頭で必ず呼び出します。
int LibHomebrew::Application::update(void) {
	int ret = 0;
	(void)ret;

	//E Update SampleUtil
	//J SampleUtilの更新
	ret = updateUtil();
	if (ret != SCE_OK) {
		return ret;
	}

	//E Get UserService events while it returns SCE_OK.
	//J SCE_OK が返ってくる間 UserService のイベントを取得する。
	SceUserServiceEvent event;
	for (;;) {
		ret = sceUserServiceGetEvent(&event);
		if (ret == SCE_OK) {
			//E Call event handler when UserService events can get.
			//J UserService のイベントが取得できたらイベントハンドラを呼び出す。
			userEventHandler(&event);
		} else {
			SCE_SAMPLE_UTIL_ASSERT(ret == SCE_USER_SERVICE_ERROR_NO_EVENT);
			break;
		}
	}

	//E Refresh User color on SystemService resume event.
	//J SystemService の resume event が来たらユーザーカラーを再取得する
	SceSystemServiceStatus systemStatus;
	SceSystemServiceEvent systemEvent;

	ret = sceSystemServiceGetStatus(&systemStatus);
	if ((ret == SCE_OK) && (systemStatus.eventNum > 0)) {
		for (int i = 0; i < systemStatus.eventNum; i++) {
			ret = sceSystemServiceReceiveEvent(&systemEvent);
			if (ret == SCE_OK) {
				if (systemEvent.eventType == SCE_SYSTEM_SERVICE_EVENT_ON_RESUME) {
					// This process was resumed.
					for (std::vector<UserInfo*>::iterator it = usersInfo.begin(); it != usersInfo.end(); it++) {
						SceUserServiceUserColor userColor;
						ret = sceUserServiceGetUserColor((*it)->userId, &userColor);
						if (ret == SCE_OK) {
							(*it)->setColor(userColor);
						}
					}
				}
			}
		}
	}

	if (!isPlaying) {
		for (std::vector<UserInfo*>::iterator it = usersInfo.begin(); it != usersInfo.end(); it++) {
			ret = (*it)->update(getGraphicsContext());
			if (ret != SCE_OK) {
				return 1;
			}
		}
	}

	// Update/Run player.
	if (isPlaying) {
		int ret;
		(void)ret;
		int32_t vhandle = Graphics()->getVideoOutHandle();
		sceVideoOutWaitVblank(vhandle);
		if (player.isPlaying()) {
			ret = player.update();
			SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
		}
	} else if (input == ssi::kButtonR1) {
		if (useVid) {
			Play();
			input = ssi::kButtonNone;
		}		
	}

	// Run User Main Entry.
	if (start) {		
		RunUserEntry();
		start = false;
	}
	return SCE_OK;
}

// Render to the screen.
void LibHomebrew::Application::render(void) {
	Graphics()->beginScene(getGraphicsContext()->getNextRenderTarget(), Graphics()->getDepthStencilSurface());
	Graphics()->clearRenderTarget(0x00000000);
	Graphics()->setDepthFunc(sce::SampleUtil::Graphics::kDepthFuncAlways);	// for drawDebugStringf

	if (isPlaying) player.render(Graphics(), sprite);
	else {
		// Draw on screen TTY.
		if (multiLine) TTY::render(getSpriteRenderer(), getGraphicsContext());

		// Draw Title.		
		if (useTitle) drawStringf(titlePos, titleSize, titleColor, titleBuffer);

		// Draw Timer.		
		if (useTime) drawStringf(timePos, timeSize, timeColor, timeBuffer);

		// Draw Banner.
		if (useBanner) {
			if (bannerEffect) {
				if (bannerCount != interval) {
					drawStringf(shaderPos, bannerSize, DARK_GRAY_SHATTERED, banner);
					drawStringf(bannerPos, bannerSize, bannerColor, banner);
				}
			}
		}

		// Print Message Buffer if data present.
		if (!multiLine) {
			if (strlen(Console::singleCenterBuff) > 0) drawStringf(Position(getCenteredPosX(strlen(Console::singleCenterBuff)), 0.50), Console::singleCenterBuff);
		}

		// Draw external forms.
		if (drawFuncs.size() > 0) {
			for (std::vector<void (*)()>::iterator it = drawFuncs.begin(); it != drawFuncs.end(); it++) {
				(*it)();
			}
		}

		// Draw cursor.
		for (std::vector<UserInfo*>::iterator it = usersInfo.begin(); it != usersInfo.end(); it++) {
			(*it)->render(Graphics(), sprite);
		}
	}
	Graphics()->endScene();
	Graphics()->flip(1);
}

// Finalize the app, close all used resources.
int LibHomebrew::Application::finalize(void) {
	int ret = 0;
	(void)ret;

	// Clear AV Player.
	if (isPlaying) player.stop();
	player.finalize();

	// Clear Resource Manager.
	resManager.finalize();

	// Clear Internal Renderer.
	sce::SampleUtil::destroy(sprite);
	sce::SampleUtil::destroy(pLoader);
	sprite = NULL;
	pLoader = NULL;

	// Finalize all missing stuff.
	delete[] sandBoxDirectory;
	delete title;
	delete banner;
	delete videoPath;

	// Finalize all logged in users.
	std::vector<UserInfo*>::iterator it;
	for (it = usersInfo.begin(); it != usersInfo.end(); it++) {
		usersInfo.erase(it);
		ret = (*it)->finalize();
		SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);
	}

	ret = sceUserServiceTerminate();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	ret = finalizeUtil();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	return SCE_OK;
}

// Just a shorter symbol.
G::GraphicsContext *LibHomebrew::Application::Graphics(void) { return getGraphicsContext(); }

// Internal shorter Symbol for drawing a Rectagnel.
void LibHomebrew::Application::drawRect(float x, float y, float width, float height, Color color) {
	sprite->drawRect(Graphics(), Position(x, y), Size(width, height), color);
}

// Internal shorter Symbol for drawing a Rectagnel.
void LibHomebrew::Application::drawRect(Position pos, Size size, Color color) {
	sprite->drawRect(Graphics(), pos, size, color);
}

// Internal shorter Symbol for drawing a Rectagnel.
void LibHomebrew::Application::fillRect(float x, float y, float width, float height, Color color) {
	sprite->fillRect(Graphics(), Position(x, y), Size(width, height), color);
}

// Internal shorter Symbol for drawing a Rectagnel.
void LibHomebrew::Application::fillRect(Position pos, Size size, Color color) {
	sprite->fillRect(Graphics(), pos, size, color);
}

// Internal Draw String.
int LibHomebrew::Application::drawStringf(float x, float y, float size, Color color, const char *format, ...) {
	char buf[256];
	int n;

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, sizeof(buf), format, args);
	Position pos = Position(x, y);
	if (sprite != NULL) sprite->drawDebugString(Application::Graphics(), pos, size, color, buf);
	va_end(args);

	return n;
}

// Internal Draw String.
int LibHomebrew::Application::drawStringf(float x, float y, float size, const char *format, ...) {
	char buf[256];
	int n;

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, sizeof(buf), format, args);
	Position pos = Position(x, y);
	if (sprite != NULL) sprite->drawDebugString(Application::Graphics(), pos, size, WHITE, buf);
	va_end(args);

	return n;
}

// Internal Draw String.
int LibHomebrew::Application::drawStringf(Position pos, float size, Color color, const char *format, ...) {
	char buf[256];
	int n;

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, sizeof(buf), format, args);
	if (sprite != NULL) sprite->drawDebugString(Graphics(), pos, size, color, buf);
	va_end(args);

	return n;
}

// Internal Draw String.
int LibHomebrew::Application::drawStringf(float x, float y, Color color, const char *format, ...) {
	char buf[256];
	int n;

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, sizeof(buf), format, args);
	Position pos = Position(x, y);
	if (sprite != NULL) sprite->drawDebugString(Graphics(), pos, CHAR_N, color, buf);
	va_end(args);

	return n;
}

// Internal Draw String.
int LibHomebrew::Application::drawStringf(Position pos, Color color, const char *format, ...) {
	char buf[256];
	int n;

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, sizeof(buf), format, args);
	if (sprite != NULL) sprite->drawDebugString(Graphics(), pos, CHAR_N, color, buf);
	va_end(args);

	return n;
}

// Internal Draw String. (Writes in white)
int LibHomebrew::Application::drawStringf(float x, float y, const char *format, ...) {
	char buf[256];
	int n;

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, sizeof(buf), format, args);
	Position pos = Position(x, y);
	if (sprite != NULL) sprite->drawDebugString(Graphics(), pos, CHAR_N, WHITE, buf);
	va_end(args);

	return n;
}

// Internal Draw String. (Writes in white)
int LibHomebrew::Application::drawStringf(Position pos, const char *format, ...) {
	char buf[256];
	int n;

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, sizeof(buf), format, args);
	if (sprite != NULL) sprite->drawDebugString(Graphics(), pos, CHAR_N, WHITE, buf);
	va_end(args);

	return n;
}

// Get centered position for x by a string, based on his length.
float LibHomebrew::Application::getCenteredPosX(int len) { return 0.5 - (len / (float)CHAR_COL / 2.0) + MARGIN_X; }

// Center Position for a Form.
Position LibHomebrew::Application::CenterForm(float x, float y) { return Position((0.5 - (x / 2.0)), (0.5 - (y / 2.0))); }

// Run Video.
void LibHomebrew::Application::Play(void) {
	int ret;
	(void)ret;
	if (strlen(videoPath) > 0) {
		Console::WriteLine("Trying to run Video...");
		int32_t vhandle = Graphics()->getVideoOutHandle();
		for (int i = 0; i < 3; i++) sceVideoOutWaitVblank(vhandle);
		ret = player.start(videoPath);
		if (ret == SCE_OK) {
			isPlaying = true;
			Console::WriteLine("Ok !\n");
		} else Console::WriteLine("Error !\n");
	} else Console::WriteError("No video, no path defined !\nNeed some coffee ?\n");
}

// Run the application loop.
int LibHomebrew::Application::exec(void) {
	int ret = 0;
	(void)ret;

	ret = initialize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	while (1) {
		if (close) break;
		ret = update();
		if (ret != SCE_OK) break;
		render();
	}

	ret = finalize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	// Dirty but since we can't exit to gui, we simple try to open a non existing path via syscall symbol.
	Sys::open("/let/me/out/here/", O_RDONLY, 0);

	return 0;
}

// Add User Main Entry to the Application to run after initializing.
void LibHomebrew::Application::Add(void *usrLoop) { UsrEntry = reinterpret_cast<void *(*)(void*)>(usrLoop); }

// Add a external Draw function to the Applications drawing loop.
void LibHomebrew::Application::AddDraw(void (*drawEvent)()) { drawFuncs.push_back(drawEvent); }

// Remove a external Draw function from the Applications drawing loop.
void LibHomebrew::Application::RemoveDraw(void (*drawEvent)()) {
	drawFuncs.erase(std::remove(drawFuncs.begin(), drawFuncs.end(), drawEvent), drawFuncs.end());
}

// Set Application Title Text.
void LibHomebrew::Application::Title(const char *_title) { title = strdup(_title); }

// Set Application Title size.
void LibHomebrew::Application::TitleSize(float size) { titleSize = size; }

// Set the position of the Title.
void LibHomebrew::Application::TitlePos(Position pos) { titlePos = pos; }

// Set the position of the Title with floats.
void LibHomebrew::Application::TitlePos(float x, float y) { titlePos = Position(x, y); }

// Set Title Color.
void  LibHomebrew::Application::TitleColor(Color _color) { titleColor = _color; }

// Set Timer size.
void LibHomebrew::Application::TimeSize(float size) { timeSize = size; }

// Set the Position of the time.
void LibHomebrew::Application::TimePos(Position pos) { timePos = pos; }

// Set the Position of the time with floats.
void LibHomebrew::Application::TimePos(float x, float y) { timePos = Position(x, y); }

// Set Time Color.
void  LibHomebrew::Application::TimeColor(Color _color) { timeColor = _color; }

// Set Baner Text.
void LibHomebrew::Application::Banner(const char *_banner) { banner = strdup(_banner); }

// Set the position of the Banner.
void LibHomebrew::Application::BannerPos(Position pos) { bannerPos = pos; }

// Set the position of the Banner.
void LibHomebrew::Application::BannerPos(float x, float y) { bannerPos = Position(x, y); }

// Set the position of Shader Banner.
void LibHomebrew::Application::ShaderPos(Position pos) { shaderPos = pos; }

// Set the position of the Shader for the banner.
void LibHomebrew::Application::ShaderPos(float x, float y) { shaderPos = Position(x, y); }

// Set Banner size.
void LibHomebrew::Application::BannerSize(float size) { bannerSize = size; }

// Set Banner Color.
void  LibHomebrew::Application::BannerColor(Color _color) { bannerColor = _color; }

// Set to use a title.
void LibHomebrew::Application::UseTitle(bool state) { useTitle = state; }

// Set to use a time to display.
void LibHomebrew::Application::UseTime(bool state) { useTime = state; }

// Set to use a Banner.
void LibHomebrew::Application::UseBanner(bool state) { useBanner = state; }

// Set Application to use Multi line output or just single line.
void LibHomebrew::Application::MultiLine(bool state) { multiLine = state; }

// Enable screenshot feature on button Triangle.
void LibHomebrew::Application::UseScreenShot(bool state) { useScreenShot = state; }

// Enable to play a mp4.
void LibHomebrew::Application::UseVideo(bool state) { useVid = state; }

// Set the Vide path.
void LibHomebrew::Application::Video(const char *path) { videoPath = strdup(path); }

// Closing the Application.
void LibHomebrew::Application::Close(void) { close = true; }

// Returns the user input.
ssi::Button LibHomebrew::Application::Input(void) { return input; }

// Clear the input if needed.
void LibHomebrew::Application::ClearInput(void) { input = ssi::kButtonNone; }

//E Handle UserService events.
//J UserService のイベントをハンドルします。
int LibHomebrew::Application::userEventHandler(SceUserServiceEvent *event) {
	if (event == NULL) {
		return 0;
	}
	if (event->eventType == SCE_USER_SERVICE_EVENT_TYPE_LOGIN) {
		onLogin(event->userId);
	}
	else if (event->eventType == SCE_USER_SERVICE_EVENT_TYPE_LOGOUT) {
		onLogout(event->userId);
	}

	return 0;
}

//E procedures on user login.
//J ユーザーがログインした時の処理。
int LibHomebrew::Application::onLogin(SceUserServiceUserId userId) {
	int ret;
	(void)ret;
	//E Get local user name of the user.
	//E After user have linked to Sony Entertainment Network account, it is the same as Online ID .
	//J ユーザーのローカルユーザー名を取得する。
	//J Sony Entertainment Networkアカウントを登録すると、オンラインIDと同じものが設定されている。
	char userName[SCE_USER_SERVICE_MAX_USER_NAME_LENGTH + 1];
	memset(userName, 0, sizeof(userName));
	ret = sceUserServiceGetUserName(userId, userName, sizeof(userName));
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	SceUserServiceUserColor userColor;
	ret = sceUserServiceGetUserColor(userId, &userColor);
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	UserInfo *userInfo = new UserInfo(userId, userName, userColor);

	ret = userInfo->initialize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	usersInfo.push_back(userInfo);

	printf("user \"%s\" (0x%x) logged in\n", userName, userId);

	return SCE_OK;
}

//E procedures on user logout.
//J ユーザーがログアウトした時の処理。
int LibHomebrew::Application::onLogout(SceUserServiceUserId userId) {
	int ret = 0;
	(void)ret;

	std::vector<UserInfo*>::iterator it;
	UserInfo *userInfo = NULL;
	for (it = usersInfo.begin(); it != usersInfo.end(); it++) {
		if ((*it)->userId == userId) {
			userInfo = *it;
			usersInfo.erase(it);
			break;
		}
	}

	printf("user \"%s\" (0x%x) logged out\n", userInfo->userName.c_str(), userInfo->userId);

	ret = userInfo->finalize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	delete userInfo;

	return SCE_OK;
}