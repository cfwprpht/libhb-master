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

#include <sampleutil.h>
#include "printf.h"
#include "ps4_file.h"
#include "ps4_directory.h"
#include "console.h"
#include "syscalls.h"
#include "swiss_knife.h"
#include "file_info.h"
#include "defines.h"
#include "logger.h"

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
	if (fd) fclose(fd);
	delete path;
	delete filename;
	delete pathto;
}

// Checks if a File or Directory Exists.
bool LibHomebrew::PS4IO::PS4File::Exists(const char *path) {
	// Is pointer or string emtpy, return false.
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;

	// Try to open the file.
	FILE *fd = fopen(path, "rb");

	// Did we got a pointer ?
	if (!fd) return false;
	fclose(fd);
	return true;
}

// Checks if a Directory Exists.
bool LibHomebrew::PS4IO::PS4File::Exists(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	FILE *fd = fopen(path, "rb");
	if (!fd) return false;
	fclose(fd);
	return true;
}

// Remove files from a directory
bool LibHomebrew::PS4IO::PS4File::Remove(const char *source) {
	if (source == nullptr) return false;
	else if (source[0] == '\0') return false;

	if (!Exists(source)) {
		if (!PS4Dir::Exists(source)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4File] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is folder not file.\n");
			_printf("[PS4File] Source path is folder not file.\n");
		}
		return false;
	}
	if (Sys::unlink(source) != SCE_OK) {
		if (verbose) Console::WriteLine("Couldn't delete file.\n%s", source);
		_printf("[PS4File] Couldn't delete file.\n%s", source);
		return false;

	}
	if (verbose) Console::WriteLine("%s deletet.", source);
	_printf("[PS4File] %s deletet.", source);
	return true;
}

// Remove files from a directory
void LibHomebrew::PS4IO::PS4File::Remove(char **source) {
	if (source == nullptr) return;
	else if (sizeof(source) / sizeof(*source) == 0) return;

	for (int i = 0; i < sizeof(source) / sizeof(*source); i++) {
		if (!Exists(source[i])) {
			if (!PS4Dir::Exists(source[i])) {
				if (verbose) Console::WriteLine("Source path does not exist.\n");
				_printf("[PS4File] Source path does not exist.\n");
			} else {
				if (verbose) Console::WriteLine("Source path is folder not file.\n");
				_printf("[PS4File] Source path is folder not file.\n");
			} continue;
		}
		if (Sys::unlink(source[i]) != SCE_OK) {
			if (verbose) Console::WriteLine("Couldn't delete file.\n%s", source[i]);
			_printf("[PS4File] Couldn't delete file.\n%s", source[i]);
			continue;
		}
		if (verbose) Console::WriteLine("%s deletet.", source[i]);
		_printf("[PS4File] %s deletet.", source[i]);
	}
}

// Remove a empty directory
bool LibHomebrew::PS4IO::PS4File::Remove(void) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;
	else if (fd != nullptr) {
		if (_verbose) Console::WriteLine("Error removing folder: is opened.");
		_printf("[PS4File] Error removing folder: is opened.");
		return false;
	}

	if (!Exists(path)) {
		if (_verbose) Console::WriteLine("Source path does not exist or data is folder and not file.\n");
		_printf("[PS4File] Source path does not exist or data is folder and not file.\n");
		return false;
	}
	if (Sys::unlink(path) != SCE_OK) {
		if (_verbose) Console::WriteLine("Couldn't delete folder.\n%s", path);
		_printf("[PS4File] Couldn't delete folder.\n%s", path);
		return false;
	}
	if (_verbose) Console::WriteLine("%s deletet.", path);
	_printf("[PS4File] %s deletet.", path);
	return true;
}

