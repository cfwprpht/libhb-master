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

#include <vector>
#include "defines.h"

namespace LibHomebrew {
	namespace PS4IO {
		class FileInfo {
		private:
			char *path;
			char *name;
			char *absolute;
			bool exists;
			bool isfile;
			bool isdir;
			static bool verbose;
			long len;

		public:
			FileInfo(const char *path);
			~FileInfo();
			char *Path(void);
			char *Name(void);
			char *AbsolutePath(void);
			String Paths(void);
			String Names(void);
			String AbsolutePaths(void);
			bool Exists(void);
			bool isFile(void);
			bool isDir(void);
			long Length(void);
			void setVerbose(bool state);
			bool Verbose(void);
		};
	}
}

typedef std::vector<LibHomebrew::PS4IO::FileInfo> FileInfoList;