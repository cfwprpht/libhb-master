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

#include "swiss_knife.h"
#include "defines.h"
#include "syscalls.h"
#include "console.h"
#include "file_info.h"
#include "ps4_directory.h"
#include "ps4_file.h"
#include <net.h>
#include <libnetctl.h>
#include <sampleutil.h>
#include <system_service.h>

using namespace LibHomebrew::PS4IO;

bool  LibHomebrew::Loot::SwissKnife::verbose = false;

/*------------------------------------
	Mostly String Manipulations
--------------------------------------*/
/* Enable or Disable Verbose output */
void LibHomebrew::Loot::SwissKnife::Verbose(void) {
	if (verbose) verbose = false;
	else verbose = true;
}

/* Get Array size. */
size_t LibHomebrew::Loot::SwissKnife::ArraySize(char *array) { return sizeof(array) / sizeof(*array); }

/* Convert a void ptr byte array to a hex string. */
String LibHomebrew::Loot::SwissKnife::ToHexString(void *bytes, int len) {
	byte *data = static_cast<byte *>(bytes);
	String str(len * 2, ' ');
	for (int i = 0; i < len; ++i) {
		str[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
		str[2 * i + 1] = hexmap[data[i] & 0x0F];
	}
	return str;
}

/* Convert a unsigned char ptr byte array to a hex string. */
String LibHomebrew::Loot::SwissKnife::ToHexString(byte *data, int len) {
	String str(len * 2, ' ');
	for (int i = 0; i < len; ++i) {
		str[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
		str[2 * i + 1] = hexmap[data[i] & 0x0F];
	}
	return str;
}

/* Generate a time String and retur nit. */
String LibHomebrew::Loot::SwissKnife::GetTimeString(void) {
	char timeBuffer[16];
	memset(timeBuffer, 0, sizeof(timeBuffer));
	time_t t;
	Time *tm;
	time(&t);
	tm = localtime(&t);
	strftime(timeBuffer, sizeof(timeBuffer), "%H_%M_%S", tm);
	String result = timeBuffer;
	return result;
}

// Hexify a Hex String. Add 0x to every two hex values.
String LibHomebrew::Loot::SwissKnife::Hexify(String data) {
	// Work string.
	String hexifyed = "";

	// The Hex delimeter.
	String hex = "0x";

	// Now loop over the string and generate a new hexifeyed one.
	for (int i = 0; i < data.size(); i += 2) {
		hexifyed += hex;
		hexifyed += data[i];
		hexifyed += data[i + 1];
		if ((i + 2) != data.size()) hexifyed += " ";
	}

	// Return result.
	return hexifyed;
}

/* Convert a hex string to byte array. */
byte *LibHomebrew::Loot::SwissKnife::ToBytes(const char *hexString) {
	String toByte(hexString);
	std::istringstream hexStream(toByte);
	std::vector<byte> bytes;

	unsigned int hex;
	while (hexStream >> std::hex >> hex) bytes.push_back(hex);
	return static_cast<byte *>(bytes.data());
}

/* Generic Hex Char to Bin function. */
uint8_t LibHomebrew::Loot::SwissKnife::hexCharToUint(char ch) {
	uint8_t val = 0;
	if (isdigit(ch)) val = (ch - '0');
	else if (isupper(ch)) val = (ch - 'A' + 10);
	else val = (ch - 'a' + 10);
	return val;
}

/* Generic Hex String to Bin function. */
void LibHomebrew::Loot::SwissKnife::hexStrToBin(const char *pHexStr, uint8_t *pBinBuf, size_t binBufSize) {
	uint8_t val = 0;
	int hexStrLen = strlen(pHexStr);

	int binOffset = 0;
	for (int i = 0; i < hexStrLen; i++) {
		val |= hexCharToUint(*(pHexStr + i));
		if (i % 2 == 0) val <<= 4;
		else {
			if (pBinBuf != NULL && binOffset < binBufSize) {
				memcpy(pBinBuf + binOffset, &val, 1);
				val = 0;
			}
			binOffset++;
		}
	}

	if (val != 0 && pBinBuf != NULL && binOffset < binBufSize) memcpy(pBinBuf + binOffset, &val, 1);
	return;
}

/* Endian Swap a Hex String. */
char *LibHomebrew::Loot::SwissKnife::StrEndianSwap(char *source) {
	int z = 0;
	char *_sub;
	String sub(source);
	char *reversed = new char[strlen(source) + 1];
	for (int i = strlen(source); i > 0; i -= 2) {
		if (i < 2) reversed[z] = source[i - i];
		else {
			_sub = (char *)sub.substr(i - 2, 2).c_str();
			reversed[z] = _sub[0];
			reversed[z + 1] = _sub[1];
		}
		z += 2;
	}
	reversed[strlen(source)] = '\0';
	return strdup(reversed);
}

/* Converts a character array to a wide character array. */
wchar_t *LibHomebrew::Loot::SwissKnife::GetWChar(const char *toConvert) {
	const size_t cSize = sizeof(toConvert);
	wchar_t *convert = new wchar_t[cSize];;
	mbstowcs(convert, toConvert, cSize);
	return convert;
}

/* Converts a wide character array to a character array. */
char *LibHomebrew::Loot::SwissKnife::GetFromWChar(const wchar_t *toConvert) {
	const size_t cSize = sizeof(toConvert);
	char *convert = new char[cSize];;
	wcstombs(convert, toConvert, cSize);
	return convert;
}

/* Psydo dup2. */
void LibHomebrew::Loot::SwissKnife::dub2(int num) {
	int sockets[0x2000];
	for (int i = 0; i < 0x2000; i++) {
		sockets[i] = sceNetSocket("psydo_dub2", SCE_NET_AF_INET, SCE_NET_SOCK_STREAM, 0);
		if (sockets[i] == num) break;
	}
	for (int i = 0; i < 0x2000; i++) {
		sceNetSocketClose(sockets[i]);
		if (sockets[i + 1] == num) break;
	}
}

/* Split a string into a list based on a token. */
void LibHomebrew::Loot::SwissKnife::Split(StringList *_list, char *str, const char *delim) {
	char *token;
	token = strtok(str, delim);
	while (token != nullptr) {
		_list->push_back(token);
		token = strtok(nullptr, delim);		
	}
}

/* Trims the file name out of a path string and returns the name. */
char *LibHomebrew::Loot::SwissKnife::GetName(char *str) {
	StringList *splitted;

	if (strstr(str, "/") != nullptr) {
		if (verbose) Console::WriteLine("Couldn't find a single path slash within this string.\n");
		return nullptr;
	} else if (!strlen(str)) {
		if (verbose) Console::WriteLine("Dude how i shall split a empty string ? o_O\n");
		return nullptr;
	}

	splitted = new StringList();                                                     // Allocate memory for our list to begin with.
	Split(splitted, str, "/");                                                       // Split the string now.
	return strdup(splitted->at(splitted->size()).c_str());                           // And return the last entry of the splitted array.
}

/*
*
*
*/
char *LibHomebrew::Loot::SwissKnife::GetPath(char *str) {
	String path(str);
	String name(SwissKnife::GetName(str));
	String done;
	int pos = path.find(name + "/");
	if (pos != -1) done = path.replace(pos, (name + "/").length(), "");
	else {
		pos = path.find(name);
		if (pos != -1) done = path.replace(pos, name.length(), "");
	}
	return strdup(done.c_str());
}

/* Finds the last matching string within a char array and returns the indexer.
* If no matching result was found or the string to find is 0, it will return
* the arrays start offset. */
char *LibHomebrew::Loot::SwissKnife::strstrLast(const char *array, const char *find) {
	if (*find == '\0') return (char *)array;
	char *start, *found;
	start = (((char *)array + sizeof(array) - sizeof(find)));
	for (;;) {
		found = strstr(start, (char *)find);
		if (found == nullptr) start = start - 1;
		else break;
	}
	return start;
}

/* Reverse a Array. */
void LibHomebrew::Loot::SwissKnife::Reverse(char *array) {
	int z = 0;                                   // Start pointer.
	size_t size = ArraySize(array);              // Get array size.
	for (int i = size; i > size / 2; i--) {      // Loop over data, till the middle of the array.
		char backup;                             // Backup storage.
		backup = array[i];                       // Backup actual pointer of i.
		array[i] = array[z];                     // Swap Data of pointer z with ptr i.
		array[z] = backup;                       // Correct pointer z with backup data of i.
		z++;                                     // Coun z ptr up.
	}
}

/* Gets the last found number(s) within a string. Max 19 values. */
int LibHomebrew::Loot::SwissKnife::strNumLast(const char *str) {
	char found[19];
	char check[19];
	char num;
	bool done = false;
	char *start = (char *)str + sizeof(str);
	int strleng = strlen(str);
	for (;;) {
		for (int i = 0; i < 10; i++) {
			if ((strlen(found) == 19) || strleng == strlen(found)) { done = true; break; }
			num = (char)i;
			check[0] = '\0';
			sprintf(check, "%c%s", num, found);
			String _begin(start);
			if (_begin.find(check) != -1) {
				found[strlen(found)] = num;
				start = start - 1;
				break;
			};
			if (i == 9) done = true;
		}
		if (done) break;
	}
	if (strlen(found) == 0) return -1;

	char *endpoint;
	char temp[20];
	temp[0] = '\0';
	for (int i = 0; i < sizeof(found); i++) temp[i + 1] = found[i];
	Reverse(temp);
	for (char ptr : temp) { if (ptr == '\0') ptr = 20; }                // Ensure to not stop on the first '\0' terminater and go trough the whole array.
	return strtoimax(temp, &endpoint, 0);
}

/* Converts integer(s) to a string. */
char *LibHomebrew::Loot::SwissKnife::intostr(int num) {
	char *converted = (char *)malloc(12);
	sprintf(converted, "%d", num);
	return converted;
}

/* Get First connected Usb Device. Attemption: Will loop till a device would be connected. */
String LibHomebrew::Loot::SwissKnife::GetUsb(void) {
	if (verbose) Console::WriteLine("Waiting for USB Device...");
	String usb_path;
	while (true) {
		FILE *check = fopen("/mnt/usb0/ghse7ihbredguwezs.txt", "wb");
		if (check) {
			fclose(check);
			if (verbose) {
				Console::WriteLine("got usb0.\n");
				Console::LineBreak();
			}
			Sys::unlink("/mnt/usb0/ghse7ihbredguwezs.txt");
			usb_path = String("/mnt/usb0/");
			break;
		} else {
			check = fopen("/mnt/usb1/ghse7ihbredguwezs.txt", "wb");
			if (check) {
				fclose(check);
				if (verbose) Console::WriteLine("got usb1.\n");
				Sys::unlink("/mnt/usb1/ghse7ihbredguwezs.txt");
				usb_path = String("/mnt/usb1/");
				break;
			}
		}
	}
	return usb_path;
}

/* Get Local IP Address. */
char *LibHomebrew::Loot::SwissKnife::GetLocalIP(void) {
	char *ret = nullptr;
	int res;
	SceNetCtlInfo info;

	res = sceNetCtlInit();
	if (res < 0) return ret;		
	
	res = sceNetCtlGetInfo(SCE_NET_CTL_INFO_IP_ADDRESS, &info);
	if (res < 0) return ret;		
	ret = strdup(info.ip_address);

	sceNetCtlTerm();

	return ret;
}

/* Get SSID Name. */
char *LibHomebrew::Loot::SwissKnife::GetSsid(void) {
	char *ret = nullptr;
	int res;
	SceNetCtlInfo info;

	res = sceNetCtlInit();
	if (res < 0) return ret;

	res = sceNetCtlGetInfo(SCE_NET_CTL_INFO_SSID, &info);
	if (res < 0) return ret;
	ret = strdup(info.ssid);

	sceNetCtlTerm();

	return ret;
}

/* Get Language. */
char *LibHomebrew::Loot::SwissKnife::GetLanguage(void) {
	char *res = nullptr;
	int32_t systemParamValue = 0;

	int ret = sceSystemServiceParamGetInt(SCE_SYSTEM_SERVICE_PARAM_ID_LANG, &systemParamValue);
	if (ret != SCE_OK) return res;

	if (systemParamValue == SCE_SYSTEM_PARAM_LANG_JAPANESE) res = strdup("jp");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_GERMAN) res = strdup("de");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_ENGLISH_US) res = strdup("en-us");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_ENGLISH_GB) res = strdup("en-gb");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_DANISH) res = strdup("da");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_SPANISH) res = strdup("es");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_FRENCH) res = strdup("fr");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_ITALIAN) res = strdup("it");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_DUTCH) res = strdup("nl");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_NORWEGIAN) res = strdup("no");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_POLISH) res = strdup("pl");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_PORTUGUESE_PT) res = strdup("pt");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_RUSSIAN) res = strdup("ru");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_FINNISH) res = strdup("fi");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_SWEDISH) res = strdup("sv");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_KOREAN) res = strdup("ko");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_CHINESE_S) res = strdup("zh-cn");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_CHINESE_T) res = strdup("zh-tw");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_PORTUGUESE_BR) res = strdup("pt-br");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_TURKISH) res = strdup("tr");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_SPANISH_LA) res = strdup("es-mx");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_ARABIC) res = strdup("ar");
	else if (systemParamValue == SCE_SYSTEM_PARAM_LANG_FRENCH_CA) res = strdup("fr-ca");

	return res;
}

