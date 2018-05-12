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
#include "file_info.h"

namespace LibHomebrew {
	namespace PS4IO {
		class PS4Dir {
		private:
			bool _verbose;
			char *path;
			char *dirname;
			char *pathto;
			int  dd;

		public:
			static bool verbose;

			PS4Dir(const char *path);
			~PS4Dir();
			static bool Exists(const char *source);
			bool Exists(void);
			static bool isDir(const char *source);
			bool isDir(void);
			static bool Copy(const char *source, const char *destination);
			bool Copy(const char *destination);
			static bool Mkdir(const char *dir);
			bool Mkdir(void);
			static bool Mkpath(const char *path);
			bool Mkpath(char *path);
			static bool Remove(const char *source);
			bool Remove(void);
			static bool RemoveRecursive(const char *path);
			bool RemoveRecursive(void);
			static bool CopyRecursive(const char *source, const char *destination);
			bool CopyRecursive(const char *destination);
			static bool Move(const char *source, const char *_new);
			bool Move(const char *_new);			
			static int Open(const char *directory);
			bool Open(void);
			static bool Close(int directory);
			bool Close(void);
			static void Verbose(bool state);
			void Verbose();
			char *Path(void);
			char *DirName(void);
			char *PathTo(void);
			std::vector<FileInfo> EntryInfoList(void);
		};
	}
}