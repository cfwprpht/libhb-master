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

#include "ps4_process.h"
#include "console.h"
#include "syscalls.h"
#include "ps4_file.h"
#include "file_info.h"
#include "logger.h"
#include "ps4_elf_loader.h"
#include <system_service.h>

extern "C" {
#include "unjail.h"
}

using namespace LibHomebrew::PS4IO;

bool LibHomebrew::Loot::Proc::verbose = false;

// Load and Execute a other elf located within this application path.
// Swapps Application with the current running one. 
bool LibHomebrew::Loot::Proc::SysLoadExec(const char *path, char *const *argv) {
	if (path == nullptr) return false;
	if (path[0] == '\0') return false;
	
	int ret = 1;
	ret = sceSystemServiceLoadExec(path, argv);
	if (ret == SCE_OK) return true;
	return false;
}

// Load and Execute a other elf from any path.
int LibHomebrew::Loot::Proc::UserLoadExec(const char *path, void *arg1, void *arg2) {
	if (path == nullptr) return false;
	if (path[0] == '\0') return false;

	int (*ptr)(void *args1, void *args2);
	static char buf[1048576];

	FILE* elf = fopen(path, "rb");
	if (!elf) { Logger::Debug("[PS4-Process] Couldn't load elf from path : %s\n", path); return -1; }
	fread(buf, 1, sizeof(buf), elf);
	ptr = reinterpret_cast<int (*)(void *, void *)>(ElfLoader::LoadImage(buf, sizeof(buf)));
	if (!ptr) return -1;

	Logger::Debug("[PS4-Process] Executing Mapped ELF now.\n");
	return ptr(arg1, arg2);
}

// Load and Execute a binary from any path.
int LibHomebrew::Loot::Proc::LoadExecBin(const char *path, void *arg1, void *arg2) {
	if (path == nullptr) return false;
	if (path[0] == '\0') return false;

	// Try to open the file.
	FILE *fd = fopen(path, "r");
	if (!fd) { Logger::Debug("[PS4-Process] Couldn't load bin from path : %s\n", path); return -1; }

	// Load it.
	byte *buffer;
	fseek(fd, 0, SEEK_END);
	long len = ftell(fd);
	rewind(fd);
	buffer = (byte *)malloc(len);
	fread(buffer, 1, len, fd);
	fclose(fd);

	// Setup memory.
	char *execute = (char *)Sys::mmap(NULL, len, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
	if (!execute) { Logger::Debug("[PS4-Process] Can't allocate memory\n"); return -1; }

	memset(execute, 0x0, len);
	memcpy(execute, buffer, len);

	// Run.
	int (*func)(void *, void *) = reinterpret_cast<int (*)(void *, void *)>(execute);

	Logger::Debug("[PS4-Process] Executing Mapped Binary now.\n");
	return func(arg1, arg2);
}

// Kernel Execute aka Syscall 11 with path argument.
int LibHomebrew::Loot::Proc::Kexec(const char *path) {
	if (path == nullptr) return 1;
	if (path[0] == '\0') return 1;

	void *buffer;
	FileInfo info(path);
	PS4File file(path);
	if (file.Exists()) {
		if (file.Open(IO::Read, IO::BYTE)) {
			buffer = malloc(info.Length());
			if (!file.Read(buffer, info.Length())) {
				if (verbose) Console::WriteError("Kexec file load error.\n");
				return 1;
			}
			file.Close();
		} else return 1;
	} else return 1;

	struct thread td;
	int result = Sys::kexec((void *)buffer, &td);
	free(buffer);
	return result;
}

// Kernel Execute aka Syscall 11 with buffer argument.
int LibHomebrew::Loot::Proc::Kexec(void *buffer) {
	if (buffer == nullptr) return 1;
	if (sizeof(buffer) == 0) return 1;
	struct thread td;
	return Sys::kexec((void *)buffer, &td);
}

// Unjail, root, full access and more for this process.
int LibHomebrew::Loot::Proc::Freedom(void) {
	struct thread td;
	return Sys::kexec((void *)&unjail, &td);
}

// Enable SUDO. DevKit ID and Allow Map of SELF patch.
int LibHomebrew::Loot::Proc::EnableMMAPSelf(void) {
	struct thread td;
	return Sys::kexec((void *)&enableMMAPSelf, &td);
}

// Disable SUDO.
int LibHomebrew::Loot::Proc::DisableMMAPSelf(void) {
	struct thread td;
	return Sys::kexec((void *)&disableMMAPSelf, &td);
}

// Disable Process ASLR.
int LibHomebrew::Loot::Proc::DisableProcASLR(void) {
	struct thread td;
	return Sys::kexec((void *)&disableProcessASLR, &td);
}

// Permanet enable the Browser on non Activated Consoles.
int LibHomebrew::Loot::Proc::EnableBrowser(void) {
	struct thread td;
	return Sys::kexec((void *)&enableBrowser, &td);
}

// Reboot the Console.
void LibHomebrew::Loot::Proc::Reboot(void) {
	int evf = syscall(540, "SceSysCoreReboot");
	syscall(546, evf, 0x4000, 0);
	syscall(541, evf);
	syscall(37, 1, 30);
}

// Set verbose.
void LibHomebrew::Loot::Proc::Verbose(bool state) { verbose = state; }