// Copy a Directory.
bool LibHomebrew::PS4IO::PS4File::Copy(const char *source, const char *destination) {
	// Are Strings ok ?
	if (*source == '\0' || *destination == '\0') return false;
	else if (source[0] == '\0' || destination[0] == '\0') return false;

	// Are paths Ok ?
	if (!PS4Dir::Exists(source)) {
		if (!Exists(source)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4File] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4File] Source path is file not folder.\n");
		}
		return false;
	}
	if (!PS4Dir::Exists(destination)) {
		if (!Exists(destination)) {
			if (verbose) Console::WriteLine("Destination path does not exist.\n");
			_printf("[PS4File] Destination path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Destination path is file not folder.\n");
			_printf("[PS4File] Destination path is file not folder.\n");
		}
		return false;
	}

	// Write out the file we copy.
	if (verbose) Console::WriteLine("Dumping: %s\n", source);
	_printf("[PS4File] Dumping: %s\n", source);

	// Open source for reading.
	FILE *fs = fopen(source, "rb");
	if (fs) {
		// Open Destination for writting.
		FILE *fd = fopen(destination, "wb");
		if (fd) {
			// Obtain file size.
			fseek(fs, 0, SEEK_END);
			long size = ftell(fs);
			rewind(fs);

			// Initialize buffer.
			byte buff[size];

			// Copy now.
			if (fread(buff, 1, size, fs) > 0) {
				if (fwrite(buff, 1, size, fd) <= 0) {
					if (verbose) Console::WriteError("Couldn't write to file: %s", destination);
					_printf("[PS4File] Couldn't write to file: %s", destination);
					fclose(fs);
					fclose(fd);
					return false;
				}
			} else {
				if (verbose) Console::WriteError("Couldn't read from file !\n");
				_printf("[PS4File] Couldn't read from file !\n");
				fclose(fs);
				fclose(fd);
				return false;
			}
		} else {
			if (verbose) Console::WriteError("Couldn't open file for writting.\n");
			_printf("[PS4File] Couldn't open file for writting.\n");
			fclose(fs);
			return false;
		}
		fclose(fd);
	} else {
		if (verbose) Console::WriteError("Couldn't open file for reading.\n");
		_printf("[PS4File] Couldn't open file for reading.\n");
		return false;
	}
	fclose(fs);
	return true;
}

// Copy a Directory.
bool LibHomebrew::PS4IO::PS4File::Copy(const char *destination) {
	if (*path == '\0' || *destination == '\0') return false;
	else if (path[0] == '\0' || destination[0] == '\0') return false;
	else if (fd != nullptr) {
		if (_verbose) Console::WriteLine("Error copying file: source path is opened.");
		_printf("[PS4File] Error copying file: source path is opened.");
		return false;
	}

	// Are paths Ok ?
	if (!PS4Dir::Exists(path)) {
		if (!Exists(path)) {
			if (verbose) Console::WriteLine("Source path does not exist.\n");
			_printf("[PS4File] Source path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Source path is file not folder.\n");
			_printf("[PS4File] Source path is file not folder.\n");
		}
		return false;
	}
	if (!PS4Dir::Exists(destination)) {
		if (!Exists(destination)) {
			if (verbose) Console::WriteLine("Destination path does not exist.\n");
			_printf("[PS4File] Destination path does not exist.\n");
		} else {
			if (verbose) Console::WriteLine("Destination path is file not folder.\n");
			_printf("[PS4File] Destination path is file not folder.\n");
		}
		return false;
	}

	// Write out the file we copy.
	if (verbose) Console::WriteLine("Dumping: %s\n", path);
	_printf("[PS4File] Dumping: %s\n", path);

	// Open source for reading.
	FILE *fs = fopen(path, "rb");
	if (fs) {
		// Open Destination for writting.
		FILE *fd = fopen(destination, "wb");
		if (fd) {
			// Obtain file size.
			fseek(fs, 0, SEEK_END);
			long size = ftell(fs);
			rewind(fs);

			// Initialize buffer.
			byte buff[size];

			// Copy now.
			if (fread(buff, 1, size, fs) > 0) {
				if (fwrite(buff, 1, size, fd) <= 0) {
					if (verbose) Console::WriteError("Couldn't write to file: %s", destination);
					_printf("[PS4File] Couldn't write to file: %s", destination);
					fclose(fs);
					fclose(fd);
					return false;
				}
			} else {
				if (verbose) Console::WriteError("Couldn't read from file !\n");
				_printf("[PS4File] Couldn't read from file !\n");
				fclose(fs);
				fclose(fd);
				return false;
			}
		} else {
			if (verbose) Console::WriteError("Couldn't open file for writting.\n");
			_printf("[PS4File] Couldn't open file for writting.\n");
			fclose(fs);
			return false;
		}
		fclose(fd);
	} else {
		if (verbose) Console::WriteError("Couldn't open file for reading.\n");
		_printf("[PS4File] Couldn't open file for reading.\n");
		return false;
	}
	fclose(fs);
	return true;
}

