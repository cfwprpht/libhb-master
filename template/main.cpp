
#include <cstdio>
#include <cstdlib>
#include <fios2\fios2_api.h>
#include <kernel.h>
#include <libsysmodule.h>
#include <common_dialog.h>

#include "program.h"
#include "printf.h"

#pragma comment( lib , "SceSysmodule_stub_weak")
#pragma comment( lib , "SceFios2_stub_weak")
#pragma comment( lib , "kernel_stub_weak")

size_t sceLibcHeapSize = 256 * 1024 * 1024;

unsigned char *pStorageForFios2;
SceKernelModule baseService;
SceKernelModule libLua;
SceKernelModule libhb;
SceKernelModule libUtil;
SceKernelModule libResources;
SceKernelModule application;
SceKernelModule printfPrx;

// Round up.
static unsigned int roundUp(unsigned int size, unsigned int align) { return ((size + (align - 1u))&(~(align - 1u))); }

// Initialize the Input/Output System.
int InitializeFios2(SceFiosParams *params) {
	int32_t ret = SCE_OK;

	if (params == NULL) {
		SceFiosParams _params = SCE_FIOS_PARAMS_INITIALIZER;

		int32_t sizeOp = SCE_FIOS_OP_STORAGE_SIZE(64, SCE_FIOS_PATH_MAX);	// 64 ops:
		int32_t sizeChunk = SCE_FIOS_CHUNK_STORAGE_SIZE(1024);				// 1024 chunks, 64KiB:
		int32_t sizeFh = SCE_FIOS_FH_STORAGE_SIZE(16, SCE_FIOS_PATH_MAX);	// 16 file handles:
		int32_t sizeDh = SCE_FIOS_DH_STORAGE_SIZE(4, SCE_FIOS_PATH_MAX);	// 4 directory handles:

		sizeOp = roundUp(sizeOp, 8);
		sizeChunk = roundUp(sizeChunk, 8);
		sizeFh = roundUp(sizeFh, 8);
		sizeDh = roundUp(sizeDh, 8);

		int32_t sizeTotal = sizeOp + sizeChunk + sizeFh + sizeDh;

		pStorageForFios2 = (unsigned char*)malloc(sizeTotal);

		if (pStorageForFios2 == NULL) {
			return -1;
		}

		_params.opStorage.pPtr = pStorageForFios2;
		_params.opStorage.length = sizeOp;
		_params.chunkStorage.pPtr = pStorageForFios2 + sizeOp;
		_params.chunkStorage.length = sizeChunk;
		_params.fhStorage.pPtr = pStorageForFios2 + sizeOp + sizeChunk;
		_params.fhStorage.length = sizeFh;
		_params.dhStorage.pPtr = pStorageForFios2 + sizeOp + sizeChunk + sizeFh;
		_params.dhStorage.length = sizeDh;
		_params.pathMax = SCE_FIOS_PATH_MAX;
		_params.pMemcpy = memcpy;

		ret = sceFiosInitialize(&_params);
	} else {
		ret = sceFiosInitialize(params);
	}

	return ret;
}

