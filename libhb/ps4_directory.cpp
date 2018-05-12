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

#include <sampleutil.h>
#include "ps4_directory.h"
#include "ps4_file.h"
#include "console.h"
#include "syscalls.h"
#include "swiss_knife.h"
#include "file_info.h"

using namespace LibHomebrew::Loot;

bool LibHomebrew::PS4IO::PS4Dir::verbose = false;

// Instance Initializer.
LibHomebrew::PS4IO::PS4Dir::PS4Dir(const char *_path) {
	if (strlen(_path) > 0) {
		path = strdup(_path);
		dirname = SwissKnife::GetName(path);
		pathto = SwissKnife::GetPath(path);
	}

	dd = 0;
	_verbose = verbose = false;
}

// Instance Initializer.
LibHomebrew::PS4IO::PS4Dir::~PS4Dir() {
	if (dd) Sys::close(dd);
	delete path;
	delete dirname;
	delete pathto;
}

// Checks if a Directory Exists.
bool LibHomebrew::PS4IO::PS4Dir::Exists(const char *path) {
	// Is pointer or string emtpy, return false.
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;

	// Try to open the directory.
	int fd = Sys::open(path, O_RDONLY | O_DIRECTORY, 0);

	// Did we got a pointer ?
	if (fd < 0) return false;

	// Close the directory.
	Sys::close(fd);
	return true;
}

// Checks if a Directory Exists.
bool LibHomebrew::PS4IO::PS4Dir::Exists(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	int fd = Sys::open(path, O_RDONLY | O_DIRECTORY, 0);
	if (fd < 0) return false;
	Sys::close(fd);
	return true;
}

/* Check if path is Dir. */
bool LibHomebrew::PS4IO::PS4Dir::isDir(const char *path) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	String _path(path);
if (_path.find("cach0:/")) { if (Sys::open(path, O_RDONLY | O_DIRECTORY, 0) < 0) return false; }
else if (Sys::open(SwissKnife::genPs4Path(path), O_RDONLY | O_DIRECTORY, 0) < 0) return false;
return true;
}

/* Check if path is Dir. */
bool LibHomebrew::PS4IO::PS4Dir::isDir(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	String _path(path);
	if (_path.find("cach0:/")) { if (Sys::open(path, O_RDONLY | O_DIRECTORY, 0) < 0) return false; }
	else if (Sys::open(SwissKnife::genPs4Path(path), O_RDONLY | O_DIRECTORY, 0) < 0) return false;
	return true;
}

/* Create a new Directory. */
bool LibHomebrew::PS4IO::PS4Dir::Mkdir(const char *path) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	if (PS4Dir::Exists(path)) { if (verbose) Console::WriteLine("Directory already exists.\n"); return false; }
	if (Sys::mkdir(path, 0777) >= 0) { if (verbose) Console::WriteLine("Directory successful created.\n"); return true; }
	if (verbose) Console::WriteLine("Could not create the Directory.\n");
	return false;
}

/* Create a new Directory. */
bool LibHomebrew::PS4IO::PS4Dir::Mkdir(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	if (PS4Dir::Exists(path)) { if (_verbose) Console::WriteLine("Directory already exists.\n"); return false; }
	if (Sys::mkdir(path, 0777) >= 0) { if (_verbose) Console::WriteLine("Directory successful created.\n"); return true; }
	if (verbose) Console::WriteLine("Could not create the Directory.\n");
	return false;
}

