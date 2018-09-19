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

#pragma comment( lib , "libSceFios2_stub_weak.a")

#include <sampleutil.h>
#include "printf.h"
#include "ps4_directory.h"
#include "ps4_file.h"
#include "console.h"
#include "syscalls.h"
#include "swiss_knife.h"
#include "file_info.h"
#include "logger.h"

using namespace LibHomebrew::Loot;

bool LibHomebrew::PS4IO::PS4Dir::verbose = false;

static char _dot[2] = ".";
static char dotdot[3] = "..";

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
	bool exists;
	SceFiosOp dir = sceFiosDirectoryExists(NULL, path, &exists);
	sceFiosOpDelete(dir);
	return exists;
}

// Checks if a Directory Exists.
bool LibHomebrew::PS4IO::PS4Dir::Exists(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	bool exists;
	SceFiosOp dir = sceFiosDirectoryExists(NULL, path, &exists);
	sceFiosOpDelete(dir);
	return exists;
}

// Create a new Directory.
bool LibHomebrew::PS4IO::PS4Dir::Mkdir(const char *path) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	if (PS4Dir::Exists(path)) {
		if (verbose) Console::WriteLine("Directory already exists.\n");
		_printf("[PS4Dir] Directory already exists.\n");
		return false;
	}
	if (Sys::mkdir(path, 0777) >= 0) {
		if (verbose) Console::WriteLine("Directory successful created.\n");
		_printf("[PS4Dir] Directory successful created.\n");
		return true;
	}
	if (verbose) Console::WriteLine("Could not create the Directory.\n");
	_printf("[PS4Dir] Could not create the Directory.\n");
	return false;
}

// Create a new Directory.
bool LibHomebrew::PS4IO::PS4Dir::Mkdir(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	if (PS4Dir::Exists(path)) {
		if (_verbose) Console::WriteLine("Directory already exists.\n");
		_printf("[PS4Dir] Directory already exists.\n");
		return false;
	}
	if (Sys::mkdir(path, 0777) >= 0) {
		if (_verbose) Console::WriteLine("Directory successful created.\n");
		_printf("[PS4Dir] Directory successful created.\n");
		return true;
	}
	if (verbose) Console::WriteLine("Could not create the Directory.\n");
	_printf("[PS4Dir] Could not create the Directory.\n");
	return false;
}

// Create all directorys which does not exist, within the overloaded path.
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
					if (verbose) Console::WriteLine("Couldn't create directory: %s\n", list.at(i).c_str());
					_printf("[PS4Dir] Couldn't create directory: %s\n", list.at(i).c_str());
					return false;
				}
			}
		} else {
			if (verbose) Console::WriteLine("Some strange error occoured.\nDir list is empty but should hold at least one entry at this point.\n");
			_printf("[PS4Dir] Some strange error occoured.\n[PS4Dir] Dir list is empty but should hold at least one entry at this point.\n");
		}
	} else {
		if (verbose) Console::WriteLine("Path created.\n");
		_printf("[PS4Dir] Path created.\n");
	}
	return true;
}

// Create all directorys which does not exist, within the overloaded path.
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
						if (_verbose) Console::WriteLine("Couldn't create directory: %s\n", list.at(i).c_str());
						_printf("[PS4Dir] Couldn't create directory: %s\n", list.at(i).c_str());
						return false;
					}
				}
			} else {
				if (_verbose) Console::WriteLine("Some strange error occoured.\nDir list is empty but should hold at least one entry at this point.\n");
				_printf("[PS4Dir] Some strange error occoured.\n[PS4Dir] Dir list is empty but should hold at least one entry at this point.\n");
			}
		} else {
			if (_verbose) Console::WriteLine("Path created.\n");
			_printf("[PS4Dir] Path created.\n");
		}
	} else {
		if (!PS4Dir::Exists(path)) {
			if (_verbose) Console::WriteLine("Source Path does not exist.\nNote: Use '.' as path to make the source path before creating sub folders within the parent tree.\n");
			_printf("[PS4Dir] Source Path does not exist.\n[PS4Dir] Note: Use '.' as path to make the source path before creating sub folders within the parent tree.\n");
			return false;
		} else {
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
							if (_verbose) Console::WriteLine("Couldn't create directory: %s\n", list.at(i).c_str());
							_printf("[PS4Dir] Couldn't create directory: %s\n", list.at(i).c_str());
							return false;
						}
					}
				} else {
					if (_verbose) Console::WriteLine("Some strange error occoured.\nDir list is empty but should hold at least one entry at this point.\n");
					_printf("[PS4Dir] Some strange error occoured.\n[PS4Dir] Dir list is empty but should hold at least one entry at this point.\n");
				}
			} else {
				if (_verbose) Console::WriteLine("Path created.\n");
				_printf("[PS4Dir] Path created.\n");
			}
		}
	}
	return true;
}

