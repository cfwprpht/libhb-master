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

namespace LibHomebrew {
	namespace Loot {
		/* Hexmap */
		constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

		class SwissKnife {
		private:
			static bool verbose;

		public:
			SwissKnife() {}
			~SwissKnife() {}
			static void Verbose(void);
			static size_t ArraySize(char *array);
			static String ToHexString(void *bytes, int len);
			static String ToHexString(byte *bytes, int len);
			static String GetTimeString(void);
			static String Hexify(String data);
			static byte *ToBytes(const char *hexString);
			static uint8_t hexCharToUint(char ch);
			static void hexStrToBin(const char *pHexStr, uint8_t *pBinBuf, size_t binBufSize);
			static char *StrEndianSwap(char *source);
			static wchar_t *GetWChar(const char *toConvert);
			static char *GetFromWChar(const wchar_t *toConvert);
			static void dub2(int num);
			static void Split(StringList *_list, char *str, const char *delim);
			static char *GetName(char *str);
			static char *GetPath(char *str);
			static char *strstrLast(const char *array, const char *find);
			static void Reverse(char *array);
			static int strNumLast(const char *str);
			static char *intostr(int num);
			static String GetUsb(void);
			static char *GetLocalIP(void);
			static char *GetSsid(void);
			static char *GetLanguage(void);
			static long FileSize(FILE *stream);
			static off_t FileSize(const char *path);			
			static bool PathExists(const char *path);
			static char *GetAbsolutePs4Path(const char *path);
			static bool isPs4Path(const char *path);
			static char *genPs4Path(const char *path);
		};
	}
}