/* Create all directorys which does not exist, within the overloaded path. */
bool LibHomebrew::PS4IO::PS4Dir::Mkpath(const char *path) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;

	StringList list;
	if (Sys::mkdir(path, 0777) != SCE_OK) {
		list.push_back(path);
		while (true) {
			String test = SwissKnife::GetPath((char *)path);
			if (!test.compare("/") && !test.compare("")) {
				if (!SwissKnife::PathExists(test.c_str())) {
					if (Sys::mkdir(test.c_str(), 0777) != SCE_OK) list.push_back(test);
					else break;
				} else break;
			} else break;
		}
		if (list.size() > 0) {
			for (unsigned int i = list.size(); i-- > 0;) {
				if (Sys::mkdir(list.at(i).c_str(), 0777) != SCE_OK) {
					if (verbose) Console::WriteLine("Couldn't create directory: %s", list.at(i).c_str());
					return false;
				}
			}
		} else if (verbose) Console::WriteLine("Some strange error occoured.\nDir list is empty but should hold at least one entry at this point.");
	} else if (verbose) Console::WriteLine("Path created.");
	return true;
}

/* Create all directorys which does not exist, within the overloaded path. */
bool LibHomebrew::PS4IO::PS4Dir::Mkpath(char *_path) {
	if (*_path == '\0') return false;
	else if (_path[0] == '\0') return false;

	StringList list;
	if (!strcmp(_path, ".")) {
		if (Sys::mkdir(path, 0777) != SCE_OK) {			
			list.push_back(path);
			while (true) {
				String test = SwissKnife::GetPath(path);
				if (!test.compare("/") && !test.compare("")) {
					if (!SwissKnife::PathExists(test.c_str())) {
						if (Sys::mkdir(test.c_str(), 0777) != SCE_OK) list.push_back(test);
						else break;
					} else break;
				} else break;
			}
			if (list.size() > 0) {
				for (unsigned int i = list.size(); i-- > 0;) {
					if (Sys::mkdir(list.at(i).c_str(), 0777) != SCE_OK) {
						if (_verbose) Console::WriteLine("Couldn't create directory: %s", list.at(i).c_str());
						return false;
					}
				}
			} else if (_verbose) Console::WriteLine("Some strange error occoured.\nDir list is empty but should hold at least one entry at this point.");
		} else if (_verbose) Console::WriteLine("Path created.");
	} else {
		if (!PS4Dir::Exists(path)) { if (_verbose) Console::WriteLine("Source Path does not exist.\nNote: Use '.' as path to make the source path before creating sub folders within the parent tree."); return false; }
		else {
			String __path(_path);
			__path.insert(0, "/");
			if (Sys::mkdir(__path.c_str(), 0777) != SCE_OK) {
				list.push_back(__path);
				while (true) {
					String test = SwissKnife::GetPath((char *)__path.c_str());
					if (!test.compare("/") && !test.compare("")) {
						if (!SwissKnife::PathExists(test.c_str())) {
							if (Sys::mkdir(test.c_str(), 0777) != SCE_OK) list.push_back(test);
							else break;
						} else break;
					} else break;
				}
				if (list.size() > 0) {
					for (unsigned int i = list.size(); i-- > 0;) {
						if (Sys::mkdir(list.at(i).c_str(), 0777) != SCE_OK) {
							if (_verbose) Console::WriteLine("Couldn't create directory: %s", list.at(i).c_str());
							return false;
						}
					}
				} else if (_verbose) Console::WriteLine("Some strange error occoured.\nDir list is empty but should hold at least one entry at this point.");
			} else if (_verbose) Console::WriteLine("Path created.");
		}
	}
	return true;
}

/* Remove a empty directory */
bool LibHomebrew::PS4IO::PS4Dir::Remove(const char *source) {
	if (*source == '\0') return false;
	else if (source[0] == '\0') return false;
	if (!SwissKnife::PathExists(source)) { if (verbose) Console::WriteLine("Source path does not exist.\n"); return false; }
	if (!isDir(source)) { if (verbose) Console::WriteLine("Source is not a directory.\n"); return false; }
	if (Sys::rmdir(source) != SCE_OK) { if (verbose) Console::WriteLine("Couldn't delete source directory.\n%s", source); return false; }
	if (verbose) Console::WriteLine("Folder deletet.");
	return true;
}

