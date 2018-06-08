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
int LibHomebrew::Loot::Proc::Freedom(void) {
	struct thread td;
	return Sys::kexec((void *)&unjail, &td);
}

// Enable SUDO. DevKit ID and Allow Map of SELF patch.
int LibHomebrew::Loot::Proc::EnableSUDO(void) {
	struct thread td;
	return Sys::kexec((void *)&enableSUDO, &td);
}

// Disable SUDO.
int LibHomebrew::Loot::Proc::DisableSUDO(void) {
	struct thread td;
	return Sys::kexec((void *)&disableSUDO, &td);
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

// Unjail, root, full access and more for this process.
int LibHomebrew::Loot::Proc::Freedom500(void) {
	struct thread td;
	return Sys::kexec((void *)&unjail500, &td);
}

// Unjail, root, full access and more for this process.
int LibHomebrew::Loot::Proc::Freedom505(void) {
	struct thread td;
	return Sys::kexec((void *)&unjail505, &td);
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