// Remove a empty directory
bool LibHomebrew::PS4IO::PS4Dir::Remove(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;

	if (!Exists(path)) {
		if (!PS4File::Exists(path)) {
			if (_verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4Dir] Source path does not exist.\n");
		} else {
			if (_verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4Dir] Source path is file not folder.\n");
		}
		return false;
	}
	if (Sys::rmdir(path) != SCE_OK) {
		if (_verbose) Console::WriteLine("Couldn't delete folder.\n%s\n", path);
		_printf("[PS4Dir] Couldn't delete folder.\n%s\n", path);
		return false;
	}
	if (_verbose) Console::WriteLine("%s deletet.\n", path);
	_printf("[PS4Dir] %s deletet.\n", path);
	return true;
}

// Remove a empty directory
bool LibHomebrew::PS4IO::PS4Dir::Remove(const char *source) {
	if (*source == '\0') return false;
	else if (source[0] == '\0') return false;

	if (!Exists(source)) {
		if (!PS4File::Exists(source)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4Dir] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4Dir] Source path is file not folder.\n");
		}
		return false;
	}
	if (Sys::rmdir(source) != SCE_OK) {
		if (verbose) Console::WriteLine("Couldn't delete folder.\n%s\n", source);
		_printf("[PS4Dir] Couldn't delete folder.\n%s\n", source);
		return false;
	}
	if (verbose) Console::WriteLine("%s deletet.\n", source);
	_printf("[PS4Dir] %s deletet.\n", source);
	return true;
}

// Remove a empty directory
void LibHomebrew::PS4IO::PS4Dir::Remove(char **source) {
	if (source == nullptr) return;
	else if (sizeof(source) / sizeof(*source) == 0) return;

	for (int i = 0; i < sizeof(source) / sizeof(*source); i++) {
		if (!Exists(source[i])) {
			if (!PS4File::Exists(source[i])) {
				if (verbose) Console::WriteLine("Source path does not exist.\n");
				_printf("[PS4Dir] Source path does not exist.\n");
			} else {
				if (verbose) Console::WriteLine("Source path is file not folder.\n");
				_printf("[PS4Dir] Source path is file not folder.\n");
			}
			continue;
		}
		if (Sys::rmdir(source[i]) != SCE_OK) {
			if (verbose) Console::WriteLine("Couldn't delete folder.\n%s\n", source[i]);
			_printf("[PS4Dir] Couldn't delete folder.\n%s\n", source[i]);
			continue;
		}
		if (verbose) Console::WriteLine("%s deletet.\n", source[i]);
		_printf("[PS4Dir] %s deletet.\n", source[i]);
	}
}

// Remove a Directory and all his sub folders recursively, including files.
bool LibHomebrew::PS4IO::PS4Dir::RemoveRecursive(const char *path) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;

	if (!Exists(path)) {
		if (!PS4File::Exists(path)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4Dir] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4Dir] Source path is file not folder.\n");
		}
		return false;
	}

	PS4Dir del(path);
	FileInfoList *entrys = del.EntryInfoList();

	for (FileInfo entry: *entrys) {
		if (entry.isFile()) { if (!PS4File::Remove(entry.Name())) return false; }
		else if (entry.isDir()) { if (!RemoveRecursive(entry.Name())) return false; }
		else {
			if (verbose) Console::WriteLine("Error: path to remove is not a Directory and not a File.\no_O\n");
			_printf("[PS4Dir] Error: path to remove is not a Directory and not a File.\no_O\n");
			return false;
		}
	}
	if (!del.Remove()) return false;
	return true;
}