/* Remove a empty directory */
bool LibHomebrew::PS4IO::PS4Dir::Remove(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	if (!SwissKnife::PathExists(path)) { if (_verbose) Console::WriteLine("Source path does not exist.\n"); return false; }
	if (!isDir(path)) { if (_verbose) Console::WriteLine("Source is not a directory.\n"); return false; }
	if (Sys::rmdir(path) != SCE_OK) { if (_verbose) Console::WriteLine("Couldn't delete source directory.\n%s", path); return false; }
	if (_verbose) Console::WriteLine("Folder deletet.");
	return true;
}

/* Remove a Directory and all his sub folders recursively, including files. */
bool LibHomebrew::PS4IO::PS4Dir::RemoveRecursive(const char *path) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	if (!SwissKnife::PathExists(path)) { if (verbose) Console::WriteLine("Path does not Exist.\n"); return false; }
	if (!PS4Dir::isDir(path)) { if (verbose) Console::WriteLine("Path is not a Directory.\n"); return false; }

	PS4Dir del(path);
	FileInfoList entrys = del.EntryInfoList();

	for (FileInfo entry: entrys) {
		if (entry.isFile()) { if (!PS4File::Remove(entry.Name())) return false; }
		else if (entry.isDir()) { if (!RemoveRecursive(entry.Name())) return false; }
		else if (verbose) { Console::WriteLine("Error: path to remove is not a Directory and not a File. o_O"); return false; }
	}
	if (!del.Remove()) return false;
	return true;
}

/* Remove a Directory and all his sub folders recursively, including files. */
bool LibHomebrew::PS4IO::PS4Dir::RemoveRecursive(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	if (!SwissKnife::PathExists(path)) { if (_verbose) Console::WriteLine("Path does not Exist.\n"); return false; }
	if (!PS4Dir::isDir(path)) { if (_verbose) Console::WriteLine("Path is not a Directory.\n"); return false; }

	PS4Dir del(path);
	FileInfoList entrys = del.EntryInfoList();

	for (FileInfo entry: entrys) {
		if (entry.isFile()) { if (!PS4File::Remove(entry.Name())) return false; }
		else if (entry.isDir()) { if (!RemoveRecursive(entry.Name())) return false; }
		else if (_verbose) { Console::WriteLine("Error: path to remove is not a Directory and not a File. o_O"); return false; }
	}
	if (!del.Remove()) return false;
	return true;
}

// Copy Files and Folders Recursively.
bool LibHomebrew::PS4IO::PS4Dir::CopyRecursive(const char *source, const char *destination) {
	// First check if source dir exists and if it is a dir.
	FileInfo *info = new FileInfo(source);
	if (!info->Exists() || !info->isDir()) {
		if (!info->Exists()) { if (verbose) Console::WriteLine("\nSource Directory does not exist !"); }
		else if (verbose) Console::WriteLine("\nSource path is not a Directory !");
		delete info;
		return false;
	}
	delete info;

	// Now check if destination exists and if so if it is a directory.
	info = new FileInfo(destination);
	if (info->Exists() && !info->isDir()) {
		{ if (verbose) Console::WriteLine("\nDestination path is not a Directory !"); }
		delete info;
		return false;
	}
	delete info;

	// Set up the paths and get a list of all files and folders of the source dir.
	PS4Dir src(source);
	PS4Dir dest(destination);
	FileInfoList entrys = src.EntryInfoList();

	// Check if destination dir exists and create if not.
	if (!dest.Exists()) {
		if (!dest.Mkpath(".")) {
			if (verbose) Console::WriteLine("\nCouldn't create folder: %s\n\n", dest.Path());
			return false;
		}
	}

	// Copy Files and folders recursive.
	for (FileInfo entry : entrys) {
		if (entry.isDir() && ((entry.Names().find(".") != -1) || (entry.Names().find("..") != -1))) continue;

		if (entry.isFile()) {
			String toCopy(destination);
			if (toCopy.find_last_of("/") != toCopy.size()) toCopy += "/";
			toCopy += entry.Names();
			if (!PS4File::Copy(entry.Path(), toCopy.c_str())) {
				if (verbose) Console::WriteLine("\nCouldn't copy file: %s\n\n\nto\n%s", entry.Path(), toCopy.c_str());
				return false;
			}
		}
		else if (entry.isDir()) {
			String toCopy(destination);
			if (toCopy.find_last_of("/") != toCopy.size()) toCopy += "/";
			toCopy += entry.Name();
			if (!CopyRecursive(entry.Path(), toCopy.c_str())) {
				if (verbose) Console::WriteLine("\nCouldn't copy directory:  %s\n\n\nto\n%s", entry.Path(), toCopy.c_str());
				return false;
			}
		}
	}
	return true;
}