// Initialize base service and load drivers.
int InitializeMe(void) {
	// Initialize Input/Output.
	if (InitializeFios2(NULL) < SCE_OK) return -1;

	// Initialize dialogs.
	if (sceCommonDialogInitialize() < SCE_OK) return -1;

	// First we load all libs which have non or only system dll depenses.
	//
	// Are we escaped from sandbox ?
	printfPrx = sceKernelLoadStartModule("/mnt/sandbox/pfsmnt/HXHB00113-app0/sce_module/printf.prx", 0, NULL, 0, NULL, NULL);
	if (printfPrx < 0) {
		// Still jailed.
		printfPrx = sceKernelLoadStartModule("/app0/sce_module/printf.prx", 0, NULL, 0, NULL, NULL);
		if (printfPrx < 0) return -1;
	}
		
	if (sceSysmoduleLoadModule(SCE_SYSMODULE_MESSAGE_DIALOG) < SCE_OK) return -1;
	if (sceSysmoduleLoadModule(SCE_SYSMODULE_IME_DIALOG) < SCE_OK) return -1;

	baseService = sceKernelLoadStartModule("/mnt/sandbox/pfsmnt/HXHB00113-app0/sce_module/libBaseService.prx", 0, NULL, 0, NULL, NULL);
	if (baseService < 0) {
		baseService = sceKernelLoadStartModule("/app0/sce_module/libBaseService.prx", 0, NULL, 0, NULL, NULL);
		if (baseService < 0) {
			_printf("Can't load libBaseService.prx : 0x%d\n", baseService);
			return -1;
		}
	}

	libLua = sceKernelLoadStartModule("/mnt/sandbox/pfsmnt/HXHB00113-app0/sce_module/libLua.prx", 0, NULL, 0, NULL, NULL);
	if (libLua < 0) {
		libLua = sceKernelLoadStartModule("/app0/sce_module/libLua.prx", 0, NULL, 0, NULL, NULL);
		if (libLua < 0) {
			_printf("Can't load libLua.prx : 0x%d\n", libLua);
			return -1;
		}
	}
		
	// Now we can safely load libhb since her depenses are loaded and initialized.
	libhb = sceKernelLoadStartModule("/mnt/sandbox/pfsmnt/HXHB00113-app0/sce_module/libhb.prx", 0, NULL, 0, NULL, NULL);
	if (libhb < 0) {
		libhb = sceKernelLoadStartModule("/app0/sce_module/libhb.prx", 0, NULL, 0, NULL, NULL);
		if (libhb < 0) {
			_printf("Can't load libhb.prx : 0x%d\n", libhb);
			return -1;
		}
	}

	// Load libUtil which hase depenses to libhb, libLua and sampleUtil.
	libUtil = sceKernelLoadStartModule("/mnt/sandbox/pfsmnt/HXHB00113-app0/sce_module/libUtil.prx", 0, NULL, 0, NULL, NULL);
	if (libUtil < 0) {
		libUtil = sceKernelLoadStartModule("/app0/sce_module/libUtil.prx", 0, NULL, 0, NULL, NULL);
		if (libUtil < 0) {
			_printf("Can't load libUtil.prx : 0x%d\n", libUtil);
			return -1;
		}
	}

	// Load Resources driver with depenses to libUtil, libLua, sampleUtil and libhb.
	libResources = sceKernelLoadStartModule("/mnt/sandbox/pfsmnt/HXHB00113-app0/sce_module/libResources.prx", 0, NULL, 0, NULL, NULL);
	if (libResources < 0) {
		libResources = sceKernelLoadStartModule("/app0/sce_module/libResources.prx", 0, NULL, 0, NULL, NULL);
		if (libResources < 0) {
			_printf("Can't load libResources.prx : 0x%d\n", libResources);
			return -1;
		}
	}

	// At last now we can load up the application it self, which hase depenses to libUtil, libResources, sampleUtil, the dialogs and libhb.
	application = sceKernelLoadStartModule("/mnt/sandbox/pfsmnt/HXHB00113-app0/sce_module/application.prx", 0, NULL, 0, NULL, NULL);
	if (application < 0) {
		application = sceKernelLoadStartModule("/app0/sce_module/application.prx", 0, NULL, 0, NULL, NULL);
		if (application < 0) {
			_printf("Can't load application.prx : 0x%d\n", application);
			return -1;
		}
	}

	return SCE_OK;
}

// Main Entry of the program code.
int main(void) {
	int ret = 0;

	// Load drivers.
	ret = InitializeMe();
	if (ret < SCE_OK) return ret;

	// Run Program.
	ret = Program();
	if (ret < SCE_OK) return -1;
	return 0;
}