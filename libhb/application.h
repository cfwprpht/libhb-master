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

// Included Library stubs.
#pragma comment( lib , "libSceCommonDialog_stub_weak.a" )
#pragma comment( lib , "libSceMsgDialog_stub_weak.a" )
#pragma comment( lib , "libSceAvPlayer_stub_weak.a" )
#pragma comment( lib , "libSceNet_stub_weak.a" )

#include "fw.h"
#include <stdio.h>
#include <vectormath.h>
#include <vector>
#include <string>
#include <sampleutil.h>
#include <user_service.h>
#include <system_service.h>
#include <libsysmodule.h>
#include <common_dialog.h>
#include <message_dialog.h>
#include <video_out.h>
#include "syscall.h"
#include "syscalls.h"
#include "defines.h"
#include "tty.h"
#include "console.h"
#include "file_info.h"
#include "ps4_directory.h"
#include "ps4_process.h"
#include "swiss_knife.h"
#include "libUtil/av_util.h"
#include "libUtil/resource_manager.h"
#include "libUtil/config.h"

// LibHomebrew !
using namespace LibHomebrew;
using namespace LibHomebrew::Loot;
using namespace LibHomebrew::PS4IO;
using namespace common::Util;
using namespace common::Configuration;
namespace G = sce::SampleUtil::Graphics;

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
		int update(ssg::GraphicsContext *graphicsContext);
		void render(ssg::GraphicsContext *graphicsContext, ssg::SpriteRenderer *spriteRenderer);
		int setColor(SceUserServiceUserColor _userColor);
	};

	public:
		static Config conf;

		Application() {}
		virtual ~Application() {}
		virtual int  initialize(void);
		virtual int  finalize(void);
		virtual int  update(void);
		virtual void render(void);
		int          exec(void);		
		void         Add(void *usrLoop);
		void         AddDraw(void (*drawEvent)());
		void         RemoveDraw(void(*drawEvent)());
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
		void         BannerSize(float size);
		void         BannerColor(Color color);
		void         UseTitle(bool state);
		void         UseTime(bool state);
		void         UseBanner(bool state);
		void         MultiLine(bool state);
		void         UseScreenShot(bool state);
		void         UseVideo(bool state);
		void         Video(const char *path);
		void         Close(void);
		ssi::Button  Input(void);
		void         ClearInput(void);
		void         drawRect(float x, float y, float width, float height, Color color);
		void         fillRect(float x, float y, float width, float height, Color color);
		void         drawRect(Position pos, Size size, Color color);
		void         fillRect(Position pos, Size size, Color color);
		int          drawStringf(float x, float y, float size, Color color, const char *format, ...);
		int          drawStringf(float x, float y, Color color, const char *format, ...);
		int          drawStringf(float x, float y, float size, const char *format, ...);
		int          drawStringf(Position pos, float size, Color color, const char *format, ...);
		int          drawStringf(Position pos, Color color, const char *format, ...);
		int          drawStringf(float x, float y, const char *format, ...);
		int          drawStringf(Position pos, const char *format, ...);
		float        getCenteredPosX(int len);
		Position     CenterForm(float x, float y);
		G::GraphicsContext *Graphics(void);

	private:
		ssg::SpriteRenderer*           sprite;
		ssg::Collada::ColladaLoader*   pLoader;
		static bool                    useTitle;
		static bool                    useTime;
		static bool                    useBanner;
		static bool                    multiLine;
		static bool                    useScreenShot;
		static bool                    useCursor;
		bool                           start;
		bool                           close;
		static bool                    useVid;
		static char                    *title;
		static char                    *banner;
		static char                    *videoPath;
		static Position                titlePos;
		static Position                timePos;
		static Position                bannerPos;
		static Color                   titleColor;
		static Color                   timeColor;
		static Color                   bannerColor;
		static float                   titleSize;
		static float                   timeSize;
		static float                   bannerSize;
		static ssi::Button             input;
		std::vector<void (*)()>        drawFuncs;
		std::vector<UserInfo*>         usersInfo;
		
		int userEventHandler(SceUserServiceEvent *event);
		int onLogin(SceUserServiceUserId userId);
		int onLogout(SceUserServiceUserId userId);		
		void Play(void);		
	};
}