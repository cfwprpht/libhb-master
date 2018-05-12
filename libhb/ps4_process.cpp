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

#include "ps4_process.h"
#include "console.h"
#include "syscalls.h"
#include "ps4_file.h"
#include "file_info.h"
#include <system_service.h>

extern "C" {
#include "unjail.h"
}


using namespace LibHomebrew::PS4IO;

bool LibHomebrew::Loot::Proc::verbose = false;

// Load and Execute a other elf located within this application path.
// Swapps Application with the current running one. 
bool LibHomebrew::Loot::Proc::LoadExec(const char *path, char *const *argv) {
	if (path == nullptr) return false;
	if (path[0] == '\0') return false;
	
	int ret = 1;
	ret = sceSystemServiceLoadExec(path, argv);
	if (ret == SCE_OK) return true;
	return false;
}

// Kernel Execute aka Syscall 11 with path argument.
int LibHomebrew::Loot::Proc::Kexec(const char *path) {
	if (path == nullptr) return 1;
	if (path[0] == '\0') return 1;

	void *buffer;
	FileInfo info(path);
	PS4File file(path);
	if (file.Exists()) {
		if (file.Open2(IO::Read, IO::Byte)) {
			buffer = malloc(info.Length());
			if (!file.Read2(buffer, info.Length())) {
				if (verbose) Console::WriteError("Kexec file load error.\n");
				return 1;
			}
			file.Close2();
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
int LibHomebrew::Loot::Proc::Freedom405(void) {
	struct thread td;
	return Sys::kexec((void *)&unjail405, &td);
}

// Unjail, root, full access and more for this process.
int LibHomebrew::Loot::Proc::Freedom455(void) {
	struct thread td;
	return Sys::kexec((void *)&unjail455, &td);
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