// Rename a Directory or a File, or Move them from one place to a other.
bool LibHomebrew::PS4IO::PS4File::Move(const char *old, const char *_new) {
	if (old == nullptr || _new == nullptr) return false;
	else if (old[0] == '\0' || _new[0] == '\0') return false;
	if (!Exists(old)) {
		if (verbose) Console::WriteLine("Source path aka 'old' does not exist.\n");
		_printf("[PS4File] Source path aka 'old' does not exist.\n");
		return false;
	}
	if (Exists(_new)) {
		if (verbose) Console::WriteLine("Destination path aka 'new' does already exist.\n");
		_printf("[PS4File] Destination path aka 'new' does already exist.\n");
		return false;
	}
	Sys::rename(old, _new);
	return true;
}

// Rename a Directory or a File, or Move them from one place to a other.
bool LibHomebrew::PS4IO::PS4File::Move(const char *_new) {
	if (path == nullptr || _new == nullptr) return false;
	else if (path[0] == '\0' || _new[0] == '\0') return false;
	else if (fd != nullptr) {
		if (_verbose) Console::WriteLine("Error moving file: is opened.");
		_printf("[PS4File] Error moving file: is opened.");
		return false;
	}

	if (!Exists(path)) {
		if (_verbose) Console::WriteLine("Source path aka 'old' does not exist.\n");
		_printf("[PS4File] Source path aka 'old' does not exist.\n");
		return false;
	}
	if (Exists(_new)) {
		if (_verbose) Console::WriteLine("Destination path aka 'new' does already exist.\n");
		_printf("[PS4File] Destination path aka 'new' does already exist.\n");
		return false;
	}
	Sys::rename(path, _new);
	return true;
}

