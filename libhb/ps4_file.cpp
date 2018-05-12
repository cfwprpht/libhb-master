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
#include "ps4_file.h"
#include "ps4_directory.h"
#include "console.h"
#include "syscalls.h"
#include "swiss_knife.h"
#include "file_info.h"
#include "defines.h"

using namespace LibHomebrew::Loot;

bool LibHomebrew::PS4IO::PS4File::verbose = false;
String LibHomebrew::PS4IO::PS4File::arg;

// Instance Initializer.
LibHomebrew::PS4IO::PS4File::PS4File(const char *_path) {
	if (strlen(_path) > 0) {
		path = strdup(_path);
		filename = SwissKnife::GetName(path);
		pathto = SwissKnife::GetPath(path);
	}

	fd = nullptr;
	_verbose = verbose = false;
}

// Instance Initializer.
LibHomebrew::PS4IO::PS4File::~PS4File() {
	if (_fd) Sys::close(_fd);
	delete path;
	delete filename;
	delete pathto;
}

// Checks if a Directory Exists.
bool LibHomebrew::PS4IO::PS4File::Exists(const char *path) {
	// Is pointer or string emtpy, return false.
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;

	// Try to open the directory.
	int fd = Sys::open(path, O_RDONLY, 0);

	// Did we got a pointer ?
	if (fd < SCE_OK) return false;
	else if (fd > SCE_OK) {
		// Is it a file and not a folder ?
		Sys::close(fd);
		fd = Sys::open(path, O_RDONLY | O_DIRECTORY, 0);
		if (fd > SCE_OK) {
			Sys::close(fd);
			return false;
		}
	} else return false;
	return true;
}

// Checks if a Directory Exists.
bool LibHomebrew::PS4IO::PS4File::Exists(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	_fd = Sys::open(path, O_RDONLY, 0);
	if (_fd < SCE_OK) return false;
	else if (_fd > SCE_OK) {
		// Is it a file and not a folder ?
		Sys::close(_fd);
		_fd = Sys::open(path, O_RDONLY | O_DIRECTORY, 0);
		if (_fd > SCE_OK) {
			Sys::close(_fd);
			return false;
		}
	} else return false;
	return true;
}

/* Check if path is Dir. */
bool LibHomebrew::PS4IO::PS4File::isFile(const char *path) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	if (Sys::open(path, O_RDONLY, 0) < 0) return false;
	return true;
}

/* Remove a empty directory */
bool LibHomebrew::PS4IO::PS4File::Remove(const char *source) {
	if (*source == '\0') return false;
	else if (source[0] == '\0') return false;
	if (!SwissKnife::PathExists(source)) { if (verbose) Console::WriteLine("Source path does not exist.\n"); return false; }
	if (!isFile(source)) { if (verbose) Console::WriteLine("Source is not a file.\n"); return false; }
	if (Sys::rmdir(source) != SCE_OK) { if (verbose) Console::WriteLine("Couldn't delete file.\n%s", source); return false; }
	if (verbose) Console::WriteLine("File deletet.");
	return true;
}

/* Remove a empty directory */
bool LibHomebrew::PS4IO::PS4File::Remove(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	else if (fd != nullptr) {
		if (_verbose) Console::WriteLine("Error removing file: is opened.");
		return false;
	}

	if (!SwissKnife::PathExists(path)) { if (_verbose) Console::WriteLine("Source path does not exist.\n"); return false; }
	if (!isFile(path)) { if (_verbose) Console::WriteLine("Source is not a file.\n"); return false; }
	if (Sys::rmdir(path) != SCE_OK) { if (_verbose) Console::WriteLine("Couldn't delete file.\n%s", path); return false; }
	if (_verbose) Console::WriteLine("File deletet.");
	return true;
}

