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

#pragma comment( lib , "libSceNet_stub_weak.a")

#include "ps4_network.h"
#include "console.h"
#include <sampleutil.h>
#include <net.h>

using namespace LibHomebrew;

int LibHomebrew::PS4Network::Network::size = 0;
char LibHomebrew::PS4Network::Network::mesBuf[512];

// Instanze Initializer.
LibHomebrew::PS4Network::Network::Network() {
	server = isInit = false;
	protocol = sockType = options = flag = 0;
}

// Instanze Initializer.
LibHomebrew::PS4Network::Network::Network(char *_ip, char *_port, char *_name, bool _server, int _protocol, int _socktype, int _options, int _flag) {
	ip = strdup(_ip);
	port = strdup(_port);
	name = strdup(_name);
	server = _server;
	protocol = _protocol;
	sockType = _socktype;
	options = _options;
	flag = _flag;

	char *endptr;
	nip = sceNetHtonl(strtoimax(ip, &endptr, 0));
	nport = sceNetHtons(strtoimax(port, &endptr, 0));
	isInit = true;
}

// Instanze Deinitializer.
LibHomebrew::PS4Network::Network::~Network() {
	if (isInit) sceNetSocketClose(sock);
	delete ip;
	delete port;
	delete name;
}

// Initialize a network.
int LibHomebrew::PS4Network::Network::Open() {
	int ret = 0;
	if (!isInit) {
		isInit = true;
		SceNetSockaddrIn socket;
		socket.sin_len = sizeof(socket);
		socket.sin_family = SCE_NET_AF_INET;
		socket.sin_addr.s_addr = nip;
		socket.sin_port = nport;
		memset(socket.sin_zero, 0, sizeof(socket.sin_zero));

		if (server) {
			sock = sceNetSocket(name, SCE_NET_AF_INET, sockType, 0);
			if (protocol != UNDEFINED) sceNetSetsockopt(sock, protocol, options, (char *)&flag, sizeof(int));

			ret = sceNetBind(sock, (SceNetSockaddr *)&socket, sizeof(socket));
			if (ret != SCE_OK) {
				isInit = false;
				Console::WriteLine("Error: Can't bind to socket.");
			}
			else {
				ret = sceNetListen(sockServer, 10);
				if (ret != SCE_OK) {
					isInit = false;
					Console::WriteLine("Error: Can't listen on socket.");
				}
				else { if (waitForClient) sock = sceNetAccept(sock, nullptr, nullptr); }
			}
		} else {
			sock = sceNetSocket(name, SCE_NET_AF_INET, sockType, 0);
			ret = sceNetConnect(sock, (SceNetSockaddr *)&socket, sizeof(socket));
			if (ret != SCE_OK) {
				isInit = false;
				Console::WriteLine("Error: Can't connect to socket.");
			}
			else { if (protocol != UNDEFINED) sceNetSetsockopt(sock, protocol, options, (char *)&flag, sizeof(int)); }
		}		
	}
	return 0;
}

// Deinitialize a network.
void LibHomebrew::PS4Network::Network::Close() { this->~Network(); }

// Seund a a message.
int LibHomebrew::PS4Network::Network::Send(const char *msg, ...) {
	if (isInit) {
		size = 0;
		memset(mesBuf, 0, sizeof(mesBuf));
		va_list args;
		va_start(args, msg);
		size = sprintf(mesBuf, msg, args);
		va_end(args);
		sceNetSend(sock, mesBuf, size, 0);
	}
	return 0;
}