/*------------------------------------
			PS4IO Extensions
--------------------------------------*/
// Gets the size of a stream, stores it, then rewinds the stream and returns the length.
long LibHomebrew::Loot::SwissKnife::FileSize(FILE *stream) {
	if (stream == nullptr) return 25;                                 // Check if stream pointer is not null, else we return a error.
	fseek(stream, 0, SEEK_END);                                       // Seek to the end of the Stream.
	long fsize = ftell(stream);                                       // Get the Stream size and store it.
	rewind(stream);                                                   // Rewind Stream.
	return fsize;                                                     // Return the Stream size.
}

// Gets the size of a file, stores it, then it close the file and returns the length.
off_t LibHomebrew::Loot::SwissKnife::FileSize(const char *path) {
	if (*path == '\0') return -1;
	else if (path[0] == '\0') return -1;

	int stream = Sys::open(path, 0, O_RDONLY);                 // Open the file in read mode.
	if (stream == -1) return 25;                               // Check if we could open the file, else return a error.
	off_t fsize = Sys::lseek(stream, 0, SEEK_END);             // Seek to the end of the file.
	Sys::close(stream);                                        // Close the File.
	return fsize;                                              // Return the File size.
}

// Check if a Path exists. Note: Does not care about if it is a dir or a file.
bool LibHomebrew::Loot::SwissKnife::PathExists(const char *path) {
	if (*path == '\0') return false;
	else if (path[0] == '\0') return false;

	// Check path for ps4 path.
	char *_path = (char *)path;
	if (!isPs4Path(path)) _path = genPs4Path(path);

	// Open path now.
	int open = Sys::open(SwissKnife::genPs4Path(_path), O_RDONLY, 0);

	// Could it be opened ?
	if (open < 0) return false;

	// Close the path.
	Sys::close(open);

	// Clean up used resources.
	if (strcmp(_path, path)) delete _path;
	return true;
}