// Remove a Directory and all his sub folders recursively, including files.
bool LibHomebrew::PS4IO::PS4Dir::RemoveRecursive(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;

	if (!Exists(path)) {
		if (!PS4File::Exists(path)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4Dir] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4Dir] Source path is file not folder.\n");
		}
		return false;
	}

	PS4Dir del(path);
	FileInfoList *entrys = del.EntryInfoList();

	for (FileInfo entry: *entrys) {
		if (entry.isFile()) { if (!PS4File::Remove(entry.Name())) return false; }
		else if (entry.isDir()) { if (!RemoveRecursive(entry.Name())) return false; }
		else {
			if (_verbose) Console::WriteLine("Error: path to remove is not a Directory and not a File.\no_O\n");
			_printf("[PS4Dir] Error: path to remove is not a Directory and not a File.\no_O\n");
			return false;

		}
	}
	if (!del.Remove()) return false;
	return true;
}

// Copy Files and Folders Recursively.
bool LibHomebrew::PS4IO::PS4Dir::CopyRecursive(const char *source, const char *destination) {
	// Are Strings ok ?
	if (*source == '\0' || *destination == '\0') return false;
	else if (source[0] == '\0' || destination[0] == '\0') return false;

	// Are paths Ok ?
	if (!Exists(source)) {
		if (!PS4File::Exists(source)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4Dir] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4Dir] Source path is file not folder.\n");
		}
		return false;
	}
	if (!Exists(destination)) {
		if (!PS4File::Exists(destination)) {
			if (verbose) Console::WriteLine("Destination path does not exist.\n");
			_printf("[PS4Dir] Destination path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Destination path is file not folder.\n");
			_printf("[PS4Dir] Destination path is file not folder.\n");
		}
		return false;
	}

	// Get Directory Entries.
	char *buf, *ebuf, *cp;
	long base;
	size_t bufsize;
	int fd, nbytes;
	SceKernelStat sb;
	struct dirent *dp;

	// Open path. Since we copy recursive, destination path to open hase to be a folder.
	fd = sceKernelOpen(source, O_RDONLY | O_DIRECTORY, 0777);

	// Could we open ?
	if (fd > 0) {
		// Get file status.
		if (sceKernelFstat(fd, &sb) < 0) {
			Console::WriteError("Couldn't get file stats.\n");
			_printf("[PS4Dir] Couldn't get file stats.\n");
			return false;
		}

		// Set buffer size.
		bufsize = sb.st_size;
		if (bufsize < sb.st_blksize) bufsize = sb.st_blksize;

		// Allocate memory for our buffer.
		if ((buf = (char *)malloc(bufsize)) == NULL) {
			Console::WriteError("Can't malloc %lu bytes", (unsigned long)bufsize);
			_printf("Can't malloc %lu bytes", (unsigned long)bufsize);
			goto done;
		}

		// Loop over all entries.
		while ((nbytes = sceKernelGetdirentries(fd, buf, bufsize, &base)) > 0) {
			ebuf = buf + nbytes;
			cp = buf;
			while (cp < ebuf) {
				dp = (struct dirent *)cp;

				// Check for dot and dot-dot and continue if so.
				if (SwissKnife::Contains(dp->d_name, _dot) || SwissKnife::Contains(dp->d_name, dotdot)) { cp += dp->d_reclen; continue; }

				// Prepare Paths.
				String src = String(source);
				String dst = String(destination);

				// Check for slash on last point.
				if (src.find_last_of("/") != src.size()) src += "/";
				if (dst.find_last_of("/") != dst.size()) dst += "/";

				// Add file/folder name to it.
				src += dp->d_name;
				dst += dp->d_name;

				// Handle File or Folder.
				if (dp->d_type == DT_DIR) {
					// Create Folder in dest dir.
					if (Sys::mkdir(dst.c_str(), 0777) != SCE_OK) {
						Console::WriteError("Couldn't create Directory.\n");						
						Console::WriteError(strerror(errno));
						_printf("[PS4Dir] Couldn't create Directory.\n[PS4Dir] %s\n", strerror(errno));
						goto done;
					}

					// Call recursive.
					if (!CopyRecursive(src.c_str(), dst.c_str())) goto done;
				} else if (!PS4File::Copy(src.c_str(), dst.c_str())) goto done;
				cp += dp->d_reclen;
			}
		}

		// Check for a error.		
		if (nbytes < 0) {
			Console::WriteError("Couldn't get Directory Entries.\n");
			Console::WriteError(strerror(errno));
			_printf("[PS4Dir] Couldn't get Directory Entries.\n[PS4Dir] %s\n", strerror(errno));
			goto done;
		}
	} else {
		Console::WriteError("Couldn't open Directory :\n %s\n", source);
		_printf("[PS4Dir] Couldn't open Directory :\n %s\n", source);
		return false;
	}

	free(buf);
	return true;

