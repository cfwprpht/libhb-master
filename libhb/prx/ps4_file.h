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

#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif

namespace LibHomebrew {
	namespace PS4IO {
		enum IO {
			Read,
			Write,
			ReadWrite,
			WriteRead,
			Append,
			ReadAppend,
			Text,
			BYTE,
		};

		class PRX_INTERFACE PS4File {
		private:
			bool _verbose;
			char *path;
			char *filename;
			char *pathto;
			static String arg;
			String _arg;
			FILE *fd;

		public:
			static bool verbose;

			PS4File(const char *path);
			~PS4File();
			static bool Exists(const char *source);
			bool Exists(void);
			static bool Copy(const char *source, const char *destination);
			bool Copy(const char *destination);
			static bool Create(const char *file);
			bool Create(void);
			bool Remove(void);
			static bool Remove(const char *source);
			static void Remove(char **source);
			static bool Move(const char *source, const char *_new);
			bool Move(const char *_new);
			static FILE *Open(const char *file, IO mode, IO type);
			bool Open(IO mode, IO type);
			static bool Read(FILE *file, void *buffer, size_t len);
			static bool Read(const char *file, IO mode, IO type, void *buffer, size_t len);
			bool Read(void *buffer, size_t len);
			static bool ReadLine(const char *path, char *buffer);
			static bool ReadAllLines(const char *path, char *buffer);
			bool ReadLine(char *buffer);
			bool ReadAllLines(char *buffer);
			static bool Write(FILE *file, const void *buffer, size_t len);
			static bool Write(const char *file, IO mode, IO type, const void *buffer, size_t len);
			bool Write(const void *buffer, size_t len);
			static bool WriteLine(const char *path, const char *message, ...);
			bool WriteLine(const char *message, ...);
			static bool Close(FILE *file);
			bool Close(void);
			static void Verbose(bool state);
			void Verbose();
			char *Path(void);
			char *FileName(void);
			char *PathTo(void);
		};
	}
}