/* Copy a Directory. */
bool LibHomebrew::PS4IO::PS4File::Copy(const char *source, const char *destination) {
	if (*source == '\0' || *destination == '\0') return false;
	else if (source[0] == '\0' || destination[0] == '\0') return false;
	if (!SwissKnife::PathExists(source)) { if (verbose) Console::WriteLine("Source path does not exist.\n"); return false; }
	if (!SwissKnife::PathExists(destination)) { if (verbose) Console::WriteLine("Destination does not exist.\n"); return false; }
	if (!PS4Dir::isDir(source)) { if (verbose) Console::WriteLine("Source is not a directory.\n"); return false; }
	if (!PS4Dir::isDir(destination)) { if (verbose) Console::WriteLine("Destination is not a directory"); return false; }

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
	for (FileInfo entry: entrys) {
		if (PS4Dir::isDir(entry.Path())) {
			if (!PS4Dir::Mkdir((dest + entry.Name()).c_str())) {
				if (verbose) Console::WriteLine("Error: couldn't copy dir: %s", entry.Path());
				return false;
			}
		}
		else if (PS4File::isFile(entry.Path())) {
			if (!PS4File::Copy(entry.Path(), (dest + entry.Name()).c_str())) {
				if (verbose) Console::WriteLine("Error: Couln't copy file: %s", entry.Path());
				return false;
			}
		}
		else if (verbose) Console::WriteLine("Some error occured !\n[PS4Dir::Copy()] Path is no File and no Dir !");
	}
	return true;
}

/* Copy a Directory. */
bool LibHomebrew::PS4IO::PS4File::Copy(const char *destination) {
	if (*path == '\0' || *destination == '\0') return false;
	else if (path[0] == '\0' || destination[0] == '\0') return false;
	else if (fd != nullptr) {
		if (_verbose) Console::WriteLine("Error copying file: is opened.");
		return false;
	}

	if (!SwissKnife::PathExists(path)) { if (_verbose) Console::WriteLine("Source path does not exist.\n"); return false; }
	if (!SwissKnife::PathExists(destination)) { if (_verbose) Console::WriteLine("Destination does not exist.\n"); return false; }
	if (!PS4Dir::isDir(path)) { if (_verbose) Console::WriteLine("Source is not a directory.\n"); return false; }
	if (!PS4Dir::isDir(destination)) { if (_verbose) Console::WriteLine("Destination is not a directory"); return false; }

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
	for (FileInfo entry: entrys) {
		if (PS4Dir::isDir(entry.Path())) {
			if (!PS4Dir::Mkdir((dest + entry.Name()).c_str())) {
				if (_verbose) Console::WriteLine("Error: couldn't copy dir: %s", entry.Path());
				return false;
			}
		}
		else if (PS4File::isFile(entry.Path())) {
			if (!PS4File::Copy(entry.Path(), (dest + entry.Name()).c_str())) {
				if (_verbose) Console::WriteLine("Error: Couln't copy file: %s", entry.Path());
				return false;
			}
		}
		else if (_verbose) Console::WriteLine("Some error occured !\n[PS4Dir::Copy()] Path is no File and no Dir !");
	}
	return true;
}

/* Rename a Directory or a File, or Move them from one place to a other. */
bool LibHomebrew::PS4IO::PS4File::Move(const char *old, const char *_new) {
	if (old == nullptr || _new == nullptr) return false;
	else if (old[0] == '\0' || _new[0] == '\0') return false;
	if (!SwissKnife::PathExists(old)) { if (verbose) Console::WriteLine("Source path aka 'old' does not exist.\n"); return false; }
	if (SwissKnife::PathExists(_new)) { if (verbose) Console::WriteLine("Destination path aka 'new' does already exist.\n"); return false; }
	Sys::rename(old, _new);
	return true;
}

/* Rename a Directory or a File, or Move them from one place to a other. */
bool LibHomebrew::PS4IO::PS4File::Move(const char *_new) {
	if (path == nullptr || _new == nullptr) return false;
	else if (path[0] == '\0' || _new[0] == '\0') return false;
	else if (fd != nullptr) {
		if (_verbose) Console::WriteLine("Error moving file: is opened.");
		return false;
	}

	if (!SwissKnife::PathExists(path)) { if (_verbose) Console::WriteLine("Source path aka 'old' does not exist.\n"); return false; }
	if (SwissKnife::PathExists(_new)) { if (_verbose) Console::WriteLine("Destination path aka 'new' does already exist.\n"); return false; }
	Sys::rename(path, _new);
	return true;
}