// Open a Directory, using Syscall open().
FILE *LibHomebrew::PS4IO::PS4File::Open(const char *file, IO mode, IO type) {
	if (file == nullptr) return nullptr;

	// Resolve options.
	if (mode == IO::Read) {
		if (type == IO::BYTE) {
			arg += "rb";
		} else arg += "r";
	} else if (mode == IO::Write) {
		if (type == IO::BYTE) {
			arg += "wb";
		} else arg += "w";
	} else if (mode == IO::Append) {
		if (type == IO::BYTE) {
			arg += "ab";
		} else arg += "a";
	} else if (mode == IO::ReadWrite) {
		if (type == IO::BYTE) {
			arg += "rb+";
		} else arg += "r+";
	} else if (mode == IO::WriteRead) {
		if (type == IO::BYTE) {
			arg += "wb+";
		} else arg += "w+";
	} else if (mode == IO::ReadAppend) {
		if (type == IO::BYTE) {
			arg += "ab+";
		} else arg += "a+";
	}
	return fopen(file, arg.c_str());
}

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4File::Open(IO mode, IO type) {
	if (path == nullptr) return false;
	else if (fd != nullptr) {
		if (_verbose) Console::WriteLine("File is already opened.");
		_printf("[PS4File] File is already opened.");
		return false;
	}

	// Resolve options.
	if (mode == IO::Read) {
		if (type == IO::BYTE) {
			_arg += "rb";
		} else _arg += "r";
	} else if (mode == IO::Write) {
		if (type == IO::BYTE) {
			_arg += "wb";
		} else _arg += "w";
	} else if (mode == IO::Append) {
		if (type == IO::BYTE) {
			_arg += "ab";
		} else _arg += "a";
	} else if (mode == IO::ReadWrite) {
		if (type == IO::BYTE) {
			_arg += "rb+";
		} else _arg += "r+";
	} else if (mode == IO::WriteRead) {
		if (type == IO::BYTE) {
			_arg += "wb+";
		} else _arg += "w+";
	} else if (mode == IO::ReadAppend) {
		if (type == IO::BYTE) {
			_arg += "ab+";
		} else _arg += "a+";
	}

	fd = fopen(path, arg.c_str());
	if (fd == nullptr) {
		if (_verbose) Console::WriteLine("Couldn't open File.\n");
		_printf("[PS4File] Couldn't open File.\n");
		return false;
	}
	return true;
}

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4File::Create(const char *file) {
	if (file == nullptr) return false;
	FILE *fp = fopen(file, "w+");
	if (fp != nullptr) {
		fclose(fp);
		return true;
	}	
	return false;
}

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4File::Create(void) {
	if (path == nullptr) return false;
	FILE *fd = fopen(path, "w+");
	if (fd != nullptr) {
		fclose(fd);
		return true;
	}
	return false;
}

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4File::Read(const char *file, IO mode, IO type, void *buffer, size_t len) {
	if (file == nullptr) return false;
	String _arg;

	// Resolve options.
	if (mode == IO::Read) {
		if (type == IO::BYTE) {
			_arg += "rb";
		} else _arg += "r";
	} else if (mode == IO::ReadAppend) {
		if (type == IO::BYTE) {
			_arg += "ab+";
		} else _arg += "a+";
	}

	FILE *_fp = fopen(file, _arg.c_str());
	if (_fp != nullptr) {
		int result = fread(buffer, 1, len, _fp);
		if (result != len) {
			if (verbose) Console::WriteLine("Could not read data from file.");
			_printf("[PS4File] Could not read data from file.");
			fclose(_fp);
			return false;
		}
		fclose(_fp);
		return true;
	}
	if (verbose) Console::WriteLine("Could not open file for reading.\nFile: %s", file);
	_printf("[PS4File] Could not open file for reading.\nFile: %s", file);
	return false;
}

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4File::Read(FILE *file, void *buffer, size_t len) {
	if (file == nullptr) return false;
	int result = fread(buffer, 1, len, file);
	if (result != len) {
		if (verbose) Console::WriteLine("Could not read data from file.");
		_printf("[PS4File] Could not read data from file.");
		return false;
	}
	return true;
}

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4File::Read(void *buffer, size_t len) {
	if (path == nullptr) return false;
	if (fd == nullptr) return false;
	int result = fread(buffer, 1, len, fd);
	if (result != len) {
		if (_verbose) Console::WriteLine("Could not read data from file.");
		_printf("[PS4File] Could not read data from file.");
		return false;
	}
	return true;
}

// Read a Line from a file.
bool LibHomebrew::PS4IO::PS4File::ReadLine(const char *path, char *buffer) {
	if (path == nullptr) return false;

	FILE *fp = fopen(path, "r");
	if (fp) {
		if (fgets(buffer, sizeof(buffer), fp) == nullptr) return false;
		fclose(fp);
		return true;
	}	
	return false;
}

// Read all lines from a file stream.
bool LibHomebrew::PS4IO::PS4File::ReadAllLines(const char *path, char *buffer) {
	if (path == nullptr) {
		if (verbose) Console::WriteError("Error path is null.\n");
		_printf("[PS4File] Error path is null");
		return false;
	}
	if (sizeof(buffer) / sizeof(*buffer) == 0) {
		if (verbose) Console::WriteError("Array is empty.\n");
		_printf("[PS4File] Array is empty.\n");
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
		_printf("[PS4File] Couldn't open file.\n");
		return false;
	}
	return true;
}

// Read a Line from a file stream.
bool LibHomebrew::PS4IO::PS4File::ReadLine(char *buffer) {
	if (path == nullptr) return false;
	if (fd == nullptr) return false;
	if (fgets(buffer, sizeof(buffer), fd) == nullptr) return false;
	return true;
}

