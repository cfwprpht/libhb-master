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
*  More informations about how to load a elf in c code language here: 'https://stackoverflow.com/questions/13908276/loading-elf-file-in-c-in-user-space#'
*/

#pragma once

#include "elf.h"
#include "elf_common.h"

#ifdef LIBRARY_IMPL
#define __declspec(dllexport)
#else
#define __declspec(dllimport)
#endif

namespace LibHomebrew {
	namespace Loot {
		class ElfLoader {
		private:			
			static bool isIdentOk(Elf64_Ehdr *ehdr);
			static void Resolve(const char *sym, void *offset);

		public:
			static void Relocate(Elf64_Shdr* shdr, const Elf64_Sym* syms, const char* strings, const char* src, char* dst);
			static void *FindSym(const char* name, Elf64_Shdr* shdr, const char* strings, const char* src, char* dst);
			static void *LoadImage(char *elf_start, unsigned int size);
		};
	}
}