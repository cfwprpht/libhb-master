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
#include "lv2.h"

#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif

struct kpayload_args { uint64_t user_arg; };

struct kdump_args { uint64_t argArrayPtr; };

// Unjail patches for all FWs.
PRX_INTERFACE void *unjail(struct thread *td);

// Enable Mapping of SELF files.
PRX_INTERFACE void *enableMMAPSelf(struct thread *td);

// Disable Mapping of SELF files.
PRX_INTERFACE void *disableMMAPSelf(struct thread *td);

// Disable Process ASLR.
PRX_INTERFACE void *disableProcessASLR(struct thread *td);

// Enable Browser.
PRX_INTERFACE void *enableBrowser(struct thread *td);