// Copy Files and Folders Recursively.
bool LibHomebrew::PS4IO::PS4Dir::CopyRecursive(const char *destination) {
	// First check if source dir exists and if it is a dir.
	FileInfo *info = new FileInfo(path);
	if (!info->Exists() || !info->isDir()) {
		if (!info->Exists()) { if (_verbose) Console::WriteLine("\nSource Directory does not exist !"); }
		else if (_verbose) Console::WriteLine("\nSource path is not a Directory !");
		delete info;
		return false;
	}
	delete info;

	// Now check if destination exists and if so if it is a directory.
	info = new FileInfo(destination);
	if (info->Exists() && !info->isDir()) {
		{ if (_verbose) Console::WriteLine("\nDestination path is not a Directory !"); }
		delete info;
		return false;
	}
	delete info;

	// Set up the paths and get a list of all files and folders of the source dir.
	PS4Dir src(path);
	PS4Dir dest(destination);
	FileInfoList entrys = src.EntryInfoList();

	// Check if destination dir exists and create if not.
	if (!dest.Exists()) {
		if (!dest.Mkpath(".")) {
			if (_verbose) Console::WriteLine("\nCouldn't create folder: %s\n\n", dest.Path());
			return false;
		}
	}

	// Copy Files and folders recursive.
	for (FileInfo entry : entrys) {
		if (entry.isDir() && ((entry.Names().find(".") != -1) || (entry.Names().find("..") != -1))) continue;

		if (entry.isFile()) {
			String toCopy(destination);
			if (toCopy.find_last_of("/") != toCopy.size()) toCopy += "/";
			toCopy += entry.Names();
			if (!PS4File::Copy(entry.Path(), toCopy.c_str())) {
				if (_verbose) Console::WriteLine("\nCouldn't copy file: %s\n\n\nto\n%s", entry.Path(), toCopy.c_str());
				return false;
			}
		}
		else if (entry.isDir()) {
			String toCopy(destination);
			if (toCopy.find_last_of("/") != toCopy.size()) toCopy += "/";
			toCopy += entry.Name();
			if (!CopyRecursive(entry.Path(), toCopy.c_str())) {
				if (_verbose) Console::WriteLine("\nCouldn't copy directory:  %s\n\n\nto\n%s", entry.Path(), toCopy.c_str());
				return false;
			}
		}
	}
	return true;
}

