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

#define LIBRARY_IMPL  (1)

#include "ps4_elf_loader.h"
#include "defines.h"
#include "logger.h"
#include "syscalls.h"

// We are gonna using LibHB in case of the Logger for debugging.
using namespace LibHomebrew;

// ELF Magic.
static const byte ELF_IDENT[10] = "\x7f\ELF\x02\x01\x01\x09\x00";

// Library paths.
const char *pathToLibSystemService = "/system/common/lib/libSceSystemService.sprx";
const char *pathToLibKernel = "/system/common/lib/libkernel.sprx";
const char *pathToLibKernelSys = "/system/common/lib/libkernel_sys.sprx";
const char *pathToLibKernelWeb = "/system/common/lib/libkernel_web.sprx";

// Check for ELF Magic.
bool LibHomebrew::Loot::ElfLoader::isIdentOk(Elf64_Ehdr *ehdr) {
	if (!memcmp(ehdr->e_ident, ELF_IDENT, sizeof(ELF_IDENT))) return true;
	return false;
}

// Resolve Dynamicall Symbols.
void LibHomebrew::Loot::ElfLoader::Resolve(const char *sym, void *offset) {
	static SceKernelModule handleLibkernel        = 0;
	static SceKernelModule handleLibc             = 0;
	static SceKernelModule handleLibFios2         = 0;
	static SceKernelModule handleLibSystemService = 0;

	// Load and Resolve libc.
	if (handleLibc == 0) {		
		handleLibc = sceKernelLoadStartModule(pathToLibc.c_str(), 0, NULL, 0, 0, 0);
		if (!handleLibc) {
			Logger::Debug("[ELF-Loader] Couldn't load module : libc.sprx.\n");
			Logger::Debug("%s\n", strerror(errno));
		}
	}
	int ret = sceKernelDlsym(handleLibc, sym, &offset);

	// Load and Resolve libkernel.
	if (ret != SCE_OK) {
		Logger::Debug("%s\n", strerror(errno));
		if (handleLibkernel == 0) {
			handleLibkernel = sceKernelLoadStartModule(pathToLibKernel, 0, NULL, 0, 0, 0);
			if (!handleLibkernel) {
				Logger::Debug("%s\n", strerror(errno));
				handleLibkernel = sceKernelLoadStartModule(pathToLibKernelSys, 0, NULL, 0, 0, 0);
				if (!handleLibkernel) {
					Logger::Debug("%s\n", strerror(errno));
					handleLibkernel = sceKernelLoadStartModule(pathToLibKernelWeb, 0, NULL, 0, 0, 0);
					if (!handleLibkernel) {
						Logger::Debug("[ELF-Loader] Couldn't load module : libkernel.sprx.\n");
						Logger::Debug("%s\n", strerror(errno));
					}
				}
			}
		}
	} else return;	
	ret = sceKernelDlsym(handleLibc, sym, &offset);

	// Load and Resolve libSceFios2.
	if (ret != SCE_OK) {
		Logger::Debug("%s\n", strerror(errno));
		if (handleLibFios2 == 0) {
			handleLibFios2 = sceKernelLoadStartModule(pathToLibSceFios2.c_str(), 0, NULL, 0, 0, 0);
			if (!handleLibFios2) {
				Logger::Debug("[ELF-Loader] Couldn't load module : libSceFios2.sprx.\n");
				Logger::Debug("%s\n", strerror(errno));
			}
		}
	} else return;
	ret = sceKernelDlsym(handleLibFios2, sym, &offset);

	// Load and Resolve libSceSystemService.
	if (ret != SCE_OK) {
		Logger::Debug("%s\n", strerror(errno));
		if (handleLibSystemService == 0) {
			handleLibSystemService = sceKernelLoadStartModule(pathToLibSystemService, 0, NULL, 0, 0, 0);
			if (!handleLibSystemService) {
				Logger::Debug("[ELF-Loader] Couldn't load module : libSceSystemService.sprx.\n");
				Logger::Debug("%s\n", strerror(errno));
			}
		}
	} else return;
	ret = sceKernelDlsym(handleLibSystemService, sym, &offset);
	if (ret != SCE_OK) Logger::Debug("%s\n", strerror(errno));
}

