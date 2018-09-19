#pragma once

#include <message_dialog.h>
#include <libsysmodule.h>
#include <cstdint>

#pragma comment( lib , "libSceCommonDialog_stub_weak.a" )
#pragma comment( lib , "libSceMsgDialog_stub_weak.a" )

#ifndef PRX_INTERFACE
#ifdef LIBRARY_IMPL
#define PRX_INTERFACE __declspec(dllexport)
#else
#define PRX_INTERFACE __declspec(dllimport)
#endif
#endif

using namespace LibHomebrew;

// Button Enum.
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

class PRX_INTERFACE MsgDialogWrapper {
public:
	enum Status {
		STATUS_BEGIN,         // Waiting for show_ime_dialog to be called.
		STATUS_DIALOG_START,  // Dialog has started running.
		STATUS_DIALOG_FINISH, // Dialog has finished running.
		STATUS_DIALOG_ERROR   // An error has occured.
	};

private:
	Status status;
	bool keep_alive;
	int32_t result;

public:
	MsgDialogWrapper() { keep_alive = false; }

	~MsgDialogWrapper() { }

	Status getStatus() const { return status; }

	int32_t getResult() const { return result; }

	void doNotTerminate(bool state) { keep_alive = state; }

	int initialize() {
		status = STATUS_BEGIN;

		int res = sceMsgDialogInitialize();
		if (res < SCE_OK) {
			_printf("\n    ERROR: sceMsgDialogInit: res=%08x\n", res);
		}
		return res;
	}

	int finalize() {
		if (keep_alive) {
			int res = sceMsgDialogTerminate();
			if (res != SCE_OK) {
				_printf("ERROR: sceMsgDialogTerminate: res=%08x\n", res);
				status = STATUS_DIALOG_ERROR;
				return res;
			}
		}
		return SCE_OK;
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
		if (res < SCE_OK) _printf("ERROR: sceMsgDialogOpen: res=%08x\n", res);
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
		if (res < SCE_OK) _printf("ERROR: sceMsgDialogOpen: res=%08x\n", res);
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
			_printf("ERROR: sceMsgDialogResult: res=%08x\n", res);
			status = STATUS_DIALOG_ERROR;
			return res;
		}

		if (dialogResult.result == SCE_COMMON_DIALOG_RESULT_USER_CANCELED) {
			_printf("User canceled MSG Dialog.\n");
		} else _printf("Terminate MSG Dialog input.\n");

		if (!keep_alive) {
			res = sceMsgDialogTerminate();
			if (res != SCE_OK) {
				_printf("ERROR: sceMsgDialogTerminate: res=%08x\n", res);
				status = STATUS_DIALOG_ERROR;
				return res;
			}
		}
		status = STATUS_DIALOG_FINISH;

		_printf("dialogResult.result = %d\n", dialogResult.result);
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