/* Open a Directory, using Syscall open(). */
int LibHomebrew::PS4IO::PS4File::Open(const char *file, IO mode) {
	if (file == nullptr) return 0;

	// Resolve options and overload.
	if (mode == IO::Read) return Sys::open(file, O_RDONLY, 0);
	else if (mode == IO::Write) return Sys::open(file, O_WRONLY | O_CREAT | O_TRUNC, 0);
	else if (mode == IO::Append) return Sys::open(file, O_WRONLY | O_CREAT | O_APPEND, 0);
	else if (mode == IO::ReadWrite) return Sys::open(file, O_RDWR, 0);
	else if (mode == IO::WriteRead) return Sys::open(file, O_RDWR | O_CREAT | O_TRUNC, 0);
	else if (mode == IO::ReadAppend) return Sys::open(file, O_RDWR | O_CREAT | O_APPEND, 0);
	return 0;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Open(IO mode) {
	if (path == nullptr) return false;
	else if (_fd != 0) {
		if (_verbose) Console::WriteLine("File is already opened.");
		return false;
	}

	// Resolve options and overload.
	if (mode == IO::Read) _fd = Sys::open(path, O_RDONLY, 0);
	else if (mode == IO::Write) _fd = Sys::open(path, O_WRONLY | O_CREAT | O_TRUNC, 0);
	else if (mode == IO::Append) _fd = Sys::open(path, O_WRONLY | O_CREAT | O_APPEND, 0);
	else if (mode == IO::ReadWrite) _fd = Sys::open(path, O_RDWR, 0);
	else if (mode == IO::WriteRead) _fd = Sys::open(path, O_RDWR | O_CREAT | O_TRUNC, 0);
	else if (mode == IO::ReadAppend) _fd = Sys::open(path, O_RDWR | O_CREAT | O_APPEND, 0);

	if (_fd != 0) { if (_verbose) Console::WriteLine("Couldn't open file.\n"); return false; }
	return true;
}

/* Open a Directory, using Syscall open(). */
FILE *LibHomebrew::PS4IO::PS4File::Open2(const char *file, IO mode, IO type) {
	if (file == nullptr) return nullptr;

	// Resolve options.
	if (mode == IO::Read) {
		if (type == IO::Byte) {
			arg += "rb";
		} else arg += "r";
	} else if (mode == IO::Write) {
		if (type == IO::Byte) {
			arg += "wb";
		} else arg += "w";
	} else if (mode == IO::Append) {
		if (type == IO::Byte) {
			arg += "ab";
		} else arg += "a";
	} else if (mode == IO::ReadWrite) {
		if (type == IO::Byte) {
			arg += "rb+";
		} else arg += "r+";
	} else if (mode == IO::WriteRead) {
		if (type == IO::Byte) {
			arg += "wb+";
		} else arg += "w+";
	} else if (mode == IO::ReadAppend) {
		if (type == IO::Byte) {
			arg += "ab+";
		} else arg += "a+";
	}
	return fopen(file, arg.c_str());
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Open2(IO mode, IO type) {
	if (path == nullptr) return false;
	else if (fd != nullptr) {
		if (_verbose) Console::WriteLine("File is already opened.");
		return false;
	}

	// Resolve options.
	if (mode == IO::Read) {
		if (type == IO::Byte) {
			_arg += "rb";
		} else _arg += "r";
	} else if (mode == IO::Write) {
		if (type == IO::Byte) {
			_arg += "wb";
		} else _arg += "w";
	} else if (mode == IO::Append) {
		if (type == IO::Byte) {
			_arg += "ab";
		} else _arg += "a";
	} else if (mode == IO::ReadWrite) {
		if (type == IO::Byte) {
			_arg += "rb+";
		} else _arg += "r+";
	} else if (mode == IO::WriteRead) {
		if (type == IO::Byte) {
			_arg += "wb+";
		} else _arg += "w+";
	} else if (mode == IO::ReadAppend) {
		if (type == IO::Byte) {
			_arg += "ab+";
		} else _arg += "a+";
	}

	fd = fopen(path, arg.c_str());
	if (fd == nullptr) { if (_verbose) Console::WriteLine("Couldn't open File.\n"); return false; }
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Create(const char *file) {
	if (file == nullptr) return false;
	int _fp = Sys::open(file, O_WRONLY | O_CREAT | O_TRUNC, 0);
	if (_fp != 0) {
		Sys::close(_fp);
		return true;
	}	
	return false;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Create(void) {
	if (path == nullptr) return false;
	int fd = Sys::open(path, O_WRONLY | O_CREAT | O_TRUNC, 0);
	if (fd != 0) {
		Sys::close(fd);
		return true;
	}
	return false;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Create2(const char *file) {
	if (file == nullptr) return false;
	FILE *fp = fopen(file, "w+");
	if (fp != nullptr) {
		fclose(fp);
		return true;
	}	
	return false;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Create2(void) {
	if (path == nullptr) return false;
	FILE *fd = fopen(path, "w+");
	if (fd != nullptr) {
		fclose(fd);
		return true;
	}
	return false;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Read(int file, void *buffer, size_t len) {
	if (file == 0) return false;
	int result = Sys::read(file, buffer, len);
	if (result != len) {
		if (verbose) Console::WriteLine("Could not read data from file.");
		return false;
	}
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Read(const char *file, void *buffer, size_t len, IO mode) {
	if (file == nullptr) return false;

	int fp;
	if (mode == IO::Read) fp = Sys::open(file, O_RDONLY, 0);
	else {
		if (verbose) Console::WriteLine("Wrong mode.\nFor reading, only 'Read' is allowed.");
		return false;
	}

	if (fp == 0) {
		if (verbose) Console::WriteLine("Could not open file for reading\nFile: %s.", file);
		return false;
	}

	int result = Sys::read(fp, buffer, len);
	if (result != len) {
		if (verbose) Console::WriteLine("Could not read data from file.");
		Sys::close(fp);
		return false;
	}
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Read(void *buffer, size_t len) {
	if (path == nullptr) return false;
	if (_fd == 0) return false;
	int result = Sys::read(_fd, buffer, len);
	if (result != len) {
		if (_verbose) Console::WriteLine("Could not read data from file.");
		return false;
	}
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Read2(const char *file, IO mode, IO type, void *buffer, size_t len) {
	if (file == nullptr) return false;
	String _arg;

	// Resolve options.
	if (mode == IO::Read) {
		if (type == IO::Byte) {
			_arg += "rb";
		} else _arg += "r";
	} else if (mode == IO::ReadAppend) {
		if (type == IO::Byte) {
			_arg += "ab+";
		} else _arg += "a+";
	}

	FILE *_fp = fopen(file, _arg.c_str());
	if (_fp != nullptr) {
		int result = fread(buffer, 1, len, _fp);
		if (result != len) {
			if (verbose) Console::WriteLine("Could not read data from file.");
			fclose(_fp);
			return false;
		}
		fclose(_fp);
		return true;
	}
	if (verbose) Console::WriteLine("Could not open file for reading.\nFile: %s", file);
	return false;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Read2(FILE *file, void *buffer, size_t len) {
	if (file == nullptr) return false;
	int result = fread(buffer, 1, len, file);
	if (result != len) {
		if (verbose) Console::WriteLine("Could not read data from file.");
		return false;
	}
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Read2(void *buffer, size_t len) {
	if (path == nullptr) return false;
	if (fd == nullptr) return false;
	int result = fread(buffer, 1, len, fd);
	if (result != len) {
		if (_verbose) Console::WriteLine("Could not read data from file.");
		return false;
	}
	return true;
}

/* Read a Line from a file. */
bool LibHomebrew::PS4IO::PS4File::ReadLine2(const char *path, char *buffer) {
	if (path == nullptr) return false;

	FILE *fp = fopen(path, "r");
	if (fp) {
		if (fgets(buffer, sizeof(buffer), fp) == nullptr) return false;
		fclose(fp);
		return true;
	}	
	return false;
}

/* Read all lines from a file stream. */
bool LibHomebrew::PS4IO::PS4File::ReadAllLines2(const char *path, char *buffer) {
	if (path == nullptr) {
		if (verbose) Console::WriteError("Error path is null.\n");
		return false;
	}
	if (sizeof(buffer) / sizeof(*buffer) == 0) {
		if (verbose) Console::WriteError("Array is empty.\n");
		return false;
	}

	FILE *fp = fopen(path, "r");
	if (fp) {
		char *ptr = buffer;
		int count, total;
		count = total = sizeof(buffer);
		while ((ptr = fgets(ptr, count, fp)) != nullptr) {
			count = count - strlen(ptr);
			ptr = &buffer[total - count];			
		}
		fclose(fp);
	} else {
		if (verbose) Console::WriteError("Couldn't open file.\n");
		return false;
	}
	return true;
}

/* Read a Line from a file stream. */
bool LibHomebrew::PS4IO::PS4File::ReadLine2(char *buffer) {
	if (path == nullptr) return false;
	if (fd == nullptr) return false;
	if (fgets(buffer, sizeof(buffer), fd) == nullptr) return false;
	return true;
}

/* Read all lines from a file stream. */
bool LibHomebrew::PS4IO::PS4File::ReadAllLines2(char *buffer) {
	if (path == nullptr) return false;
	if (fd == nullptr) return false;
	if (sizeof(buffer) / sizeof(*buffer) == 0) return false;

	char *ptr = buffer;
	int count, total;
	count = total = sizeof(buffer);
	while ((ptr = fgets(ptr, count, fd)) != nullptr) {
		count = count - (strlen(ptr));
		ptr = &buffer[total - count];

	}
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Write(int file, const void *buffer, size_t len) {
	if (file == 0) return false;
	int result = Sys::write(file, buffer, len);
	if (result != len) {
		if (verbose) Console::WriteLine("Could not write data to file.");
		return false;
	}
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Write(const char *file, const void *buffer, size_t len, IO mode) {
	if (file == nullptr) return false;

	int fp;
	if (mode == IO::Write) fp = Sys::open(file, O_WRONLY | O_CREAT | O_TRUNC, 0);
	else if (mode == IO::Append) fp = Sys::open(file, O_WRONLY | O_CREAT | O_APPEND, 0);
	else {
		if (verbose) Console::WriteLine("Wrong mode.\nFor writting either 'Write' or 'Append' are allowed.");
		return false;
	}

	if (fp == 0) {
		if (verbose) Console::WriteLine("Could not open file for writting\nFile: %s.", file);
		return false;
	}

	int result = Sys::write(fp, buffer, len);
	if (result != len) {
		if (verbose) Console::WriteLine("Could not write data to file.");
		Sys::close(fp);
		return false;
	}
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Write(const void *buffer, size_t len) {
	if (path == nullptr) return false;
	if (_fd == 0) return false;
	int result = Sys::write(_fd, buffer, len);
	if (result != len) {
		if (_verbose) Console::WriteLine("Could not write data to file.");
		return false;
	}
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Write2(const char *file, IO mode, IO type, const void *buffer, size_t len) {
	if (file == nullptr) return false;
	String _arg;

	// Resolve options.
	if (mode == IO::Read) {
		if (type == IO::Byte) {
			_arg += "rb";
		}
		else _arg += "r";
	}
	else if (mode == IO::Write) {
		if (type == IO::Byte) {
			_arg += "wb";
		}
		else _arg += "w";
	}
	else if (mode == IO::Append) {
		if (type == IO::Byte) {
			_arg += "ab";
		}
		else _arg += "a";
	}
	else if (mode == IO::ReadWrite) {
		if (type == IO::Byte) {
			_arg += "rb+";
		}
		else _arg += "r+";
	}
	else if (mode == IO::WriteRead) {
		if (type == IO::Byte) {
			_arg += "wb+";
		}
		else _arg += "w+";
	}
	else if (mode == IO::ReadAppend) {
		if (type == IO::Byte) {
			_arg += "ab+";
		}
		else _arg += "a+";
	}

	FILE *_fp = fopen(file, _arg.c_str());
	if (_fp != nullptr) {
		int result = fwrite(buffer, 1, len, _fp);
		if (result != len) {
			if (verbose) Console::WriteLine("Could not write data to file.");
			fclose(_fp);
			return false;
		}
		fclose(_fp);
		return true;
	}
	if (verbose) Console::WriteLine("Could not open file for writting.\nFile: %s", file);
	return false;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Write2(FILE *file, const void *buffer, size_t len) {
	if (file == nullptr) return false;
	int result = fwrite(buffer, 1, len, file);
	if (result != len) {
		if (verbose) Console::WriteLine("Could not write data to file.");
		return false;
	}
	return true;
}

/* Open a Directory, using Syscall open(). */
bool LibHomebrew::PS4IO::PS4File::Write2(const void *buffer, size_t len) {
	if (path == nullptr) return false;
	if (fd == nullptr) return false;
	int result = fwrite(buffer, 1, len, fd);
	if (result != len) {
		if (_verbose) Console::WriteLine("Could not write data to file.");
		return false;
	}
	return true;
}

/* Writting Lines to a file. */
bool LibHomebrew::PS4IO::PS4File::WriteLine2(const char *path, const char *message, ...) {
	if (path == nullptr) return false;
	FILE *fp = fopen(path, "a");
	if (fp) {
		va_list args;
		va_start(args, message);
		vfprintf(fp, message, args);
		va_end(args);
		fclose(fp);
		return true;
	} else Console::WriteError("Couldn't open file for writting: \n%s\n", path);	
	return false;
}

/* Writting Lines to a opened file Stream. */
bool LibHomebrew::PS4IO::PS4File::WriteLine2(const char *message, ...) {
	if (path == nullptr) return false;
	if (fd == nullptr) return false;
	va_list args;
	va_start(args, message);
	vfprintf(fd, message, args);
	va_end(args);
	return true;
}

/* Closing a Directory, using Syscall clsoe(). */
bool LibHomebrew::PS4IO::PS4File::Close(int _fd) {
	if (_fd == 0) return false;
	int result = Sys::close(_fd);
	if (result != SCE_OK) {
		if (verbose) Console::WriteLine("Couldn't close File Stream.");
		return false;
	}
	_fd = 0;
	return true;
}

/* Closing a File Stream, using Syscall close(). */
bool LibHomebrew::PS4IO::PS4File::Close(void) {
	if (_fd == 0) {
		if (_verbose) Console::WriteLine("File not opened.");
		return false;
	}

	int result = Sys::close(_fd);
	if (result != SCE_OK) {
		if (_verbose) Console::WriteLine("Couldn't close file: %s", filename);
		return false;
	}
	_fd = 0;
	return true;
}

/* Closing a Directory, using Syscall clsoe(). */
bool LibHomebrew::PS4IO::PS4File::Close2(FILE *_fd) {
	if (_fd == nullptr) return false;
	int result = fclose(_fd);
	if (result != SCE_OK) {
		if (verbose) Console::WriteLine("Couldn't close File Stream.");
		return false;
	}
	_fd = nullptr;
	return true;
}

/* Closing a File Stream, using Syscall close(). */
bool LibHomebrew::PS4IO::PS4File::Close2(void) {
	if (fd == nullptr) {
		if (_verbose) Console::WriteLine("File not opened.");
		return false;
	}

	int result = fclose(fd);
	if (result != SCE_OK) {
		if (_verbose) Console::WriteLine("Couldn't close file: %s", filename);
		return false;
	}
	fd = nullptr;
	return true;
}

// Set verbose (static).
void LibHomebrew::PS4IO::PS4File::Verbose(bool state) { verbose = state; }

// Set verbose.
void LibHomebrew::PS4IO::PS4File::Verbose() {
	if (_verbose) _verbose = false;
	else _verbose = true;
}

// Get Path.
char *LibHomebrew::PS4IO::PS4File::Path(void) { return path; }

// Get name of the file.
char *LibHomebrew::PS4IO::PS4File::FileName(void) { return filename; }

// Get path to file. (without name)
char *LibHomebrew::PS4IO::PS4File::PathTo(void) { return pathto; }