/* Copy a Directory. */
bool LibHomebrew::PS4IO::PS4Dir::Copy(const char *source, const char *destination) {
	if (*source == '\0' || *destination == '\0') return false;
	else if (source[0] == '\0' || destination[0] == '\0') return false;
	if (!SwissKnife::PathExists(source)) { if (verbose) Console::WriteLine("Source path does not exist.\n"); return false; }
	if (!SwissKnife::PathExists(destination)) { if (verbose) Console::WriteLine("Destination does not exist.\n"); return false; }
	if (!isDir(source)) { if (verbose) Console::WriteLine("Source is not a directory.\n"); return false; }
	if (!isDir(destination)) { if (verbose) Console::WriteLine("Destination is not a directory"); return false; }

	// Get source dir name.
	FileInfo info(source);

	// Prepare a destination string.
	String dest(destination);
	String name(info.Name());
	dest += "/" + name + "/";

	// Copy directory.
	PS4Dir::Mkdir(dest.c_str());

	// Are ther other directories ?
	PS4Dir src(source);
	FileInfoList entrys = src.EntryInfoList();
	for (FileInfo entry : entrys) {
		if (PS4Dir::isDir(entry.Path())) {
			if (!PS4Dir::Mkdir((dest + entry.Name()).c_str())) {
				if (verbose) Console::WriteLine("Error: couldn't copy dir: %s", entry.Path());
				return false;
			}
		} else if (PS4File::isFile(entry.Path())) {
			if (!PS4File::Copy(entry.Path(), (dest + entry.Name()).c_str())) {
				if (verbose) Console::WriteLine("Error: Couln't copy file: %s", entry.Path());
				return false;
			}
		} else if (verbose) Console::WriteLine("Some error occured !\n[PS4Dir::Copy()] Path is no File and no Dir !");
	}
	return true;
}

/* Copy a Directory. */
bool LibHomebrew::PS4IO::PS4Dir::Copy(const char *destination) {
	if (*path == '\0' || *destination == '\0') return false;
	else if (path[0] == '\0' || destination[0] == '\0') return false;
	if (!SwissKnife::PathExists(path)) { if (_verbose) Console::WriteLine("Source path does not exist.\n"); return false; }
	if (!SwissKnife::PathExists(destination)) { if (_verbose) Console::WriteLine("Destination does not exist.\n"); return false; }
	if (!isDir(path)) { if (_verbose) Console::WriteLine("Source is not a directory.\n"); return false; }
	if (!isDir(destination)) { if (_verbose) Console::WriteLine("Destination is not a directory"); return false; }

	// Get source dir name.
	FileInfo info(path);

	// Prepare a destination string.
	String dest(destination);
	String name(info.Name());
	dest += "/" + name + "/";

	// Copy directory.
	PS4Dir::Mkdir(dest.c_str());

	// Are ther other directories ?
	PS4Dir src(path);
	FileInfoList entrys = src.EntryInfoList();
	for (FileInfo entry : entrys) {
		if (PS4Dir::isDir(entry.Path())) {
			if (!PS4Dir::Mkdir((dest + entry.Name()).c_str())) {
				if (_verbose) Console::WriteLine("Error: couldn't copy dir: %s", entry.Path());
				return false;
			}
		} else if (PS4File::isFile(entry.Path())) {
			if (!PS4File::Copy(entry.Path(), (dest + entry.Name()).c_str())) {
				if (_verbose) Console::WriteLine("Error: Couln't copy file: %s", entry.Path());
				return false;
			}
		} else if (_verbose) Console::WriteLine("Some error occured !\n[PS4Dir::Copy()] Path is no File and no Dir !");
	}
	return true;
}

/* Rename a Directory or a File, or Move them from one place to a other. */
bool LibHomebrew::PS4IO::PS4Dir::Move(const char *old, const char *_new) {
	if (old == nullptr || _new == nullptr) return false;
	else if (old[0] == '\0' || _new[0] == '\0') return false;
	if (!SwissKnife::PathExists(old)) { if (verbose) Console::WriteLine("Source path aka 'old' does not exist.\n"); return false; }
	if (SwissKnife::PathExists(_new)) { if (verbose) Console::WriteLine("Destination path aka 'new' does already exist.\n"); return false; }
	Sys::rename(old, _new);
	return true;
}

