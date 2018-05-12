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

#include <_types.h>

#define UNDEFINED -99

namespace LibHomebrew {
	namespace PS4Network {
		class Network {
		private:
			bool isInit;
			static int size;
			static char mesBuf[512];

		public:
			uint32_t nip;
			char *ip;
			uint16_t nport;
			char *port;
			char *name;
			bool server;
			bool waitForClient;
			int protocol = -99;
			int sockType;
			int options;
			int flag;
			int sockServer;
			int sock;

			Network();
			Network(char *ip, char *port, char *name, bool server, int protocol, int socktype, int options, int flag);
			~Network();
			int Open(void);
			void Close(void);
			int Send(const char *msg, ...);
			static char *GetLocalIP(void);
		};
	}
}