done:
	free(buf);
	return false;
}

// Copy Files and Folders Recursively.
bool LibHomebrew::PS4IO::PS4Dir::CopyRecursive(const char *destination) {
	// Are Strings ok ?
	if (*path == '\0' || *destination == '\0') return false;
	else if (path[0] == '\0' || destination[0] == '\0') return false;

	// Are paths Ok ?
	if (!Exists(path)) {
		if (!PS4File::Exists(path)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4Dir] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4Dir] Source path is file not folder.\n");
		}
		return false;
	}
	if (Exists(destination)) {
		if (!PS4File::Exists(destination)) {
			if (verbose) Console::WriteLine("Destination path does not exist.\n");
			_printf("[PS4Dir] Destination path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Destination path is file not folder.\n");
			_printf("[PS4Dir] Destination path is file not folder.\n");
		}
		return false;
	}

	// Get Directory Entries.
	char *buf, *ebuf, *cp;
	long base;
	size_t bufsize;
	int fd, nbytes;
	SceKernelStat sb;
	struct dirent *dp;

	// Open path. Since we copy recursive, destination path to open hase to be a folder.
	fd = sceKernelOpen(path, O_RDONLY | O_DIRECTORY, 0777);

	// Could we open ?
	if (fd > 0) {
		// Get file status.
		if (sceKernelFstat(fd, &sb) < 0) {
			Console::WriteError("Couldn't get file stats.\n");
			_printf("[PS4Dir] Couldn't get file stats.\n");
			return false;
		}

		// Set buffer size.
		bufsize = sb.st_size;
		if (bufsize < sb.st_blksize) bufsize = sb.st_blksize;

		// Allocate memory for our buffer.
		if ((buf = (char *)malloc(bufsize)) == NULL) {
			Console::WriteError("Can't malloc %lu bytes", (unsigned long)bufsize);
			_printf("Can't malloc %lu bytes", (unsigned long)bufsize);
			goto done;
		}

		// Loop over all entries.
		while ((nbytes = sceKernelGetdirentries(fd, buf, bufsize, &base)) > 0) {
			ebuf = buf + nbytes;
			cp = buf;
			while (cp < ebuf) {
				dp = (struct dirent *)cp;

				// Check for dot and dot-dot and continue if so.
				if (SwissKnife::Contains(dp->d_name, _dot) || SwissKnife::Contains(dp->d_name, dotdot)) { cp += dp->d_reclen; continue; }

				// Prepare Paths.
				String src = String(path);
				String dst = String(destination);

				// Check for slash on last point.
				if (src.find_last_of("/") != src.size()) src += "/";
				if (dst.find_last_of("/") != dst.size()) dst += "/";

				// Add file/folder name to it.
				src += dp->d_name;
				dst += dp->d_name;

				// Handle File or Folder.
				if (dp->d_type == DT_DIR) {
					// Create Folder in dest dir.
					if (Sys::mkdir(dst.c_str(), 0777) != SCE_OK) {
						Console::WriteError("Couldn't create Directory.\n");
						Console::WriteError(strerror(errno));
						_printf("[PS4Dir] Couldn't create Directory.\n[PS4Dir] %s\n", strerror(errno));
						goto done;
					}

					// Call recursive.
					if (!CopyRecursive(src.c_str(), dst.c_str())) goto done;
				} else if (!PS4File::Copy(src.c_str(), dst.c_str())) goto done;
				cp += dp->d_reclen;
			}
		}

		// Check for a error.		
		if (nbytes < 0) {
			Console::WriteError("Couldn't get Directory Entries.\n");
			Console::WriteError(strerror(errno));
			_printf("[PS4Dir] Couldn't get Directory Entries.\n[PS4Dir] %s\n", strerror(errno));
			goto done;
		}
	} else {
		Console::WriteError("Couldn't open Directory :\n %s\n", path);
		_printf("[PS4Dir] Couldn't open Directory :\n %s\n", path);
		return false;
	}

	free(buf);
	return true;

