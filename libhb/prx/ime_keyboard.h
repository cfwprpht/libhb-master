#pragma once

#include <libime.h>
#include <libsysmodule.h>
#include <cstdint>
#include "printf.h"

//E Link with libSceIme_stub_weak.a.
//J libSceIme_stub_weak.a へのリンク
#pragma comment( lib , "libSceIme_stub_weak.a" )


#ifndef PRX_INTERFACE
#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif
#endif

class PRX_INTERFACE LibImeKeyboardWrapper {
public:
	int initialize() {
		//E Load module
		//J モジュールのロード
		const int loadModuleResult = sceSysmoduleLoadModule(SCE_SYSMODULE_LIBIME);
		if (loadModuleResult != SCE_OK) {
			_printf("Error loading IME PRX: 0x%8X\n", loadModuleResult);
		}
		return loadModuleResult;
	}

	int finalize() {
		//E Unload the module.
		//J モジュールのアンロード
		const int unloadModuleResult = sceSysmoduleUnloadModule(SCE_SYSMODULE_LIBIME);
		if (unloadModuleResult != SCE_OK) {
			_printf("Error unloading IME PRX: 0x%8X\n", unloadModuleResult);
		}
		return unloadModuleResult;
	}

	int start_keyboard(const SceUserServiceUserId userId, uint32_t *state) {
		int ret = 0;
		SceImeKeyboardParam kparam;

		kparam.option = 0;				//E Set keyboard options
		kparam.arg = (void*)state;		//E Set application-defined argument
		kparam.handler = keyboardEvent;	//E Set keyboard event handler
										//E zero padding reserve area
										//J reserve領域をゼロ埋めする
		memset(kparam.reserved1, 0x00, sizeof(kparam.reserved1));
		memset(kparam.reserved2, 0x00, sizeof(kparam.reserved2));

		//E Open the keyboard manager
		//J keyboardのオープン
		if ((ret = sceImeKeyboardOpen(userId, &kparam)) != SCE_OK) {
			if (ret == SCE_IME_ERROR_NOT_ACTIVE) {
				_printf(".");
			}
			else {
				_printf("    # keyboard : sceImeKeyboardOpen() error:0x%08x #\n", ret);
			}
			return ret;
		}
		_printf("    # keyboard : sceImeKeyboardOpen() Success #\n");
		SceImeKeyboardResourceIdArray resourceIdArray;
		//E Get keyboard resource Id.
		//J キーボードのリソースIDの取得
		if ((ret = sceImeKeyboardGetResourceId(userId, &resourceIdArray)) != SCE_OK) {
			_printf("    # keyboard : sceImeKeyboardGetResourceId() error:0x%08x #\n", ret);
		}
		else {
			_printf("    # keyboard : sceImeKeyboardGetResourceId() Success    resourceId:%d, %d, %d, %d %d #\n", resourceIdArray.resourceId[0], resourceIdArray.resourceId[1], resourceIdArray.resourceId[2], resourceIdArray.resourceId[3], resourceIdArray.resourceId[4]);
			for (int idx = 0; idx < SCE_IME_KEYBOARD_MAX_NUMBER; ++idx) {
				if (resourceIdArray.resourceId[idx] != SCE_IME_KEYBOARD_RESOURCE_ID_INVALID) {
					SceImeKeyboardInfo info;
					//E Get keyboard information by resouce Id.
					//J リソースIDからキーボード情報を取得
					if ((ret = sceImeKeyboardGetInfo(resourceIdArray.resourceId[idx], &info)) != SCE_OK) {
						_printf("    # keyboard : sceImeKeyboardGetInfo() error:0x%08x #\n", ret);
					}
					else {
						_printf("    # keyboard : sceImeKeyboardGetInfo() Success:[idx=%d] uid=%08x type=%d#\n", idx, info.userId, info.type);
					}
				}
			}
		}
		*state = 0;
		return SCE_OK;
	}

	static void keyboardEvent(void *arg, const SceImeEvent *e) {
		switch (e->id) {
		case SCE_IME_KEYBOARD_EVENT_OPEN:
			_printf("Keyboard library is opened[uid=0x%08x]:Resource ID(%d, %d, %d)\n", e->param.resourceIdArray.userId, e->param.resourceIdArray.resourceId[0], e->param.resourceIdArray.resourceId[1], e->param.resourceIdArray.resourceId[2]);
			break;
		case SCE_IME_KEYBOARD_EVENT_KEYCODE_DOWN:
			_printf("Key DOWN:\"0x%04X\"(%lc:%04x), M 0x%05X(%d%d%d%d-%d%d%d%d)\n", e->param.keycode.keycode, (e->param.keycode.status & 2) ? e->param.keycode.character : '-', e->param.keycode.character,
				e->param.keycode.status,
				(e->param.keycode.status >> 15) & 1, (e->param.keycode.status >> 14) & 1, (e->param.keycode.status >> 13) & 1, (e->param.keycode.status >> 12) & 1,
				(e->param.keycode.status >> 11) & 1, (e->param.keycode.status >> 10) & 1, (e->param.keycode.status >> 9) & 1, (e->param.keycode.status >> 8) & 1);
			break;
		case SCE_IME_KEYBOARD_EVENT_KEYCODE_UP:
			_printf("Key UP:\"0x%04X\"(%lc), M 0x%05X\n", e->param.keycode.keycode, (e->param.keycode.status & 2) ? e->param.keycode.character : '-', e->param.keycode.status);
			if (e->param.keycode.keycode == 0x0029) {
				*(uint32_t*)arg = 1;
			}
			break;
		case SCE_IME_KEYBOARD_EVENT_KEYCODE_REPEAT:
			_printf("Key REPEAT:\"0x%04X\"(%lc)\n", e->param.keycode.keycode, (e->param.keycode.status & 2) ? e->param.keycode.character : '-');
			break;
		case SCE_IME_KEYBOARD_EVENT_CONNECTION:
			_printf("Keyboard is connected[uid=0x%08x]:Resource ID(%d, %d, %d)\n", e->param.resourceIdArray.userId, e->param.resourceIdArray.resourceId[0], e->param.resourceIdArray.resourceId[1], e->param.resourceIdArray.resourceId[2]);
			break;
		case SCE_IME_KEYBOARD_EVENT_DISCONNECTION:
			_printf("Keyboard is disconnected[uid=0x%08x]:Resource ID(%d, %d, %d)\n", e->param.resourceIdArray.userId, e->param.resourceIdArray.resourceId[0], e->param.resourceIdArray.resourceId[1], e->param.resourceIdArray.resourceId[2]);
			break;
		case SCE_IME_KEYBOARD_EVENT_ABORT:
			*(uint32_t*)arg = 2;
			_printf("Keyboard manager was aborted\n");
			break;
		default:
			_printf("Invalid Event: id=%d\n", e->id);
			break;
		}
	}

	void update_keyboard() {
		//E Update keyboard
		//J Keyboard イベントのアップデート通知
		sceImeUpdate(keyboardEvent);
	}

	void end_keyboard(const SceUserServiceUserId userId) {
		//E Close the keyboard manager
		//J keyboardのクローズ
		int ret = sceImeKeyboardClose(userId);
		if (ret != SCE_OK) {
			_printf("    # keyboard : sceImeKeyboardClose() Error: userId=%08x ret=%08x#\n", userId, ret);
			return;
		}
		_printf("    # keyboard : sceImeKeyboardClose() Success: userId=%08x#\n", userId);
	}
};