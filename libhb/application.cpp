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

// Included Library stubs.
#pragma comment( lib , "SceSysmodule_stub_weak")
#pragma comment( lib , "SceSystemService_stub_weak")
#pragma comment( lib , "SceVideoOut_stub_weak")
#pragma comment( lib , "SceAvPlayer_stub_weak" )

#define LIBRARY_IMPL (1)
#include <algorithm>
#include <video_out.h>
#include <system_service.h>
#include "printf.h"
#include "application.h"
#include "prx/console.h"
#include "prx/file_info.h"
#include "prx/ps4_process.h"
#include "prx/syscall.h"
#include "prx/syscalls.h"

using namespace LibHomebrew;
using namespace LibHomebrew::Loot;
using namespace LibHomebrew::PS4IO;
using namespace common;

// AV Player.
common::Util::AvPlayer player;
ssi::Button            LibHomebrew::Application::input = ssi::kButtonNone;

// Resources
ResourceManager        LibHomebrew::Application::resManager;
SoundManager           LibHomebrew::Application::soundManager;
EventDispatcher        LibHomebrew::Application::eventDispatcher;
Config                 LibHomebrew::Application::conf;

// Console informations.
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
bool LibHomebrew::Application::useResources  = false;
bool LibHomebrew::Application::useSound      = false;
bool LibHomebrew::Application::useIme        = false;
bool LibHomebrew::Application::useDialog     = false;
String LibHomebrew::Application::titleId("");
EventDataUserInfo LibHomebrew::Application::data;
PictureBox LibHomebrew::Application::bgi;
Position LibHomebrew::Application::titlePos  = Position(0.0, 0.0);
Position LibHomebrew::Application::timePos   = Position(0.01, 0.0);
Position LibHomebrew::Application::bannerPos = Position(0.0, 0.0);
Position LibHomebrew::Application::shaderPos = Position(0.0, 0.0);
Color LibHomebrew::Application::titleColor   = LIGHT_BLUE;;
Color LibHomebrew::Application::timeColor    = RED2;
Color LibHomebrew::Application::bannerColor  = YELLOW;
float LibHomebrew::Application::titleSize    = CHAR_N;
float LibHomebrew::Application::timeSize     = CHAR_S;
float LibHomebrew::Application::bannerSize   = 0.08;

//The domain that stores the character string of the result
wchar_t LibHomebrew::Application::resultTextBuf[TEXT_MAX_LENGTH + 1];

// Effect fun.
float posX;
float posY;
float _posX;
float _posY;
//static int count;
int bannerCount;
int pauseCount;
int blinked;
static int interval       = 80;
//static int intervalShowen = 3;
static bool bannerEffect  = true;

// The user Main Entry.
void *(*UsrEntry)(void*) = 0;

