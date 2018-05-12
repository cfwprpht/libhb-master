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

#include "test.h"

void *KConTest(struct thread *td) {
	lv2_init();

	//char buffer[256];
	char msg1[26] = "Hello from the kernel !\n";
	char msg2[33] = "Thats a error from the kernel.\n";
	char msg3[35] = "Thats a warning from the kernel.\n";
	char msg4[76] = "If you can see thoes 4 lines, hurray !\nKernel Console Messages working !\n";

	//lv2.sprintf(buffer, );
	WriteLine(msg1);
	//lv2.memset(buffer, 0, sizeof(buffer));

	//lv2.sprintf(buffer, );
	WriteError(msg2);
	//lv2.memset(buffer, 0, sizeof(buffer));

	//lv2.sprintf(buffer, );
	WriteWarning(msg3);
	//lv2.memset(buffer, 0, sizeof(buffer));

	//lv2.sprintf(buffer, );
	WriteLine(msg4);
	//lv2.memset(buffer, 0, sizeof(buffer));
	return 0;
}