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

#include "file_info.h"
#include "swiss_knife.h"
#include "ps4_file.h"
#include "ps4_directory.h"

using namespace LibHomebrew::Loot;

bool LibHomebrew::PS4IO::FileInfo::verbose = false;

// Instance Initializer.
LibHomebrew::PS4IO::FileInfo::FileInfo(const char *_path) {
	path     = strdup(_path);
	name     = SwissKnife::GetName((char *)_path);
	absolute = SwissKnife::GetAbsolutePs4Path(_path);
	exists   = SwissKnife::PathExists(path);
	isfile   = PS4File::Exists(path);
	isdir    = PS4Dir::Exists(path);
	if (!isdir) len = SwissKnife::FileSize(_path);
	else len = 0;
}

// Instance Deinitializer.
LibHomebrew::PS4IO::FileInfo::~FileInfo(void) { delete path; delete name; delete absolute; }

// Get the path for this file.
char *LibHomebrew::PS4IO::FileInfo::Path(void) { return path; }

// Get the name of this file.
char *LibHomebrew::PS4IO::FileInfo::Name(void) { return name; }

// Returns file existens.
bool LibHomebrew::PS4IO::FileInfo::Exists(void) { return exists; }

// Returns the type for this file.
bool LibHomebrew::PS4IO::FileInfo::isFile(void) { return isfile; }

// Returns the type for this file.
bool LibHomebrew::PS4IO::FileInfo::isDir(void) { return isdir; }

// Get the File length. Note: If the file is a directory, then the size will be 0.
long LibHomebrew::PS4IO::FileInfo::Length(void) { return len; }

// Set Console output.
void LibHomebrew::PS4IO::FileInfo::setVerbose(bool state) { verbose = state; }

// Get Console output condition.
bool LibHomebrew::PS4IO::FileInfo::Verbose(void) { return verbose; }

// Get PS4 comfort absolute path.
char *LibHomebrew::PS4IO::FileInfo::AbsolutePath(void) { return absolute; }

// Get the path for this file.
String LibHomebrew::PS4IO::FileInfo::Paths(void) {
	String _path(path);
	return _path;
}

// Get the name of this file.
String LibHomebrew::PS4IO::FileInfo::Names(void) {
	String _name(name);
	return _name;
}

// Get PS4 comfort absolute path.
String LibHomebrew::PS4IO::FileInfo::AbsolutePaths(void) {
	String _absolute(absolute);
	return _absolute;
}