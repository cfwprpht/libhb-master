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

#include <sampleutil.h>
#include <sstream>
#include <sys/dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Display defination */
#define DISPLAY_WIDTH	1920
#define DISPLAY_HEIGHT	1080

/* Application infos */
#define TITLE_STR           ""
#define VERSION_STR         ""
#define APP_STR             "EMPTY_FILL_ME_UP"
#define SDK_350

/*E Definitions about charactor displayed */
#define CHAR_COL			84
#define CHAR_LINE			25

#define CHAR_N		    	0.040
#define CHAR_S		    	0.030
#define MARGIN_X			0.024
#define MARGIN_Y			0.010
#define HEADER_Y			0.000
#define FOOTER_Y			0.940
#define CHAR_X_INTERVAL		(1.00 - MARGIN_X*2) / CHAR_COL
#define CHAR_Y_INTERVAL		(1.00 - MARGIN_Y*2) / CHAR_LINE

#define LINE_BOTTOM_Y (DISPLAY_HEIGHT - CHAR_Y_INTERVAL)
#define ALIGN_RIGHT_X_POS(str) \
    (MARGIN_X + (DISPLAY_WIDTH - (strlen(str) + 1) * CHAR_X_INTERVAL))

/*E Definitions of button code */
#define BTN_CODE_ENTER		'x'
#define BTN_CODE_BACK		'o'

//The maximum number of character string
#define TEXT_MAX_LENGTH 20

/*E Definitions of the colors */
namespace ssg     = sce::SampleUtil::Graphics;
namespace ssi     = sce::SampleUtil::Input;
namespace vecmath = sce::Vectormath::Simd::Aos;

/* Some better read able definations */
typedef vecmath::Vector2         Position;
typedef vecmath::Vector2         Size;
typedef vecmath::Vector2_arg     size;
typedef vecmath::Vector4         Color;
typedef vecmath::Vector4_arg     color;
typedef std::string              String;
typedef std::vector<std::string> StringList;
typedef unsigned char            byte;
typedef struct tm                Time;
typedef struct dirent            DirEntry;
typedef int FILEC;

/* Importend paths */
static String pathToLibc;
static String pathToLibSceFios2;

/* Color definations */
static const vecmath::Vector4_arg BLACK               = vecmath::Vector4(0.0, 0.0, 0.0, 1.0);
static const vecmath::Vector4_arg WHITE               = vecmath::Vector4(1.0, 1.0, 1.0, 1.0);
static const vecmath::Vector4_arg YELLOW              = vecmath::Vector4(1.0, 1.0, 0.0, 1.0);
static const vecmath::Vector4_arg GREEN_YELLOW        = vecmath::Vector4(0.5, 1.0, 0.0, 1.0);
static const vecmath::Vector4_arg ORANGE              = vecmath::Vector4(1.0, 0.5, 0.0, 1.0);
static const vecmath::Vector4_arg GREEN               = vecmath::Vector4(0.0, 1.0, 0.0, 1.0);
static const vecmath::Vector4_arg DARK_GREEN          = vecmath::Vector4(0.0, 0.5, 0.0, 1.0);
static const vecmath::Vector4_arg LIGHT_GREEN         = vecmath::Vector4(0.5, 1.0, 0.5, 1.0);
static const vecmath::Vector4_arg RED                 = vecmath::Vector4(1.0, 0.0, 0.0, 1.0);
static const vecmath::Vector4_arg RED2                = vecmath::Vector4(1.0, 0.3, 0.0, 1.0);
static const vecmath::Vector4_arg LIGHT_BLUE          = vecmath::Vector4(0.0, 1.0, 1.0, 1.0);
static const vecmath::Vector4_arg BLUE                = vecmath::Vector4(0.0, 0.0, 1.0, 1.0);
static const vecmath::Vector4_arg PURPLE              = vecmath::Vector4(1.0, 0.0, 1.0, 1.0);
static const vecmath::Vector4_arg CYAN                = vecmath::Vector4(0.0, 1.0, 1.0, 1.0);
static const vecmath::Vector4_arg LIGHT_GRAY          = vecmath::Vector4(0.0, 0.0, 0.0, 0.17);
static const vecmath::Vector4_arg GRAY                = vecmath::Vector4(0.5, 0.5, 0.5, 1.0);
static const vecmath::Vector4_arg DARK_GRAY           = vecmath::Vector4(0.3, 0.3, 0.3, 1.0);
static const vecmath::Vector4_arg DARK_GRAY_SHATTERED = vecmath::Vector4(0.3, 0.3, 0.3, 0.7);
static const vecmath::Vector4_arg PINK                = vecmath::Vector4(0, 0, 0, 0);

static const uint32_t _YELLOW = 0xFFFF11FF;
static const uint32_t _RED    = 0xFF0000FF;
static const uint32_t _BLACK  = 0x00000000;
static const uint32_t _WHITE  = 0xFFFFFFFF;