done:
	free(buf);
	return false;
}

// Copy a Directory.
bool LibHomebrew::PS4IO::PS4Dir::Copy(const char *source, const char *destination) {
	if (*source == '\0' || *destination == '\0') return false;
	else if (source[0] == '\0' || destination[0] == '\0') return false;

	if (!Exists(source)) {
		if (!PS4File::Exists(source)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4Dir] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4Dir] Source path is file not folder.\n");
		}
		return false;
	}
	if (!Exists(destination)) {
		if (!PS4File::Exists(destination)) {
			if (verbose) Console::WriteLine("Destination path does not exist.\n");
			_printf("[PS4Dir] Destination path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Destination path is file not folder.\n");
			_printf("[PS4Dir] Destination path is file not folder.\n");
		}
		return false;
	}

	// Get Directory Entries.
	char *buf, *ebuf, *cp;
	long base;
	size_t bufsize;
	int fd, nbytes;
	SceKernelStat sb;
	struct dirent *dp;

	// Open path. Since we copy recursive, destination path to open hase to be a folder.
	fd = sceKernelOpen(source, O_RDONLY | O_DIRECTORY, 0777);

	// Could we open ?
	if (fd > 0) {
		// Get file status.
		if (sceKernelFstat(fd, &sb) < 0) {
			Console::WriteError("Couldn't get file stats.\n");
			_printf("[PS4Dir] Couldn't get file stats.\n");
			return false;
		}

		// Set buffer size.
		bufsize = sb.st_size;
		if (bufsize < sb.st_blksize) bufsize = sb.st_blksize;

		// Allocate memory for our buffer.
		if ((buf = (char *)malloc(bufsize)) == NULL) {
			Console::WriteError("Can't malloc %lu bytes", (unsigned long)bufsize);
			_printf("Can't malloc %lu bytes", (unsigned long)bufsize);
			goto done;
		}

		// Loop over all entries.
		while ((nbytes = sceKernelGetdirentries(fd, buf, bufsize, &base)) > 0) {
			ebuf = buf + nbytes;
			cp = buf;
			while (cp < ebuf) {
				dp = (struct dirent *)cp;

				// Check for dot and dot-dot and continue if so.
				if (SwissKnife::Contains(dp->d_name, _dot) || SwissKnife::Contains(dp->d_name, dotdot)) { cp += dp->d_reclen; continue; }

				// Prepare Paths.
				String src = String(source);
				String dst = String(destination);

				// Check for slash on last point.
				if (src.find_last_of("/") != src.size()) src += "/";
				if (dst.find_last_of("/") != dst.size()) dst += "/";

				// Add file/folder name to it.
				src += dp->d_name;
				dst += dp->d_name;

				// Handle File or Folder.
				if (dp->d_type == DT_DIR) {
					// Create Folder in dest dir.
					if (Sys::mkdir(dst.c_str(), 0777) != SCE_OK) {
						Console::WriteError("Couldn't create Directory.\n");
						Console::WriteError(strerror(errno));
						_printf("[PS4Dir] Couldn't create Directory.\n[PS4Dir] %s\n", strerror(errno));
						goto done;
					}
				} else if (!PS4File::Copy(src.c_str(), dst.c_str())) goto done;
				cp += dp->d_reclen;
			}
		}

		// Check for a error.		
		if (nbytes < 0) {
			Console::WriteError("Couldn't get Directory Entries.\n");
			Console::WriteError(strerror(errno));
			_printf("[PS4Dir] Couldn't get Directory Entries.\n[PS4Dir] %s\n", strerror(errno));
			goto done;
		}
	} else {
		Console::WriteError("Couldn't open Directory :\n %s\n", source);
		_printf("[PS4Dir] Couldn't open Directory :\n %s\n", source);
		return false;
	}

	free(buf);
	return true;

done:
	free(buf);
	return false;
}