// Read all lines from a file stream.
bool LibHomebrew::PS4IO::PS4File::ReadAllLines(char *buffer) {
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

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4File::Write(const char *file, IO mode, IO type, const void *buffer, size_t len) {
	if (file == nullptr) return false;
	String _arg;

	// Resolve options.
	if (mode == IO::Read) {
		if (type == IO::BYTE) {
			_arg += "rb";
		}
		else _arg += "r";
	}
	else if (mode == IO::Write) {
		if (type == IO::BYTE) {
			_arg += "wb";
		}
		else _arg += "w";
	}
	else if (mode == IO::Append) {
		if (type == IO::BYTE) {
			_arg += "ab";
		}
		else _arg += "a";
	}
	else if (mode == IO::ReadWrite) {
		if (type == IO::BYTE) {
			_arg += "rb+";
		}
		else _arg += "r+";
	}
	else if (mode == IO::WriteRead) {
		if (type == IO::BYTE) {
			_arg += "wb+";
		}
		else _arg += "w+";
	}
	else if (mode == IO::ReadAppend) {
		if (type == IO::BYTE) {
			_arg += "ab+";
		}
		else _arg += "a+";
	}

	FILE *_fp = fopen(file, _arg.c_str());
	if (_fp != nullptr) {
		int result = fwrite(buffer, 1, len, _fp);
		if (result != len) {
			if (verbose) Console::WriteLine("Could not write data to file.");
			_printf("[PS4File] Could not write data to file.");
			fclose(_fp);
			return false;
		}
		fclose(_fp);
		return true;
	}
	if (verbose) Console::WriteLine("Could not open file for writting.\nFile: %s", file);
	_printf("[PS4File] Could not open file for writting.\nFile: %s", file);
	return false;
}

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4File::Write(FILE *file, const void *buffer, size_t len) {
	if (file == nullptr) return false;
	int result = fwrite(buffer, 1, len, file);
	if (result != len) {
		if (verbose) Console::WriteLine("Could not write data to file.");
		_printf("[PS4File] Could not write data to file.");
		return false;
	}
	return true;
}

// Open a Directory, using Syscall open().
bool LibHomebrew::PS4IO::PS4File::Write(const void *buffer, size_t len) {
	if (path == nullptr) return false;
	if (fd == nullptr) return false;
	int result = fwrite(buffer, 1, len, fd);
	if (result != len) {
		if (_verbose) Console::WriteLine("Could not write data to file.");
		_printf("[PS4File] Could not write data to file.");
		return false;
	}
	return true;
}

// Writting Lines to a file.
bool LibHomebrew::PS4IO::PS4File::WriteLine(const char *path, const char *message, ...) {
	if (path == nullptr) return false;
	FILE *fp = fopen(path, "a");
	if (fp) {
		va_list args;
		va_start(args, message);
		vfprintf(fp, message, args);
		va_end(args);
		fclose(fp);
		return true;
	} else {
		Console::WriteError("Couldn't open file for writting: \n%s\n", path);
		_printf("[PS4File] Couldn't open file for writting: \n%s\n", path);
	}
	return false;
}

// Writting Lines to a opened file Stream.
bool LibHomebrew::PS4IO::PS4File::WriteLine(const char *message, ...) {
	if (path == nullptr) return false;
	if (fd == nullptr) return false;
	va_list args;
	va_start(args, message);
	vfprintf(fd, message, args);
	va_end(args);
	return true;
}

// Closing a Directory, using Syscall clsoe().
bool LibHomebrew::PS4IO::PS4File::Close(FILE *_fd) {
	if (_fd == nullptr) return false;
	int result = fclose(_fd);
	if (result != SCE_OK) {
		if (verbose) Console::WriteLine("Couldn't close File Stream.");
		_printf("[PS4File] Couldn't close File Stream.");
		return false;
	}
	_fd = nullptr;
	return true;
}

// Closing a File Stream, using Syscall close().
bool LibHomebrew::PS4IO::PS4File::Close(void) {
	if (fd == nullptr) {
		if (_verbose) Console::WriteLine("File not opened.");
		_printf("[PS4File] File not opened.");
		return false;
	}

	int result = fclose(fd);
	if (result != SCE_OK) {
		if (_verbose) Console::WriteLine("Couldn't close file: %s", filename);
		_printf("[PS4File] Couldn't close file: %s", filename);
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