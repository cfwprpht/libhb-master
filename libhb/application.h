﻿/*
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

// Included Library stubs.
#pragma comment( lib , "libSceAvPlayer_stub_weak.a" )
#pragma comment( lib , "libSceNet_stub_weak.a" )
#pragma comment( lib , "libSceNetCtl_stub_weak.a" )

#include "fw.h"
#include <stdio.h>
#include <vectormath.h>
#include <vector>
#include <string>
#include <net.h>
#include <libnetctl.h>
#include <sampleutil.h>
#include <user_service.h>
#include <system_service.h>
#include <libsysmodule.h>
#include <common_dialog.h>
#include <message_dialog.h>
#include <video_out.h>
#include <sys/dirent.h>
#include "syscall.h"
#include "syscalls.h"
#include "defines.h"
#include "console.h"
#include "file_info.h"
#include "ps4_directory.h"
#include "ps4_process.h"
#include "ps4_file.h"
#include "ps4_forms.h"
#include "ps4_network.h"
#include "swiss_knife.h"
#include "logger.h"
#include "ime_dialog_outer.h"
#include "msg_dialog_outer.h"
#include "ps4_forms.h"
#include "libUtil/av_util.h"
#include "libUtil/resource_manager.h"
#include "libUtil/sound_manager.h"
#include "libUtil/config.h"

#ifdef LIBRARY_IMPL
#define __declspec(dllexport)
#else
#define __declspec(dllimport)
#endif

// LibHomebrew !
using namespace LibHomebrew;
using namespace PS4Forms;
using namespace LibHomebrew::Loot;
using namespace LibHomebrew::PS4IO;
using namespace common;
using namespace common::Util;
using namespace common::Configuration;
using namespace common::Service;
namespace G = sce::SampleUtil::Graphics;
namespace A = sce::SampleUtil::Audio;

namespace LibHomebrew {
	class Application : public sce::SampleUtil::SampleSkeleton {
	//E User information
	//J ユーザー情報
	struct UserInfo {
		UserInfo(SceUserServiceUserId _userId, const char *_userName, const SceUserServiceUserColor _userColor)
			: userId(_userId)
			, userName(_userName)
			, padContext(NULL)
		{
			setColor(_userColor);
			posX = (rand() % 1000)*0.001;
			posY = (rand() % 1000)*0.001;
		}
		~UserInfo() {}

		SceUserServiceUserId userId;
		String userName;
		Color userColor;
		float posX;
		float posY;
		float _posX;
		float _posY;
		int count;
		ssi::PadContext *padContext;

		int initialize();
		int finalize();
		int update(ssg::GraphicsContext *graphicsContext, Application *app);
		void render(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *spriteRenderer);
		int setColor(SceUserServiceUserColor _userColor);
	};

	public:
		static Config conf;
		static ResourceManager  resManager;
		static SoundManager     soundManager;
		static EventDispatcher  eventDispatcher;

		Application() {}
		virtual ~Application() { close = true; }
		virtual int  initialize(void);
		virtual int  finalize(void);
		virtual int  update(void);
		virtual void render(void);
		//int          exec(Application *app);
		int          exec(void);
		void         Title(const char *title);
		void         TitlePos(Position pos);
		void         TitlePos(float x, float y);
		void         TitleSize(float size);
		void         TitleColor(Color color);
		void         TimePos(Position pos);
		void         TimePos(float x, float y);
		void         TimeColor(Color color);
		void         TimeSize(float size);
		void         Banner(const char *banner);
		void         BannerPos(Position pos);
		void         BannerPos(float x, float y);
		void         ShaderPos(Position pos);
		void         ShaderPos(float x, float y);
		void         BannerSize(float size);
		void         BannerColor(Color color);
		void         UseTitle(bool state);
		void         UseTime(bool state);
		void         UseBanner(bool state);
		void         MultiLine(bool state);
		void         UseScreenShot(bool state);
		void         UseVideo(bool state);
		void         UseSound(bool state);
		void         UseResources(bool state);
		void         UseDebug(void);
		void         UseIme(void);
		void         UseDialog(void);
		void         Video(const char *path);
		void         Close(void);
		bool         IsClosed(void);
		ssi::Button  Input(void);
		void         setBackgroundImage(ssg::Texture *texture);
		void         ShowBgi(void);
		void         HideBgi(void);
		void         ClearInput(void);
		wchar_t      *ShowIme(wchar_t *title, wchar_t *placeholder);
		DialogResult ShowMsg(const char *msg);
		DialogResult ShowMsg(const char *msg, Buttons button, const char *button1, const char *button2);
		void         drawRect(float x, float y, float width, float height, Color color);
		void         fillRect(float x, float y, float width, float height, Color color);
		void         drawRect(Position pos, Size size, Color color);
		void         fillRect(Position pos, Size size, Color color);
		int          drawStringf(float x, float y, float size, Color color, const char *format, ...);
		int          drawStringArrayf(float x, float y, float size, Color color, const char *format, ...);
		int          drawStringf(float x, float y, Color color, const char *format, ...);
		int          drawStringf(float x, float y, float size, const char *format, ...);
		int          drawStringf(Position pos, float size, Color color, const char *format, ...);
		int          drawStringf(Position pos, Color color, const char *format, ...);
		int          drawStringf(float x, float y, const char *format, ...);
		int          drawStringf(Position pos, const char *format, ...);
		int          drawStringArrayf(Position pos, const char *format, ...);
		float        getCenteredPosX(int len);
		void         AVP_TriggerPlay(void);
		void         AVP_TriggerResume(void);
		void         AVP_TriggerPause(void);
		void         AVP_TriggerStop(void);
		G::GraphicsContext *Graphics(void);
		A::AudioContext    *Audio();
		int          Play(const char *path);
		void         AddCode(void *usrLoop);
		void         Add(void (*drawEvent)());
		void         Add(PS4Forms::Form *toAdd);
		void         Add(PS4Forms::WidgetBase *toAdd);
		void         Add(PS4Forms::PictureBox *toAdd);
		void         Add(PS4Forms::RichTextBox *toAdd);
		void         Add(PS4Forms::TextViewer *toAdd);
		void         Add(PS4Forms::HexViewer *toAdd);
		void         Remove(void (*drawEvent)());
		void         Remove(PS4Forms::Form *toRemove);
		void         Remove(PS4Forms::WidgetBase *toRemove);
		void         Remove(PS4Forms::PictureBox *toRemove);
		void         Remove(PS4Forms::RichTextBox *toAdd);
		void         Remove(PS4Forms::TextViewer *toAdd);
		void         Remove(PS4Forms::HexViewer *toAdd);
		Form         *GetFormById(String resourceName);
		WidgetBase   *GetWidgetById(String resourceName);
		PictureBox   *GetPictureById(String resourceName);
		RichTextBox  *GetRtbById(String resourceName);
		TextViewer   *GetTextViewerById(String resourceName);
		HexViewer    *GetHexViewerById(String resourceName);

	private:
		static EventDataUserInfo       data;
		ssg::SpriteRenderer*           sprite;
		ssg::Collada::ColladaLoader*   pLoader;
		static PictureBox              bgi;
		static bool                    useTitle;
		static bool                    useTime;
		static bool                    useBanner;
		static bool                    multiLine;
		static bool                    useScreenShot;
		static bool                    useCursor;
		static bool                    useResources;
		static bool                    useSound;
		static bool                    debug;
		static bool                    useIme;
		static bool                    useDialog;
		bool                           start;
		bool                           close;
		static bool                    useVid;
		static char                    *title;
		static char                    *banner;
		static char                    *videoPath;
		static Position                titlePos;
		static Position                timePos;
		static Position                bannerPos;
		static Position                shaderPos;
		static Color                   titleColor;
		static Color                   timeColor;
		static Color                   bannerColor;
		static float                   titleSize;
		static float                   timeSize;
		static float                   bannerSize;
		static ssi::Button             input;
		static wchar_t                 resultTextBuf[TEXT_MAX_LENGTH + 1];
		ImeDialogWrapper               *imeDialog;
		MsgDialogWrapper               *msgDialog;
		std::vector<void (*)()>        drawFuncs;
		std::vector<UserInfo *>        usersInfo;
		std::vector<Form *>            forms;
		std::vector<WidgetBase *>      widgets;
		std::vector<PictureBox *>      pictures;
		std::vector<RichTextBox *>     rtbs;
		std::vector<TextViewer *>      textViewer;
		std::vector<HexViewer *>       hexViewer;
		
		int userEventHandler(SceUserServiceEvent *event);
		int onLogin(SceUserServiceUserId userId);
		int onLogout(SceUserServiceUserId userId);		
		void Play(void);		
	};
}