/* Rename a Directory or a File, or Move them from one place to a other. */
bool LibHomebrew::PS4IO::PS4Dir::Move(const char *_new) {
	if (path == nullptr || _new == nullptr) return false;
	else if (path[0] == '\0' || _new[0] == '\0') return false;
	if (!SwissKnife::PathExists(path)) { if (_verbose) Console::WriteLine("Source path aka 'old' does not exist.\n"); return false; }
	if (SwissKnife::PathExists(_new)) { if (_verbose) Console::WriteLine("Destination path aka 'new' does already exist.\n"); return false; }
	Sys::rename(path, _new);
	return true;
}

/* Open a Directory, using Syscall open(). */
int LibHomebrew::PS4IO::PS4Dir::Open(const char *directory) {
	if (directory == nullptr) return -1;
	if (!SwissKnife::PathExists(directory)) { if (verbose) Console::WriteLine("Source path does not exist.\n"); return -1; }
	if (!isDir(directory)) { if (verbose) Console::WriteLine("Source is not a directory.\n"); return -1; }
	return Sys::open(directory, O_RDONLY | O_DIRECTORY, 0);
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4Dir::Open(void) {
	if (path == nullptr) return 0;
	if (!SwissKnife::PathExists(path)) { if (verbose) Console::WriteLine("Source path does not exist.\n"); return false; }
	if (!isDir(path)) { if (verbose) Console::WriteLine("Source is not a directory.\n"); return false; }
	dd = Sys::open(path, O_RDONLY | O_DIRECTORY, 0);
	if (dd <= 0) { if (verbose) Console::WriteLine("Couldn't open directory.\n"); return false; }
	return true;
}

/* Closing a Directory, using Syscall close(). */
bool LibHomebrew::PS4IO::PS4Dir::Close(int directory) {
	if (directory == 0) return false;
	int result = Sys::close(directory);
	if (result != SCE_OK) {
		if (verbose) Console::WriteLine("Couldn't close directory.");
		return false;
	}
	return true;
}

/* Closing a Directory, using Syscall close(). */
bool LibHomebrew::PS4IO::PS4Dir::Close(void) {
	if (dd == 0) return false;
	int result = Sys::close(dd);
	if (result != SCE_OK) {
		if (_verbose) Console::WriteLine("Couldn't close directory: %s", dirname);
		return false;
	}
	dd = 0;
	return true;
}

// Set verbose (static).
void LibHomebrew::PS4IO::PS4Dir::Verbose(bool state) { verbose = state; }

// Set verbose.
void LibHomebrew::PS4IO::PS4Dir::Verbose() {
	if (_verbose) _verbose = false;
	else _verbose = true;
}

// Get Path.
char *LibHomebrew::PS4IO::PS4Dir::Path(void) { return path; }

// Get name of the directory.
char *LibHomebrew::PS4IO::PS4Dir::DirName(void) { return dirname; }

// Get path to directory. (without path name)
char *LibHomebrew::PS4IO::PS4Dir::PathTo(void) { return pathto; }

// Get a FileInfoList on the overloaded path. Note: Have to be a directory, else nothing will be done.
std::vector<LibHomebrew::PS4IO::FileInfo> LibHomebrew::PS4IO::PS4Dir::EntryInfoList(void) {
	FileInfoList list;
	if (SwissKnife::PathExists(path)) {
		if (!PS4File::isFile(path)) {			
			int dir = Sys::open(path, O_RDONLY | O_DIRECTORY, 0);
			if (dir < 0) { if (verbose) Console::WriteLine("Could not open Directory.\n"); }
			else {
				FileInfo *test = new FileInfo(path);
				DirEntry *dents;
				char dentBuff[512];
				while (Sys::getdents(dir, dentBuff, sizeof(dentBuff)) != 0) {
					dents = (DirEntry *)dentBuff;					
					while (dents->d_fileno) {
						test->~FileInfo();
						test = new FileInfo(dents->d_name);
						list.push_back((*test));
						dents = (DirEntry *)(static_cast<void *>(dents + dents->d_reclen));
					}
				}
			}
			Sys::close(dir);
			return list;
		}
	}
	return list;
}