// The Thread for the User main Entry.
ScePthread usrMainEntry;
void RunUserEntry(void) {
	// Set to run the User Entry Loop in Core 1.
	scePthreadSetaffinity(usrMainEntry, 1);
	
	// Run UserEntry.
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

int LibHomebrew::Application::UserInfo::update(ssg::GraphicsContext *graphicsContext, Application *app) {
	if (padContext->update() >= SCE_OK) {
		// Update Left Stick.
		/*sce::Vectormath::Simd::Aos::Vector2 leftStick = padContext->getLeftStick();
		posY += leftStick.getY() * 0.01;
		posX += leftStick.getX() * 0.005;*/

		// Compare new position against the old one and count up if they are same, aka stick didn't move.
		// Cursor and button navi effect.
		/*if (posY == _posY) {
			if (posX == _posX) {
				if (count != 300) count++;
			} else count = 0;
		} else count = 0;
		_posY = posY;
		_posX = posX;*/

		// Are we out of range ?
		/*if (posX < 0.0) posX = 0.0;
		if (posX > 1.0) posX = 1.0;
		if (posY < 0.0) posY = 0.0;
		if (posY > 1.0) posY = 1.0;*/

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

		// Do Buttons.
		if (padContext->isButtonDown(ssi::kButtonUp, ssi::kButtonEventPatternAny)) {
			// Moved into TTY Class so we can determine if there are any more lines on top and on end when using Line Highlighting.
		} else if (padContext->isButtonDown(ssi::kButtonDown, ssi::kButtonEventPatternAny)) {
			// Moved into TTY Class so we can determine if there are any more lines on top and on end when using Line Highlighting.
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
		} else if (padContext->isButtonPressed(ssi::kButtonR1, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonR1;
			Console::SetUserInput(ssi::kButtonR1);
		} else if (padContext->isButtonPressed(ssi::kButtonR2, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonR2;
			Console::SetUserInput(ssi::kButtonR2);
		} else if (padContext->isButtonPressed(ssi::kButtonR3, ssi::kButtonEventPatternAny)) {
			input = ssi::kButtonR3;
			Console::SetUserInput(ssi::kButtonR3);			
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

		if (app->textViewer.size() > 0) {
			for (std::vector<TextViewer *>::iterator it = app->textViewer.begin(); it != app->textViewer.end(); it++) {
				(*it)->Update(padContext);
			}
		}
		if (app->hexViewer.size() > 0) {
			for (std::vector<HexViewer *>::iterator it = app->hexViewer.begin(); it != app->hexViewer.end(); it++) {
				(*it)->Update(padContext);
			}
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

LibHomebrew::Application::~Application(void) { close = true; }

// Initialize the Application Instance.
int LibHomebrew::Application::initialize(void) {
	int ret = 0;
	(void)ret;

	//E Initialize SampleUtil. Graphics feature and SpriteRenderer feature of SampleUtil are enabled.
	//J SampleUtilの初期化。SampleUtilのGraphics機能とSpriteRenderer機能を有効化。
	ret = initializeUtil(kFunctionFlagGraphics | kFunctionFlagSpriteRenderer | kFunctionFlagAudio);
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
		
	// Shall we use Ime Dialog ?
	if (useIme) {
		wcsncpy(resultTextBuf, L"Edit", sizeof(resultTextBuf) / sizeof(wchar_t));

		imeDialog = new ImeDialogWrapper();
		SCE_SAMPLE_UTIL_ASSERT(imeDialog != NULL);
		imeDialog->initialize();
	}

	// Shall we use Message Dialog ?
	if (useDialog) {
		msgDialog = new MsgDialogWrapper();
		SCE_SAMPLE_UTIL_ASSERT(msgDialog != NULL);
		ret = msgDialog->initialize();
		if (ret == SCE_OK) msgDialog->doNotTerminate(true);    // Needed so we can re-use the dialog without the need to initialize it over and over again.
	}

	// Get Freedom for this Process.
	int uid = Sys::getuid();
	if (uid != 0) Proc::Freedom();

	// Get App path.
	String appPath("/mnt/sandbox/pfsmnt/");
	appPath += titleId;
	appPath += "-app0/";

	// Initialize config.
	String configLua(appPath);
	configLua += "game_data/config.lua";
	if (useResources | useSound) conf.initialize(configLua.c_str(), "english_us");
	
	// Initialize Resource Manager.
	resManager.initialize(Graphics(), sprite, &conf, width, height);

	// Shall we load resources from the app folder ?
	if (useResources) resManager.load();

	// Initialize Sound Manager.
	if (useSound) soundManager.initialize(Audio(), &conf, &eventDispatcher);

	// Initialize AV Player.
	player.initialize(Graphics(), sprite, Audio(), &resManager.directMemoryHeap);	

	// Clear Buffer before using.
	memset(messageBuffer, 0, sizeof(messageBuffer));
	memset(titleBuffer, 0, sizeof(titleBuffer));
	memset(timeBuffer, 0, sizeof(timeBuffer));
	memset(timeBuffer2, 0, sizeof(timeBuffer2));

	// Generate Titel string with version.
	if (useTitle) {
		snprintf(titleBuffer, sizeof(titleBuffer), "%s", title);
		titlePos = Position(getCenteredPosX(strlen(titleBuffer)), MARGIN_Y);
	}

	// Set start and close flags.
	start = true;
	close = false;

	// Set Background Image properties.
	bgi.setFillMode(bgi.kFillModeDotByDot);
	bgi.setVAlign(bgi.kVAlignCenter);
	bgi.setHAlign(bgi.kHAlignCenter);	
	bgi.setPosition(Position(0.0f, 0.0f));
	bgi.setSize(Size(1.0f, 1.0f));
	bgi.Visible(true);
	bgi.Hide();
	
	// Clear SplashScreen	
	sceSystemServiceHideSplashScreen();
	_printf("[Application] Initializing done.\n");

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

	// Update User Info aka pad to userId.
	if (!isPlaying) {
		for (std::vector<UserInfo*>::iterator it = usersInfo.begin(); it != usersInfo.end(); it++) {
			ret = (*it)->update(getGraphicsContext(), this);
			if (ret != SCE_OK) {
				return 1;
			}
		}		
	}

	// Update/Run player.
	if (isPlaying) {
#ifdef FW_350
		int32_t vhandle = Graphics()->getVideoOutHandle();
		sceVideoOutWaitVblank(vhandle);
#endif
		if (player.isPlaying()) {
			ret = player.update();
			SCE_SAMPLE_UTIL_ASSERT_EQUAL(ret, SCE_OK);
		}
	}

	// Run User Main Entry.
	if (start) {		
		RunUserEntry();
		start = false;
	}

	// Update Dialogs.
	if (!isPlaying) {
		if (useIme) {
			if (imeDialog->everyFrame() == ImeDialogWrapper::STATUS_DIALOG_FINISH) {
				_printf("[Application] IME set resultTextBuf to \"%ls\"\n", resultTextBuf);
			}
		}
		if (useDialog) msgDialog->everyFrame();
	}
	return SCE_OK;
}

// Render to the screen.
void LibHomebrew::Application::render(void) {
	Graphics()->beginScene(getGraphicsContext()->getNextRenderTarget(), Graphics()->getDepthStencilSurface());
	Graphics()->clearRenderTarget(0x00000000);
	Graphics()->setDepthFunc(G::kDepthFuncAlways);	// for drawDebugStringf

	if (isPlaying) player.render(Graphics(), sprite);
	else {
		// Draw Background Image, if set.
		bgi.Draw(Graphics(), sprite);

		// Draw external forms.	
		if (drawFuncs.size() > 0) {
			for (std::vector<void(*)()>::iterator it = drawFuncs.begin(); it != drawFuncs.end(); it++) {
				(*it)();
			}
		}

		// Draw on screen TTY.
		if (multiLine) TTY::render(sprite, Graphics());

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
		if (forms.size() > 0) {
			for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
				(*it)->Draw(Graphics(), sprite);
			}
		}
		if (widgets.size() > 0) {
			for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
				(*it)->Draw(Graphics(), sprite);
			}
		}
		if (pictures.size() > 0) {
			for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
				// Change the finger position.
				if (!strcmp((*it)->getId(), "finger")) {
					Position pos = Position(0.0f, 0.05f + (TTY::getSelectedLineIndex() * 0.03f));
					(*it)->setPosition(pos);
				}				
				(*it)->Draw(Graphics(), sprite);
			}
		}
		if (rtbs.size() > 0) {
			for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
				(*it)->Draw(Graphics(), sprite);
			}
		}
		if (textViewer.size() > 0) {
			for (std::vector<TextViewer *>::iterator it = textViewer.begin(); it != textViewer.end(); it++) {
				(*it)->Draw(Graphics(), sprite);
			}
		}
		if (hexViewer.size() > 0) {
			for (std::vector<HexViewer *>::iterator it = hexViewer.begin(); it != hexViewer.end(); it++) {
				(*it)->Draw(Graphics(), sprite);
			}
		}

		// Draw cursor.
		/*for (std::vector<UserInfo*>::iterator it = usersInfo.begin(); it != usersInfo.end(); it++) {
			(*it)->render(Graphics(), sprite);
		}*/
	}
	Graphics()->endScene();
	Graphics()->flip(1);
}

// Finalize the app, close all used resources.
int LibHomebrew::Application::finalize(void) {
	int ret = 0;
	(void)ret;

	// Finalize Dialogs.
	if (useIme && imeDialog) {
		const int unloadModuleResult = sceSysmoduleUnloadModule(SCE_SYSMODULE_IME_DIALOG);
		if (unloadModuleResult != SCE_OK) _printf("[Application] Error unloading IME Dialog PRX: 0x%8X\n", unloadModuleResult);
		delete imeDialog;
		imeDialog = NULL;
	}
	if (useDialog && msgDialog) {
		msgDialog->finalize();
		const int unloadModuleResult = sceSysmoduleUnloadModule(SCE_SYSMODULE_MESSAGE_DIALOG);
		if (unloadModuleResult != SCE_OK) _printf("[Application] Error unloading Message Dialog PRX: 0x%8X\n", unloadModuleResult);
		delete msgDialog;
		msgDialog = NULL;
	}

	// Clear used Resources of the Sound Manager.
	if (useSound) soundManager.finalize();

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

	// Free Event data.
	delete data.userInfo;
	data.userInfo = NULL;

	// Finalize all missing stuff.
	free(title);
	free(banner);
	free(videoPath);

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

// Just a shorter symbol.
A::AudioContext *LibHomebrew::Application::Audio(void) { return getAudioContext(); }

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
int LibHomebrew::Application::drawStringArrayf(float x, float y, float size, Color color, const char *format, ...) {
	char buf[1000];
	char tmp[1000];
	int n, startIndex, lines;

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, sizeof(buf), format, args);
	Position pos = Position(x, y);
	int len = strlen(buf);
	if (sprite != NULL) {
		startIndex = 0;
		lines = 0;
		for (int i = 0; i < len; i++) {
			if (buf[i] == '\n') {
				strncat(tmp + startIndex + (startIndex ? 1 : 0), buf + startIndex + (startIndex ? 1 : 0), i - startIndex);
				startIndex = i;
				sprite->drawDebugString(Application::Graphics(), Position(pos.getX(), pos.getY() + (lines * size)), size, color, tmp);
				lines++;
				memset(tmp, 0, i - startIndex);
			}
		}
		if (startIndex == 0) sprite->drawDebugString(Application::Graphics(), pos, size, color, buf);
		else if (startIndex != len) {
			strncat(tmp + startIndex + (startIndex ? 1 : 0), buf + startIndex + (startIndex ? 1 : 0), len - startIndex);
			sprite->drawDebugString(Application::Graphics(), Position(pos.getX(), pos.getY() + (lines * size)), size, color, tmp);
		}
	}
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

// Internal Draw String. (Writes in white)
int LibHomebrew::Application::drawStringArrayf(Position pos, const char *format, ...) {
	char buf[1000];
	char tmp[1000];
	int n, startIndex, lines;

	va_list args;
	va_start(args, format);
	n = vsnprintf(buf, sizeof(buf), format, args);
	int len = strlen(buf);
	if (sprite != NULL) {
		startIndex = 0;
		lines = 0;
		for (int i = 0; i < len; i++) {
			if (buf[i] == '\n') {
				strncat(tmp + startIndex + (startIndex ? 1 : 0), buf + startIndex + (startIndex ? 1 : 0), i - startIndex);
				startIndex = i;
				sprite->drawDebugString(Application::Graphics(), Position(pos.getX(), pos.getY() + (lines * 0.03f)), 0.03f, WHITE, tmp);
				lines++;
				memset(tmp, 0, i - startIndex);
			}
		}
		if (startIndex == 0) sprite->drawDebugString(Application::Graphics(), pos, 0.03f, WHITE, buf);
		else if (startIndex != len) {
			strncat(tmp + startIndex + (startIndex ? 1 : 0), buf + startIndex + (startIndex ? 1 : 0), len - startIndex);
			sprite->drawDebugString(Application::Graphics(), Position(pos.getX(), pos.getY() + (lines * 0.03f)), 0.03f, WHITE, tmp);
		}
	}
	va_end(args);

	return n;
}

// Get centered position for x by a string, based on his length.
float LibHomebrew::Application::getCenteredPosX(int len) { return 0.5 - (len / (float)CHAR_COL / 2.0) + MARGIN_X; }

// Run Video.
void LibHomebrew::Application::Play(void) {
	if (!isPlaying) {
		int ret;
		(void)ret;
		if (strlen(videoPath) > 0) {
			_printf("[Application] Trying to run Video...");
#ifdef FW_350
			int32_t vhandle = Graphics()->getVideoOutHandle();
			for (int i = 0; i < 3; i++) sceVideoOutWaitVblank(vhandle);
#endif
			ret = player.start(videoPath);
			if (ret == SCE_OK) {
				isPlaying = true;
				_printf("Ok !\n");
			} else _printf("Error !\n");
		} else _printf("[Application] No video, no path defined !\n[Application] Need some coffee ?\n");
	}
}

// Run Video.
int LibHomebrew::Application::Play(const char *path) {
	int ret = 0;
	
	if (!isPlaying) {
		(void)ret;
		if (strlen(path) > 0) {
			_printf("[Application] Trying to run Video...");
#ifdef FW_350
			int32_t vhandle = Graphics()->getVideoOutHandle();
			for (int i = 0; i < 3; i++) sceVideoOutWaitVblank(vhandle);
#endif
			ret = player.start(path);
			if (ret == SCE_OK) {
				isPlaying = true;
				_printf("Ok !\n");
			} else _printf("Error !\n");
		} else _printf("[Application] No video, no path defined !\nNeed some coffee ?\n");
	}

	return ret;
}

// Trigger Play if a video file was already loaded and startet. (Does not load a file and play it.)
void LibHomebrew::Application::AVP_TriggerPlay(void) {
	if (isPlaying) {
		
	}
}

// Trigger Resume if a video file was already loaded and startet.
void LibHomebrew::Application::AVP_TriggerResume(void) {
	if (isPlaying) {

	}
}

// Trigger Pause if a video file was already loaded and startet.
void LibHomebrew::Application::AVP_TriggerPause(void) {
	if (isPlaying) {

	}
}

// Trigger Stop if a video file was already loaded and startet.
void LibHomebrew::Application::AVP_TriggerStop(void) {
	if (isPlaying) {
		player.stop();
		isPlaying = false;
	}
}

// The App Loop.
void *AppLoop(void *app) {
	int ret = 0;
	(void)ret;

	while (1) {
		if (((Application *)app)->IsClosed()) break;
		ret = ((Application *)app)->update();
		if (ret != SCE_OK) break;
		((Application *)app)->render();
	}
	return 0;
}

// Run the application loop in a own quite core.
ScePthread appLoop;
int LibHomebrew::Application::exec(Application *app) {
	int ret = 0;
	(void)ret;

	// Initialize the app before we gonna loop.
	ret = ((Application *)app)->initialize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	// Set App Loop to run in Core 3.
	scePthreadSetaffinity(appLoop, 3);

	// Run the App Loop.
	if (scePthreadCreate(&appLoop, NULL, AppLoop, app, "App Loop") == SCE_OK) {
		// Wait for exit.
		scePthreadJoin(appLoop, NULL);
	}

	// Finalize, clear all used resources.
	ret = ((Application *)app)->finalize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

    return 0;
}

// Run the application loop in a own quite core.
/*int LibHomebrew::Application::exec(void) {
	int ret = 0;
	(void)ret;

	ret = initialize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	while (1) {
		if (IsClosed()) break;
		ret = update();
		if (ret != SCE_OK) break;
		render();
	}

	ret = finalize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);
	return 0;
}*/

// Set the Title ID of this running Program.
void LibHomebrew::Application::SetTitleId(const char *titleID) { titleId = titleID; }

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

// Enable to use Sound Effects.
void LibHomebrew::Application::UseSound(bool state) { useSound = state; }

// Enable to use and load Application Resources.
void LibHomebrew::Application::UseResources(bool state) { useResources = state; }

// Enable Ime Dialog.
void LibHomebrew::Application::UseIme(void) { useIme = true; }

// Enable Msg Dialog.
void LibHomebrew::Application::UseDialog(void) { useDialog = true; }

// Show Ime Dialog.
wchar_t *LibHomebrew::Application::ShowIme(wchar_t *title, wchar_t *placeholder) {
	if (imeDialog->getStatus() == ImeDialogWrapper::STATUS_BEGIN) {
		memset(resultTextBuf, 0, sizeof(resultTextBuf) / sizeof(wchar_t));
		wcsncpy(resultTextBuf, L"Edit", sizeof(resultTextBuf) / sizeof(wchar_t));
		
		const int32_t showDialogResult = imeDialog->show_ime_dialog(title, placeholder, resultTextBuf, TEXT_MAX_LENGTH);
		if (showDialogResult != SCE_OK) {
			_printf("[Application] Error in show_ime_dialog(): 0x%8X\n", showDialogResult);
			return resultTextBuf;
		}

		ImeDialogWrapper::Status status;
		while (true) {
			status = imeDialog->getStatus();
			if (status == ImeDialogWrapper::STATUS_DIALOG_FINISH) break;
			else if (status == ImeDialogWrapper::STATUS_DIALOG_ERROR) break;
		}
	}
	return resultTextBuf;
}

// Show Msg Dialog.
DialogResult LibHomebrew::Application::ShowMsg(const char *message) {
	// Set a result.
	DialogResult _result = DialogResult::Error;

	// Is Dialog Ready ?
	if (msgDialog->getStatus() == MsgDialogWrapper::STATUS_BEGIN) {		
		// Show the dialog.
		const int32_t showDialogResult = msgDialog->show_msg_dialog(message);
		if (showDialogResult != SCE_OK) {
			_printf("[Application] Error in show_msg_dialog(): 0x%8X\n", showDialogResult);
			goto done;
		}

		// Wait for Dialog to be done.
		MsgDialogWrapper::Status status;
		while (true) {
			status = msgDialog->getStatus();
			if (status == MsgDialogWrapper::STATUS_DIALOG_FINISH) break;
			else if (status == MsgDialogWrapper::STATUS_DIALOG_ERROR) break;
		}

		// Resolve the result.	
		if (status == MsgDialogWrapper::STATUS_DIALOG_FINISH) {
			uint32_t result = msgDialog->getResult();
			if (result == SCE_MSG_DIALOG_BUTTON_ID_OK) {
				_result = PS4Forms::DialogResult::Ok;
			} else if (result == SCE_MSG_DIALOG_BUTTON_ID_NO) {
				_result = DialogResult::No;
			} else if (result == SCE_MSG_DIALOG_BUTTON_ID_YES) {
				_result = DialogResult::Yes;
			} else if (result == SCE_MSG_DIALOG_BUTTON_ID_INVALID) {
				_result = DialogResult::Cancel;
			}
		}			
	}
done:
	// Return result.
	return _result;
}

// Show Msg Dialog, defining specific buttons to use.
DialogResult LibHomebrew::Application::ShowMsg(const char *message, Buttons button, const char *button1, const char *button2) {
	// Set a result.
	DialogResult _result = DialogResult::Error;

	// Is Dialog Ready ?
	if (msgDialog->getStatus() == MsgDialogWrapper::STATUS_BEGIN) {
		// Show the dialog.
		const int32_t showDialogResult = msgDialog->show_msg_dialog(message, button, button1, button2);
		if (showDialogResult != SCE_OK) {
			_printf("[Application] Error in show_msg_dialog(): 0x%8X\n", showDialogResult);
			goto done;
		}

		// Wait for Dialog to be done.
		MsgDialogWrapper::Status status;
		while (true) {
			status = msgDialog->getStatus();
			if (status == MsgDialogWrapper::STATUS_DIALOG_FINISH) break;
			else if (status == MsgDialogWrapper::STATUS_DIALOG_ERROR) break;
		}

		// Resolve the result.	
		if (status == MsgDialogWrapper::STATUS_DIALOG_FINISH) {
			uint32_t result = msgDialog->getResult();
			if (result == SCE_MSG_DIALOG_BUTTON_ID_OK) {
				_result = PS4Forms::DialogResult::Ok;
			} else if (result == SCE_MSG_DIALOG_BUTTON_ID_NO) {
				_result = DialogResult::No;
			} else if (result == SCE_MSG_DIALOG_BUTTON_ID_YES) {
				_result = DialogResult::Yes;
			} else if (result == SCE_MSG_DIALOG_BUTTON_ID_INVALID) {
				_result = DialogResult::Cancel;
			} else if (result == SCE_MSG_DIALOG_BUTTON_ID_BUTTON1) {
				_result = DialogResult::Button1;
			} else if (result == SCE_MSG_DIALOG_BUTTON_ID_BUTTON2) {
				_result = DialogResult::Button2;
			}
		}			
	}
done:
	// Return result.
	return _result;
}

// Set the Vide path.
void LibHomebrew::Application::Video(const char *path) { videoPath = strdup(path); }

// Closing the Application.
void LibHomebrew::Application::Close(void) { close = true; }

// Determine if the application loop shall be stopped and this instance be closed.
bool LibHomebrew::Application::IsClosed(void) { return close; }

// Returns the user input.
ssi::Button LibHomebrew::Application::Input(void) { return input; }

// Add a background image to the picture box.
void LibHomebrew::Application::setBackgroundImage(ssg::Texture *texture) {
	bgi.addImage(texture);
	bgi.Show();
}

// Show the Background Image.
void LibHomebrew::Application::ShowBgi(void) { bgi.Show(); }

// Hide the Background Image.
void LibHomebrew::Application::HideBgi(void) { bgi.Hide(); }

// Clear the input if needed.
void LibHomebrew::Application::ClearInput(void) { input = ssi::kButtonNone; }

// Add User Main Entry to the Application to run after initializing.
void LibHomebrew::Application::AddCode(void *usrLoop) { UsrEntry = reinterpret_cast<void *(*)(void*)>(usrLoop); }

// Add a external Draw function to the Applications drawing loop.
void LibHomebrew::Application::Add(void (*drawEvent)()) { drawFuncs.push_back(drawEvent); }

// Add a Form to the Application loop.
void LibHomebrew::Application::Add(PS4Forms::Form *toAdd) { if (toAdd != nullptr) forms.push_back(toAdd); }

// Add a Widget to the Application loop.
void LibHomebrew::Application::Add(PS4Forms::WidgetBase *toAdd) { if (toAdd != nullptr) widgets.push_back(toAdd); }

// Add a PictureBox to the Application loop.
void LibHomebrew::Application::Add(PS4Forms::PictureBox *toAdd) { if (toAdd != nullptr) pictures.push_back(toAdd); }

// Add a RichTextBox to the Application loop.
void LibHomebrew::Application::Add(PS4Forms::RichTextBox *toAdd) { if (toAdd != nullptr) rtbs.push_back(toAdd); }

// Add a TextViewer to the Application loop.
void LibHomebrew::Application::Add(PS4Forms::TextViewer *toAdd) { if (toAdd != nullptr) textViewer.push_back(toAdd); }

// Add a HexViewer to the Application loop.
void LibHomebrew::Application::Add(PS4Forms::HexViewer *toAdd) { if (toAdd != nullptr) hexViewer.push_back(toAdd); }

// Remove a external Draw function from the Applications drawing loop.
void LibHomebrew::Application::Remove(void (*drawEvent)()) {
	drawFuncs.erase(std::remove(drawFuncs.begin(), drawFuncs.end(), drawEvent), drawFuncs.end());
}

// Remove Form from the Application loop.
void LibHomebrew::Application::Remove(PS4Forms::Form *toRemove) {
	if (toRemove != nullptr) {
		forms.erase(std::remove(forms.begin(), forms.end(), toRemove), forms.end());
	}
}

// Remove Widget from the Application loop.
void LibHomebrew::Application::Remove(PS4Forms::WidgetBase *toRemove) {
	if (toRemove != nullptr) {
		widgets.erase(std::remove(widgets.begin(), widgets.end(), toRemove), widgets.end());
	}
}

// Add a PictureBox to the Application loop.
void LibHomebrew::Application::Remove(PS4Forms::PictureBox *toRemove) {
	if (toRemove != nullptr) {
		pictures.erase(std::remove(pictures.begin(), pictures.end(), toRemove), pictures.end());
	}
}

// Remove Form from the Application loop.
void LibHomebrew::Application::Remove(PS4Forms::RichTextBox *toRemove) {
	if (toRemove != nullptr) {
		rtbs.erase(std::remove(rtbs.begin(), rtbs.end(), toRemove), rtbs.end());
	}
}

// Remove Widget from the Application loop.
void LibHomebrew::Application::Remove(PS4Forms::TextViewer *toRemove) {
	if (toRemove != nullptr) {
		textViewer.erase(std::remove(textViewer.begin(), textViewer.end(), toRemove), textViewer.end());
	}
}

// Add a PictureBox to the Application loop.
void LibHomebrew::Application::Remove(PS4Forms::HexViewer *toRemove) {
	if (toRemove != nullptr) {
		hexViewer.erase(std::remove(hexViewer.begin(), hexViewer.end(), toRemove), hexViewer.end());
	}
}

// Get a Form based on his ID.
Form *LibHomebrew::Application::GetFormById(String id) {
	if (forms.size() > 0) {
		for (std::vector<Form *>::iterator it = forms.begin(); it != forms.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a Widget based on his ID.
WidgetBase *LibHomebrew::Application::GetWidgetById(String id) {
	if (widgets.size() > 0) {
		for (std::vector<WidgetBase *>::iterator it = widgets.begin(); it != widgets.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a PictureBox based on his ID.
PictureBox *LibHomebrew::Application::GetPictureById(String id) {
	if (pictures.size() > 0) {
		for (std::vector<PictureBox *>::iterator it = pictures.begin(); it != pictures.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a Form based on his ID.
RichTextBox *LibHomebrew::Application::GetRtbById(String id) {
	if (rtbs.size() > 0) {
		for (std::vector<RichTextBox *>::iterator it = rtbs.begin(); it != rtbs.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a Widget based on his ID.
TextViewer *LibHomebrew::Application::GetTextViewerById(String id) {
	if (textViewer.size() > 0) {
		for (std::vector<TextViewer *>::iterator it = textViewer.begin(); it != textViewer.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

// Get a PictureBox based on his ID.
HexViewer *LibHomebrew::Application::GetHexViewerById(String id) {
	if (hexViewer.size() > 0) {
		for (std::vector<HexViewer *>::iterator it = hexViewer.begin(); it != hexViewer.end(); it++) {
			if (!strcmp((*it)->getId(), id.c_str())) return (*it);
		}
	}
	return nullptr;
}

//E Handle UserService events.
//J UserService のイベントをハンドルします。
int LibHomebrew::Application::userEventHandler(SceUserServiceEvent *event) {
	if (event == NULL) return 0;
	if (event->eventType == SCE_USER_SERVICE_EVENT_TYPE_LOGIN) {
		onLogin(event->userId);
	} else if (event->eventType == SCE_USER_SERVICE_EVENT_TYPE_LOGOUT) {
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

	_printf("[Application] user \"%s\" (0x%x) logged in\n", userName, userId);

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

	_printf("user \"%s\" (0x%x) logged out\n", userInfo->userName.c_str(), userInfo->userId);

	ret = userInfo->finalize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	delete userInfo;

	return SCE_OK;
}