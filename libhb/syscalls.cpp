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

#include "_types.h"
#include "syscall.h"
#include "syscalls.h"

ssize_t Sys::read(int fd, void *buf, size_t nbyte) { return syscall(3, fd, buf, nbyte); }
ssize_t Sys::write(int fd, const void *buf, size_t count) { return syscall(4, fd, buf, count); }
int Sys::open(const char *path, int flags, int mode) { return syscall(5, path, flags, mode); }
int Sys::close(int fd) { return syscall(6, fd); }
long Sys::waitpid(pid_t pid, int *stat_address, int options) { return syscall(7, pid, stat_address, options); }
int Sys::create(const char *pathname, int mode) { return syscall(8, pathname, mode); }
int Sys::link(const char *oldname, const char *newname) { return syscall(9, oldname, newname); }
int Sys::unlink(const char *pathname) { return syscall(10, pathname); }
int Sys::kexec(void* func, void *user_arg) { return syscall(11, func, user_arg); }
int Sys::getpid() { return syscall(20); }
int Sys::mount(const char *type, const char *dir, int flags, void *data) { return syscall(21, type, dir, flags, data); }
int Sys::unmount(const char *dir, int flags) { return syscall(22, dir, flags); }
int Sys::getuid() { return syscall(24); }
int Sys::kill(int pid, int signum) { return syscall(37, pid, signum); }
int Sys::ioctl(int fd, unsigned long com, void *data) { return syscall(54, fd, com, data); }
int Sys::readlink(const char *path, char *buf, int bufsiz) { return syscall(58, path, buf, bufsiz); }
int Sys::msync(void *addr, size_t len, int flags) { return syscall(65, addr, len, flags); }
int Sys::munmap(void *addr, size_t len) { return syscall(73, addr, len); }
int Sys::mprotect(void *addr, size_t len, int prot) { return syscall(74, addr, len, prot); }
int Sys::fchown(int fd, int uid, int gid) { return syscall(123, fd, uid, gid); }
int Sys::fchmod(int fd, int mode) { return syscall(124, fd, mode); }
int Sys::rename(const char *oldpath, const char *newpath) { return syscall(128, oldpath, newpath); }
int Sys::mkdir(const char *pathname, mode_t mode) { return syscall(136, pathname, mode); }
int Sys::rmdir(const char *path) { return syscall(137, path); }
int Sys::stat(const char *path, struct stat *sb) { return syscall(188, path, sb); }
int Sys::fstat(int fd, struct stat *sb) { return syscall(189, fd, sb); }
int Sys::mlock(void *addr, size_t len) { return syscall(203, addr, len); }
int Sys::munlock(void *addr, size_t len) { return syscall(204, addr, len); }
int Sys::getdents(int fd, char *buf, size_t count) { return syscall(272, fd, buf, count); }
int Sys::nmount(struct iovec *iov, uint32_t niov, int flags) { return syscall(378, iov, niov, flags); }
void *Sys::mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) { return reinterpret_cast<void *>(syscall(477, addr, len, prot, flags, fd, offset)); }
off_t Sys::lseek(int fd, off_t offset, int origin) { return syscall(478, fd, offset, origin); }
int Sys::fstatat(int fd, const char *path, struct stat *buf, int flag) { return syscall(493, fd, path, buf, flag); }
int Sys::registryCommand(int command) { return syscall(532, command); }
int Sys::createEventFlag(const char *name) { return syscall(538, name); }
int Sys::destroyEventFlag(int eventFlag) { return syscall(539, eventFlag); }
int Sys::getMemoryInfo(void *address, struct memoryRegionInfo *destination) { return syscall(547, address, destination); }
int Sys::mutexInit(const char *name, unsigned int attributes) { return syscall(557, name, attributes); }
int Sys::mutexDestroy(int mutex) { return syscall(558, mutex); }
int Sys::getOtherMemoryInfo(void *address, int nextMatchIfUnmapped, struct otherMemoryRegionInfo *destination) { return syscall(572, address, nextMatchIfUnmapped, destination); }
int Sys::unknownResourceCreate(const char *name) { return syscall(574, name); }
int Sys::unknownResourceDestroy(int unknownResource) { return syscall(575, unknownResource); }
int Sys::getFunctionAddressByName(int loadedModuleID, char *name, void *destination) { return syscall(591, loadedModuleID, name, destination); }
int Sys::getLoadedModules(int *destination, int max, int *count) { return syscall(592, destination, max, count); }
int Sys::getModuleInfo(int loadedModuleID, ModuleInfo *destination) {
	destination->size = sizeof(*destination);
	return syscall(593, loadedModuleID, destination);
}
int Sys::loadModule(const char *name, int *idDestination) { return syscall(594, name, 0, idDestination, 0); }
int Sys::unloadModule(int id) { return syscall(595, id, 0, 0); }
int Sys::getSandboxDirectory(char *destination, int *length) { return syscall(602, 0, destination, length); }