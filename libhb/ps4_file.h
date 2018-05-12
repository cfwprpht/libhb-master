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
	namespace PS4IO {
		enum IO {
			Read,
			Write,
			ReadWrite,
			WriteRead,
			Append,
			ReadAppend,
			Text,
			Byte,
		};

		class PS4File {
		private:
			bool _verbose;
			char *path;
			char *filename;
			char *pathto;
			static String arg;
			String _arg;
			FILE *fd;
			int _fd;

		public:
			static bool verbose;

			PS4File(const char *path);
			~PS4File();
			static bool Exists(const char *source);
			bool Exists(void);
			static bool isFile(const char *source);
			static bool Copy(const char *source, const char *destination);
			bool Copy(const char *destination);
			static bool Create(const char *dir);
			bool Create(void);
			static bool Create2(const char *dir);
			bool Create2(void);
			static bool Remove(const char *source);
			bool Remove(void);
			static bool Move(const char *source, const char *_new);
			bool Move(const char *_new);
			static int Open(const char *file, IO mode);
			bool Open(IO mode);
			static FILE *Open2(const char *file, IO mode, IO type);
			bool Open2(IO mode, IO type);
			static bool Read(int file, void *buffer, size_t len);
			static bool Read(const char *file, void *buffer, size_t len, IO mode);
			bool Read(void *buffer, size_t len);
			static bool Read2(FILE *file, void *buffer, size_t len);
			static bool Read2(const char *file, IO mode, IO type, void *buffer, size_t len);
			bool Read2(void *buffer, size_t len);
			static bool ReadLine2(const char *path, char *buffer);
			static bool ReadAllLines2(const char *path, char *buffer);
			bool ReadLine2(char *buffer);
			bool ReadAllLines2(char *buffer);
			static bool Write(int file, const void *buffer, size_t len);
			static bool Write(const char *file, const void *buffer, size_t len, IO mode);
			bool Write(const void *format, size_t len);
			static bool Write2(FILE *file, const void *buffer, size_t len);
			static bool Write2(const char *file, IO mode, IO type, const void *buffer, size_t len);
			bool Write2(const void *buffer, size_t len);
			static bool WriteLine2(const char *path, const char *message, ...);
			bool WriteLine2(const char *message, ...);
			static bool Close(int file);
			bool Close(void);
			static bool Close2(FILE *file);
			bool Close2(void);
			static void Verbose(bool state);
			void Verbose();
			char *Path(void);
			char *FileName(void);
			char *PathTo(void);
		};
	}
}