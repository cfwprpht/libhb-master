#pragma once

#include <ime_dialog.h>
#include <libsysmodule.h>
#include <cstdint>
#include "logger.h"

#pragma comment( lib , "libSceImeDialog_stub_weak.a" )

using namespace LibHomebrew;

class ImeDialogWrapper {
public:
	enum Status {
		STATUS_BEGIN,         // Waiting for show_ime_dialog to be called.
		STATUS_DIALOG_START,  // Dialog has started running.
		STATUS_DIALOG_FINISH, // Dialog has finished running.
		STATUS_DIALOG_ERROR   // An error has occured.
	};

private:
	Status status;
	uint32_t dialogWidth, dialogHeight;

public:
	Status getStatus() const { return status; }

	int initialize() {
		status = STATUS_BEGIN;
		const int loadModuleResult = sceSysmoduleLoadModule(SCE_SYSMODULE_IME_DIALOG);
		if (loadModuleResult != SCE_OK) Logger::Debug("\n   Error loading IME Dialog PRX: 0x%8X\n", loadModuleResult);
		return loadModuleResult;
	}

	int finalize() {
		const int unloadModuleResult = sceSysmoduleUnloadModule(SCE_SYSMODULE_IME_DIALOG);
		if (unloadModuleResult != SCE_OK) Logger::Debug("Error unloading IME Dialog PRX: 0x%8X\n", unloadModuleResult);
		return unloadModuleResult;
	}

	int show_ime_dialog(const wchar_t* dialogTitle,  	// dialog title
		const wchar_t* placeholderText,					// placeholder string or null
		wchar_t* resultBuffer,							// input text buffer
		uint32_t maxTextLength)						    // maximum length of character string
	{
		SceUserServiceUserId m_userId;
		SceImeDialogParam	dialogParam;
		int		res;

		sceImeDialogParamInit(&dialogParam);

		int ret = sceUserServiceGetInitialUser(&m_userId);
		if (ret < SCE_OK) {
			return ret;
		}
		Logger::Debug("    # UserID:0x%08x #\n", m_userId);

		dialogParam.userId = m_userId;							//E Set user Id
		dialogParam.type = SCE_IME_TYPE_DEFAULT;				//E Set input UI types
		dialogParam.option = SCE_IME_OPTION_MULTILINE;			//E Set options settings
		dialogParam.supportedLanguages = 0;						//E Set supported languages by the IME, or 0
		dialogParam.enterLabel = SCE_IME_ENTER_LABEL_DEFAULT;	//E Set label of Enter key
		dialogParam.inputMethod = SCE_IME_INPUT_METHOD_DEFAULT; //E Set input method
		dialogParam.title = dialogTitle;						//E Set title character string of the IME dialog, or NULL
		dialogParam.placeholder = placeholderText;				//E Set guide string, or NULL
		dialogParam.inputTextBuffer = resultBuffer;           	//E Set buffer to store input string
		dialogParam.maxTextLength = maxTextLength;				//E Set maximum length of string that can be input

		//E Get ime dialog size.
		sceImeDialogGetPanelSizeExtended(&dialogParam, NULL, &dialogWidth, &dialogHeight);
		Logger::Debug("    # Dialog Size(%d,%d)\n", dialogWidth, dialogHeight);

		dialogParam.posx = 1920.0f / 2.0f - dialogWidth / 2.0;	//E Set X coordinate of the display position
		dialogParam.posy = 1080.0f / 2.0f - dialogHeight / 2.0;	//E Set Y coordinate of the display position
		dialogParam.horizontalAlignment = SCE_IME_HALIGN_LEFT;	//E Set IME dialog horizontal display origins
		dialogParam.verticalAlignment = SCE_IME_VALIGN_TOP;		//E Set IME dialog vertical display origins
		dialogParam.filter = NULL;								//E Set character string filter callback function or NULL

		res = sceImeDialogInit(&dialogParam, NULL);

		//E Check for errors.
		if (res != SCE_OK) {
			if (res == SCE_IME_ERROR_NOT_ACTIVE) Logger::Debug(".");
			else Logger::Debug("ERROR: sceImeDialogInit: res=%08x\n", res);
		} else status = STATUS_DIALOG_START;

		return res;
	}

	int update_ime_dialog(void) {
		SceImeDialogStatus	dialogStatus;

		int		res;
		dialogStatus = sceImeDialogGetStatus();

		switch (dialogStatus) {
			default:
			case SCE_IME_DIALOG_STATUS_NONE:
			case SCE_IME_DIALOG_STATUS_RUNNING:
				return SCE_OK;
			case SCE_IME_DIALOG_STATUS_FINISHED:
				break;
		}

		SceImeDialogResult	dialogResult;
		memset(&dialogResult, 0, sizeof(SceImeDialogResult));
		res = sceImeDialogGetResult(&dialogResult);
		if (res != SCE_OK) {
			Logger::Debug("ERROR: sceImeDialogGetResult: res=%08x\n", res);
			status = STATUS_DIALOG_ERROR;
			return res;
		}

		if (dialogResult.endstatus == SCE_IME_DIALOG_END_STATUS_ABORTED) {
			Logger::Debug("Aborted IME Dialog input.\n");
		} else if (dialogResult.endstatus == SCE_IME_DIALOG_END_STATUS_USER_CANCELED) {
			Logger::Debug("User canceled IME Dialog input.\n");
		} else Logger::Debug("Terminate IME Dialog input.\n");

		res = sceImeDialogTerm();
		if (res != SCE_OK) {
			Logger::Debug("ERROR: sceImeDialogTerm: res=%08x\n", res);
			status = STATUS_DIALOG_ERROR;
			return res;
		}
		status = STATUS_DIALOG_FINISH;

		Logger::Debug("dialogResult.endstatus = %d\n", dialogResult.endstatus);
		return SCE_OK;
	}

	int everyFrame() {
		if (status == STATUS_DIALOG_START) {
			update_ime_dialog();
			if (status == STATUS_DIALOG_FINISH
				|| status == STATUS_DIALOG_ERROR) {
				return status;
			}
		}

		if (status == STATUS_DIALOG_FINISH) status = STATUS_BEGIN;
		return status;
	}
};