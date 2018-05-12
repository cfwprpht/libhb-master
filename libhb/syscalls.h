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

#include "_types.h"
#include "syscall.h"

class Sys {
public:
	/* Module Information structure. */
	typedef struct mdlInfo_t {
		size_t size;           // 0x0
		char name[32];         // 0x8
		char padding1[0xe0];   // 0x28
		void *codeBase;        // 0x108
		unsigned int codeSize; // 0x110
		void *dataBase;        // 0x118
		unsigned int dataSize; // 0x120
		char padding2[0x3c];   // 0x124
	} ModuleInfo;

	static ssize_t read(int fd, void *buf, size_t nbyte);
	static ssize_t write(int fd, const void *buf, size_t count);
	static int open(const char *path, int flags, int mode);
	static int close(int fd);
	static long waitpid(pid_t pid, int *stat_address, int options);
	static int create(const char *pathname, int mode);
	static int link(const char *oldname, const char *newname);
	static int unlink(const char *pathname);
	static int kexec(void* func, void *user_arg);
	static off_t lseek(int fd, off_t offset, int origin);	
	static int getpid(void);
	static int mount(const char *type, const char *dir, int flags, void *data);
	static int unmount(const char *dir, int flags);
	static int getuid(void);
	static int kill(int pid, int signum);
	static int ioctl(int fd, unsigned long com, void *data);
	static int msync(void *addr, size_t len, int flags);
	static int munmap(void *addr, size_t len);
	static int mprotect(void *addr, size_t len, int prot);
	static int fchown(int fd, int uid, int gid);
	static int fchmod(int fd, int mode);
	static int rename(const char *oldpath, const char *newpath);
	static int mkdir(const char *pathname, mode_t mode);
	static int rmdir(const char *path);
	static int stat(const char *path, struct stat *sb);
	static int fstat(int fd, struct stat *sb);
	static int mlock(void *addr, size_t len);
	static int munlock(void *addr, size_t len);
	static int getdents(int fd, char *buf, size_t count);
	static void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
	static int registryCommand(int command);
	static int createEventFlag(const char *name);
	static int destroyEventFlag(int eventFlag);
	static int getMemoryInfo(void *address, struct memoryRegionInfo *destination);
	static int mutexInit(const char *name, unsigned int attributes);
	static int mutexDestroy(int mutex);
	static int getOtherMemoryInfo(void *address, int nextMatchIfUnmapped, struct otherMemoryRegionInfo *destination);
	static int unknownResourceCreate(const char *name);
	static int unknownResourceDestroy(int unknownResource);
	static int getFunctionAddressByName(int loadedModuleID, char *name, void *destination);
	static int getLoadedModules(int *destination, int max, int *count);
	static int getModuleInfo(int loadedModuleID, ModuleInfo *destination);
	static int loadModule(const char *name, int *idDestination);
	static int unloadModule(int id);
	static int getSandboxDirectory(char *destination, int *length);
};