// Copy a Directory.
bool LibHomebrew::PS4IO::PS4Dir::Copy(const char *destination) {
	if (*path == '\0' || *destination == '\0') return false;
	else if (path[0] == '\0' || destination[0] == '\0') return false;

	if (!Exists(path)) {
		if (!PS4File::Exists(path)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4Dir] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4Dir] Source path is file not folder.\n");
		}
		return false;
	}
	if (!Exists(destination)) {
		if (!PS4File::Exists(destination)) {
			if (verbose) Console::WriteLine("Destination path does not exist.\n");
			_printf("[PS4Dir] Destination path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Destination path is file not folder.\n");
			_printf("[PS4Dir] Destination path is file not folder.\n");
		}
		return false;
	}

	// Get Directory Entries.
	char *buf, *ebuf, *cp;
	long base;
	size_t bufsize;
	int fd, nbytes;
	SceKernelStat sb;
	struct dirent *dp;

	// Open path. Since we copy recursive, destination path to open hase to be a folder.
	fd = sceKernelOpen(path, O_RDONLY | O_DIRECTORY, 0777);

	// Could we open ?
	if (fd > 0) {
		// Get file status.
		if (sceKernelFstat(fd, &sb) < 0) {
			Console::WriteError("Couldn't get file stats.\n");
			_printf("[PS4Dir] Couldn't get file stats.\n");
			return false;
		}

		// Set buffer size.
		bufsize = sb.st_size;
		if (bufsize < sb.st_blksize) bufsize = sb.st_blksize;

		// Allocate memory for our buffer.
		if ((buf = (char *)malloc(bufsize)) == NULL) {
			Console::WriteError("Can't malloc %lu bytes", (unsigned long)bufsize);
			_printf("Can't malloc %lu bytes", (unsigned long)bufsize);
			goto done;
		}

		// Loop over all entries.
		while ((nbytes = sceKernelGetdirentries(fd, buf, bufsize, &base)) > 0) {
			ebuf = buf + nbytes;
			cp = buf;
			while (cp < ebuf) {
				dp = (struct dirent *)cp;

				// Check for dot and dot-dot and continue if so.
				if (SwissKnife::Contains(dp->d_name, _dot) || SwissKnife::Contains(dp->d_name, dotdot)) { cp += dp->d_reclen; continue; }

				// Prepare Paths.
				String src = String(path);
				String dst = String(destination);

				// Check for slash on last point.
				if (src.find_last_of("/") != src.size()) src += "/";
				if (dst.find_last_of("/") != dst.size()) dst += "/";

				// Add file/folder name to it.
				src += dp->d_name;
				dst += dp->d_name;

				// Handle File or Folder.
				if (dp->d_type == DT_DIR) {
					// Create Folder in dest dir.
					if (Sys::mkdir(dst.c_str(), 0777) != SCE_OK) {
						Console::WriteError("Couldn't create Directory.\n");
						Console::WriteError(strerror(errno));
						_printf("[PS4Dir] Couldn't create Directory.\n[PS4Dir] %s\n", strerror(errno));
						goto done;
					}
				} else if (!PS4File::Copy(src.c_str(), dst.c_str())) goto done;
				cp += dp->d_reclen;
			}
		}

		// Check for a error.		
		if (nbytes < 0) {
			Console::WriteError("Couldn't get Directory Entries.\n");
			Console::WriteError(strerror(errno));
			_printf("[PS4Dir] Couldn't get Directory Entries.\n[PS4Dir] %s\n", strerror(errno));
			goto done;
		}
	} else {
		Console::WriteError("Couldn't open Directory :\n %s\n", path);
		_printf("[PS4Dir] Couldn't open Directory :\n %s\n", path);
		return false;
	}

	free(buf);
	return true;

done:
	free(buf);
	return false;
}

// Rename a Directory or a File, or Move them from one place to a other.
bool LibHomebrew::PS4IO::PS4Dir::Move(const char *old, const char *_new) {
	if (old == nullptr || _new == nullptr) return false;
	else if (old[0] == '\0' || _new[0] == '\0') return false;
	if (!Exists(old)) {
		if (verbose) Console::WriteLine("Source path aka 'old' does not exist.\n");
		_printf("[PS4Dir] Source path aka 'old' does not exist.\n");
		return false;
	}
	if (Exists(_new)) {
		if (verbose) Console::WriteLine("Destination path aka 'new' does already exist.\n");
		_printf("[PS4Dir] Destination path aka 'new' does already exist.\n");
		return false;
	}
	Sys::rename(old, _new);
	return true;
}