// Relocate the whole Symbol Table of a elf. (Dynamically resolving)
void LibHomebrew::Loot::ElfLoader::Relocate(Elf64_Shdr* shdr, const Elf64_Sym* syms, const char* strings, const char* src, char* dst) {
	Elf64_Rel * rel = (Elf64_Rel *)(src + shdr->sh_offset);
	int j;
	for (j = 0; j < shdr->sh_size / sizeof(Elf32_Rel); j += 1) {
		const char* sym = strings + syms[ELF64_R_SYM(rel[j].r_info)].st_name;
		switch (ELF64_R_TYPE(rel[j].r_info)) {
			case R_386_JMP_SLOT:
			case R_386_GLOB_DAT:
				Elf64_Word addr;
				Resolve(sym, &addr);
				if (!addr) Logger::Debug("[ELF-Loader]  Couldn't relocate %s !\n", sym);
				else *(Elf64_Word *)(dst + rel[j].r_offset) = addr;
				break;
		}
	}
}

// Searches for a symbol and returns his offset.
void *LibHomebrew::Loot::ElfLoader::FindSym(const char* name, Elf64_Shdr* shdr, const char* strings, const char* src, char* dst) {
	Elf64_Sym* syms = (Elf64_Sym*)(src + shdr->sh_offset);
	int i;
	for (i = 0; i < shdr->sh_size / sizeof(Elf64_Sym); i += 1) {
		if (strcmp(name, strings + syms[i].st_name) == 0) return dst + syms[i].st_value;
	}
	return NULL;
}

// Load a ELF File into memory and prepare for userland execution.
void *LibHomebrew::Loot::ElfLoader::LoadImage(char *elf_start, unsigned int size) {
	Elf64_Ehdr      *hdr = NULL;
	Elf64_Phdr      *phdr = NULL;
	Elf64_Shdr      *shdr = NULL;
	Elf64_Sym       *syms = NULL;
	char            *strings = NULL;
	char            *start = NULL;
	char            *taddr = NULL;
	void            *entry = NULL;
	int             i = 0;
	char            *exec = NULL;

	hdr = (Elf64_Ehdr *)elf_start;
	Logger::Debug("[ELF-Loader] New EHDR set.\n");

	Logger::Debug("[ELF-Loader] Checking Header...");
	if (!isIdentOk(hdr)) { Logger::Debug("Magic missmatch : Not a ELF File !\n"); return 0; }
	Logger::Debug("ok !\n");

	Logger::Debug("[ELF-Loader] Mapping Memory...");
	exec = (char *)Sys::mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if (!exec) { Logger::Debug("error : Allocating memory !\n"); return 0; }
	Logger::Debug("ok !\n");	

	// Start with clean memory.
	memset(exec, 0x0, size);
	phdr = (Elf64_Phdr *)(elf_start + hdr->e_phoff);
	Logger::Debug("[ELF-Loader] Memory cleaned.\n");

	Logger::Debug("[ELF-Loader] Mapping Programms...");
	for (i = 0; i < hdr->e_phnum; ++i) {
		if (phdr[i].p_type != PT_LOAD) continue;
		if (phdr[i].p_filesz > phdr[i].p_memsz) {
			Logger::Debug("error : p_filesz > p_memsz\n");
			Sys::munmap(exec, size);
			return 0;
		}
		if (!phdr[i].p_filesz) continue;

		// p_filesz can be smaller than p_memsz,
		// the difference is zeroe'd out.
		start = elf_start + phdr[i].p_offset;
		taddr = phdr[i].p_vaddr + exec;
		memmove(taddr, start, phdr[i].p_filesz);

		if (!(phdr[i].p_flags & PF_W)) Sys::mprotect((unsigned char *)taddr, phdr[i].p_memsz, PROT_READ); // Read-only.
		if (phdr[i].p_flags & PF_X) Sys::mprotect((unsigned char *)taddr, phdr[i].p_memsz, PROT_EXEC);    // Executable.
	}
	Logger::Debug("ok !\n");

	shdr = (Elf64_Shdr *)(elf_start + hdr->e_shoff);
	Logger::Debug("[ELF-Loader] New SHDR set.\n");

	Logger::Debug("[ELF-Loader] Remapping Tables and Entry point...");
	for (i = 0; i < hdr->e_shnum; ++i) {
		if (shdr[i].sh_type == SHT_DYNSYM) {
			syms = (Elf64_Sym*)(elf_start + shdr[i].sh_offset);
			strings = elf_start + shdr[shdr[i].sh_link].sh_offset;
			entry = FindSym("main", shdr + i, strings, elf_start, exec);
			break;
		}
	}
	Logger::Debug("done.\n");

	Logger::Debug("[ELF-Loader] Try to relocate as much as possible.\n");
	for (i = 0; i < hdr->e_shnum; ++i) {
		if (shdr[i].sh_type == SHT_REL) Relocate(shdr + i, syms, strings, elf_start, exec);
	}
	Logger::Debug("[ELF-Loader] ELF Loaded and Armed.\n");

	return entry;
}