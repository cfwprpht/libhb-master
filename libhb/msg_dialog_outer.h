#pragma once

#include <message_dialog.h>
#include <libsysmodule.h>
#include <cstdint>
#include "logger.h"

#pragma comment( lib , "libSceCommonDialog_stub_weak.a" )
#pragma comment( lib , "libSceMsgDialog_stub_weak.a" )

using namespace LibHomebrew;

// Button Enu
enum Buttons {
	Ok,
	YesNo,
	None,
	OkCancel,
	Cancel,
	Wait,
	WaitCancel,
	YesNoFocusNo,
	OkCancelFocusCancel,
	_2Buttons,
};

class MsgDialogWrapper {
public:
	enum Status {
		STATUS_BEGIN,         // Waiting for show_ime_dialog to be called.
		STATUS_DIALOG_START,  // Dialog has started running.
		STATUS_DIALOG_FINISH, // Dialog has finished running.
		STATUS_DIALOG_ERROR   // An error has occured.
	};

private:
	Status status;
	bool keep_alive = false;
	int32_t result;

public:
	Status getStatus() const { return status; }

	int32_t getResult() const { return result; }

	void doNotTerminate(bool state) { keep_alive = state; }

	int initialize() {
		status = STATUS_BEGIN;
		sceCommonDialogInitialize();

		const int loadModuleResult = sceSysmoduleLoadModule(SCE_SYSMODULE_MESSAGE_DIALOG);
		if (loadModuleResult != SCE_OK) {
			Logger::Debug("\n    Error loading Message Dialog PRX: 0x%8X\n", loadModuleResult);
		}

		int res = sceMsgDialogInitialize();
		if (res < SCE_OK) {
			Logger::Debug("\n    ERROR: sceMsgDialogInit: res=%08x\n", res);
		}

		return res;
	}

	int finalize() {
		if (keep_alive) {
			int res = sceMsgDialogTerminate();
			if (res != SCE_OK) {
				Logger::Debug("ERROR: sceMsgDialogTerminate: res=%08x\n", res);
				status = STATUS_DIALOG_ERROR;
				return res;
			}
		}
		
		const int unloadModuleResult = sceSysmoduleUnloadModule(SCE_SYSMODULE_MESSAGE_DIALOG);
		if (unloadModuleResult != SCE_OK) {
			Logger::Debug("Error unloading Message Dialog PRX: 0x%8X\n", unloadModuleResult);
		}
		return unloadModuleResult;
	}

	int show_msg_dialog(const char *message) {
		int	                            res;
		SceMsgDialogParam	            dialogParam;
		SceMsgDialogUserMessageParam    userMsgParam;
		sceMsgDialogParamInitialize(&dialogParam);

		memset(&userMsgParam, 0, sizeof(userMsgParam));
		userMsgParam.msg = message;
		dialogParam.mode = SCE_MSG_DIALOG_MODE_USER_MSG;		
		userMsgParam.buttonType = SCE_MSG_DIALOG_BUTTON_TYPE_OK;
		dialogParam.userMsgParam = &userMsgParam;

		res = sceMsgDialogOpen(&dialogParam);
		if (res < SCE_OK) Logger::Debug("ERROR: sceMsgDialogOpen: res=%08x\n", res);
		else status = STATUS_DIALOG_START;

		return res;
	}

	int show_msg_dialog(const char *message, Buttons button, const char *button1, const char *button2) {
		int	                            res;
		SceMsgDialogParam	            dialogParam;
		SceMsgDialogUserMessageParam    userMsgParam;
		sceMsgDialogParamInitialize(&dialogParam);

		memset(&userMsgParam, 0, sizeof(userMsgParam));
		userMsgParam.msg = message;
		dialogParam.mode = SCE_MSG_DIALOG_MODE_USER_MSG;
		userMsgParam.buttonType = button;
		if (button == _2Buttons) {
			userMsgParam.buttonsParam->msg1 = button1;
			userMsgParam.buttonsParam->msg2 = button2;
		}
		dialogParam.userMsgParam = &userMsgParam;		

		res = sceMsgDialogOpen(&dialogParam);
		if (res < SCE_OK) Logger::Debug("ERROR: sceMsgDialogOpen: res=%08x\n", res);
		else status = STATUS_DIALOG_START;

		return res;
	}

	int update_msg_dialog(void) {
		int	                    res;
		SceCommonDialogStatus	dialogStatus;
		result = 0;

		while (1) {
			dialogStatus = sceMsgDialogUpdateStatus();
			if (dialogStatus == SCE_COMMON_DIALOG_STATUS_FINISHED) {
				break;
			} else if (dialogStatus == SCE_COMMON_DIALOG_STATUS_RUNNING) {				
				return SCE_OK;
			}
		}

		SceMsgDialogResult	dialogResult;
		memset(&dialogResult, 0, sizeof(SceMsgDialogResult));
		res = sceMsgDialogGetResult(&dialogResult);
		if (res != SCE_OK) {
			Logger::Debug("ERROR: sceMsgDialogResult: res=%08x\n", res);
			status = STATUS_DIALOG_ERROR;
			return res;
		}

		if (dialogResult.result == SCE_COMMON_DIALOG_RESULT_USER_CANCELED) {
			Logger::Debug("User canceled MSG Dialog.\n");
		} else Logger::Debug("Terminate MSG Dialog input.\n");

		if (!keep_alive) {
			res = sceMsgDialogTerminate();
			if (res != SCE_OK) {
				Logger::Debug("ERROR: sceMsgDialogTerminate: res=%08x\n", res);
				status = STATUS_DIALOG_ERROR;
				return res;
			}
		}
		status = STATUS_DIALOG_FINISH;

		Logger::Debug("dialogResult.result = %d\n", dialogResult.result);
		result = dialogResult.result;
		return SCE_OK;
	}

	int everyFrame() {
		if (status == STATUS_DIALOG_START) {
			update_msg_dialog();
			if (status == STATUS_DIALOG_FINISH
				|| status == STATUS_DIALOG_ERROR) {
				return status;
			}
		}

		if (status == STATUS_DIALOG_FINISH) status = STATUS_BEGIN;
		return status;
	}
};