// Rename a Directory or a File, or Move them from one place to a other.
bool LibHomebrew::PS4IO::PS4Dir::Move(const char *_new) {
	if (path == nullptr || _new == nullptr) return false;
	else if (path[0] == '\0' || _new[0] == '\0') return false;
	if (!Exists(path)) {
		if (_verbose) Console::WriteLine("Source path aka 'old' does not exist.\n");
		_printf("[PS4Dir] Source path aka 'old' does not exist.\n");
		return false;
	}
	if (Exists(_new)) {
		if (_verbose) Console::WriteLine("Destination path aka 'new' does already exist.\n");
		_printf("[PS4Dir] Destination path aka 'new' does already exist.\n");
		return false;
	}
	Sys::rename(path, _new);
	return true;
}

// Open a Directory, using Syscall open().
int LibHomebrew::PS4IO::PS4Dir::Open(const char *directory) {
	if (directory == nullptr) return -1;
	if (!Exists(directory)) {
		if (verbose) Console::WriteLine("Source path does not exist.\n");
		_printf("[PS4Dir] Source path does not exist.\n");
		return -1;
	}
	if (PS4File::Exists(directory)) {
		if (verbose) Console::WriteLine("Source is not a directory.\n");
		_printf("[PS4Dir] Source is not a directory.\n");
		return -1;
	}
	return sceKernelOpen(directory, O_RDONLY | O_DIRECTORY, 0700);
}

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4Dir::Open(void) {
	if (path == nullptr) return 0;
	if (!Exists(path)) {
		if (verbose) Console::WriteLine("Source path does not exist.\n");
		_printf("[PS4Dir] Source path does not exist.\n");
		return false;
	}
	if (!PS4File::Exists(path)) {
		if (verbose) Console::WriteLine("Source is not a directory.\n");
		_printf("[PS4Dir] Source is not a directory.\n");
		return false;
	}
	dd = sceKernelOpen(path, O_RDONLY | O_DIRECTORY, 0700);
	if (dd <= 0) {
		if (verbose) Console::WriteLine("Couldn't open directory.\n");
		_printf("[PS4Dir] Couldn't open directory.\n");
		return false;
	}
	return true;
}

// Closing a Directory, using Syscall close().
bool LibHomebrew::PS4IO::PS4Dir::Close(int directory) {
	if (directory == 0) return false;
	int result = Sys::close(directory);
	if (result != SCE_OK) {
		if (verbose) Console::WriteLine("Couldn't close directory.");
		_printf("[PS4Dir] Couldn't close directory.");
		return false;
	}
	return true;
}

// Closing a Directory, using Syscall close().
bool LibHomebrew::PS4IO::PS4Dir::Close(void) {
	if (dd == 0) return false;
	int result = Sys::close(dd);
	if (result != SCE_OK) {
		if (_verbose) Console::WriteLine("Couldn't close directory: %s", dirname);
		_printf("[PS4Dir] Couldn't close directory: %s", dirname);
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
std::vector<LibHomebrew::PS4IO::FileInfo> *LibHomebrew::PS4IO::PS4Dir::EntryInfoList(void) {
	FileInfoList *list = new FileInfoList();
	if (Exists(path)) {
		if (!PS4File::Exists(path)) {			
			int dir = sceKernelOpen(path, O_RDONLY | O_DIRECTORY, 0700);
			if (dir < 0) {
				if (verbose) Console::WriteLine("Could not open Directory.\n");
				_printf("[PS4Dir] Could not open Directory.\n");
			} else {
				FileInfo *test = new FileInfo(path);
				DirEntry *dents;
				char dentBuff[512];
				while (sceKernelGetdents(dir, dentBuff, sizeof(dentBuff)) != 0) {
					dents = (DirEntry *)dentBuff;					
					while (dents->d_fileno) {
						test->~FileInfo();
						test = new FileInfo(dents->d_name);
						list->push_back((*test));
						dents = (DirEntry *)(static_cast<void *>(dents + dents->d_reclen));
					}
				}
			}
			sceKernelClose(dir);
			return list;
		}
	}
	return list;
}