// Get Absolute PS4 Path, if not already onw.
char *LibHomebrew::Loot::SwissKnife::GetAbsolutePs4Path(const char *path) {
	if (*path == '\0') return (char *)path;
	else if (path[0] == '\0') return (char *)path;

	// Check path for ps4 path.
	char *_path = (char *)path;
	if (!isPs4Path(path)) _path = genPs4Path(path);
	return _path;
}

/* Check if path string is a PS4 valid path. */
bool LibHomebrew::Loot::SwissKnife::isPs4Path(const char *path) {
	String test(path);
	if (test.find_first_of("cache0:/") != -1) return true;
	return false;
}

/* Generates a PS4 valid path for IO functions that don't use Libc but Libkernel.
* This Function do not check the overloaded path pointer if it is a valid one
* or if the path hase value. Please check the path string before you use this function. */
char *LibHomebrew::Loot::SwissKnife::genPs4Path(const char *path) {
	if (path[0] != '/') return (char *)path;
	char *newPath = new char[strlen(path) + 9];
	
	// check if path starts with '/'
	String test(path);
	if (test.find_first_of("/") == 0) sprintf(newPath, "%s%s", "cache0:", path);
	else sprintf(newPath, "%s%s", "cache0:/", path);	
	
	char *ret = strdup(newPath);
	delete[] newPath;
	return ret;
}
