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

#include <stdio.h>
#include <sampleutil.h>
#include "ps4_message_box.h"
#include "logger.h"

using namespace LibHomebrew;

//size_t sceLibcHeapSize = 256 * 1024 * 1024;

//The maximum number of character string
#define TEXT_MAX_LENGTH 20

//The domain that stores the character string of the result
static wchar_t resultTextBuf[TEXT_MAX_LENGTH + 1];

int PS4Forms::Ime::initialize(void) {
	Logger::Debug("### ime_dialog : Initialize ###\n");
	int ret = 0;

	ret = initializeUtil(kFunctionFlagAll);
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	wcsncpy(resultTextBuf, L"Edit", sizeof(resultTextBuf) / sizeof(wchar_t));

	imeDialog = new ImeDialogWrapper();
	SCE_SAMPLE_UTIL_ASSERT(imeDialog != NULL);
	return imeDialog->initialize();
}

int PS4Forms::Ime::update(void) {
	int ret = 0;

	ret = updateUtil();
	if (ret != SCE_OK) return ret;

	if (imeDialog->everyFrame() == ImeDialogWrapper::STATUS_DIALOG_FINISH) {
		Logger::Debug("IME set resultTextBuf to \"%ls\"\n", resultTextBuf);
		return 1;
	}

	return SCE_OK;
}

//E Simple render example. Edit this part for your sample.
//E If your sample does not need graphics, please execute the flip process only.
//J 簡単なrenderの実装例。適宜サンプルに併せて修正してください。
//J グラフィック表示が不要な場合はフリップ処理のみ行ってください。
void PS4Forms::Ime::render(void) {
	/*getGraphicsContext()->beginScene(getGraphicsContext()->getNextRenderTarget(),
		getGraphicsContext()->getDepthStencilSurface());
	getGraphicsContext()->clearRenderTarget(0x00000000);
	getGraphicsContext()->endScene();*/
	getGraphicsContext()->flip(1);
}

int PS4Forms::Ime::finalize(void) {
	int ret = 0;

	if (imeDialog) {
		imeDialog->finalize();
		delete imeDialog;
		imeDialog = NULL;
	}

	ret = finalizeUtil();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	return SCE_OK;
}

void PS4Forms::Ime::Show(wchar_t *title, wchar_t *placeholder, wchar_t *resultBuffer) {
	if (imeDialog->getStatus() == ImeDialogWrapper::STATUS_BEGIN) {
		const int32_t showDialogResult = imeDialog->show_ime_dialog(title, placeholder, resultTextBuf, TEXT_MAX_LENGTH);
		if (showDialogResult != SCE_OK) Logger::Debug("Error in show_ime_dialog(): 0x%8X\n", showDialogResult);
	}
}

int PS4Forms::Ime::exec(void) {
	int ret = 0;

	ret = initialize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	while (1) {
		ret = update();
		if (ret != SCE_OK) break;
		render();
	}

	ret = finalize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	return 0;
}

int PS4Forms::ImeKeyboard::initialize(void) {
	Logger::Debug("### keyboard : Initialize ###\n");
	int ret = 0;
	ret = initializeUtil(kFunctionFlagDefault);
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);
	if (ret < SCE_OK) return ret;
	ret = sceUserServiceGetInitialUser(&userId);
	if (ret < SCE_OK) return ret;
	Logger::Debug("    # UserID:0x%08x #\n", userId);

	imeKeyboard = new LibImeKeyboardWrapper();
	if (imeKeyboard == NULL) return -1;
	imeKeyboard->initialize();

	return SCE_OK;
}

uint32_t PS4Forms::ImeKeyboard::getState(void) const { return state; }

int PS4Forms::ImeKeyboard::update(void) {
	int ret = 0;

	if ((ret = updateUtil()) != SCE_OK) return ret;

	if (startflag == true) {
		ret = imeKeyboard->start_keyboard(userId, &state);
		if (ret == SCE_OK) startflag = false;
		else if (ret != SCE_IME_ERROR_NOT_ACTIVE) return ret;
	}
	if (startflag == false) {
		imeKeyboard->update_keyboard();
		if (getState() == 1) return -1;
		if (getState() == 2) {
			imeKeyboard->end_keyboard(userId);
			startflag = true;
		}
	}

	return SCE_OK;
}

//E Simple render example. Edit this part for your sample.
//E If your sample does not need graphics, please execute the flip process only.
//J 簡単なrenderの実装例。適宜サンプルに併せて修正してください。
//J グラフィック表示が不要な場合はフリップ処理のみ行ってください。
void PS4Forms::ImeKeyboard::render(void) {
	/*getGraphicsContext()->beginScene(getGraphicsContext()->getNextRenderTarget(),
		getGraphicsContext()->getDepthStencilSurface());
	getGraphicsContext()->clearRenderTarget(0x00000000);
	getGraphicsContext()->endScene();*/
	getGraphicsContext()->flip(1);
}

int PS4Forms::ImeKeyboard::finalize(void) {
	int ret = 0;

	if (imeKeyboard) {
		imeKeyboard->finalize();
		delete imeKeyboard;
		imeKeyboard = NULL;
	}

	ret = finalizeUtil();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);
	return SCE_OK;
}

int PS4Forms::ImeKeyboard::exec(void) {
	int ret = 0;

	ret = initialize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	while (1) {
		ret = update();
		if (ret != SCE_OK) break;
		render();
	}

	ret = finalize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);
	return 0;
}

int PS4Forms::Msg::initialize(void) {
	Logger::Debug("### msg_dialog : Initialize ###\n");
	int ret = 0;

	ret = initializeUtil(kFunctionFlagAll);
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	msgDialog = new MsgDialogWrapper();
	SCE_SAMPLE_UTIL_ASSERT(msgDialog != NULL);
	msgDialog->initialize();

	return SCE_OK;
}

int PS4Forms::Msg::update(void) {
	int ret = 0;

	ret = updateUtil();
	if (ret != SCE_OK) return ret;

	if (msgDialog->everyFrame() == MsgDialogWrapper::STATUS_DIALOG_FINISH) {
		return 1;
	}

	return SCE_OK;
}

//E Simple render example. Edit this part for your sample.
//E If your sample does not need graphics, please execute the flip process only.
//J 簡単なrenderの実装例。適宜サンプルに併せて修正してください。
//J グラフィック表示が不要な場合はフリップ処理のみ行ってください。
void PS4Forms::Msg::render(void) {
	/*getGraphicsContext()->beginScene(getGraphicsContext()->getNextRenderTarget(),
	getGraphicsContext()->getDepthStencilSurface());
	getGraphicsContext()->clearRenderTarget(0x00000000);
	getGraphicsContext()->endScene();*/
	getGraphicsContext()->flip(1);
}

int PS4Forms::Msg::finalize(void) {
	int ret = 0;

	if (msgDialog) {
		msgDialog->finalize();
		delete msgDialog;
		msgDialog = NULL;
	}

	ret = finalizeUtil();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	return SCE_OK;
}

void PS4Forms::Msg::Show(const char *message) {
	if (msgDialog->getStatus() == MsgDialogWrapper::STATUS_BEGIN) {
		const int32_t showDialogResult = msgDialog->show_msg_dialog(message);
		if (showDialogResult != SCE_OK) Logger::Debug("Error in show_msg_dialog(): 0x%8X\n", showDialogResult);
	}
}

void PS4Forms::Msg::Show(const char *message, Buttons buttons, const char *button1, const char *button2) {
	if (msgDialog->getStatus() == MsgDialogWrapper::STATUS_BEGIN) {
		const int32_t showDialogResult = msgDialog->show_msg_dialog(message, buttons, button1, button2);
		if (showDialogResult != SCE_OK) Logger::Debug("Error in show_msg_dialog(): 0x%8X\n", showDialogResult);
	}
}

int PS4Forms::Msg::exec(void) {
	int ret = 0;

	ret = initialize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	while (1) {
		ret = update();
		if (ret != SCE_OK) break;
		render();
	}

	ret = finalize();
	SCE_SAMPLE_UTIL_ASSERT(ret == SCE_OK);

	return 0;
}

ScePthread PS4Forms::Dialogs::ImeDialog::thread = nullptr;

// A Ime Dialog Class instance.
PS4Forms::Ime *PS4Forms::Dialogs::ImeDialog::imedialog;

// Run the Ime Dialog in a own thread, so we can call the show function.
void *PS4Forms::Dialogs::ImeDialog::RunIme(void *) { imedialog->exec(); return 0; }

// Show a Ime Dialog.
wchar_t *PS4Forms::Dialogs::ImeDialog::ShowIme(wchar_t *title, wchar_t *placeholder) {
	// Clear buffer.
	memset(resultTextBuf, 0, sizeof(resultTextBuf) / sizeof(wchar_t));
	
	// Initialize a new Ime Dialog.
	imedialog = new Ime();

	// Run a thread and execute 
	if (scePthreadCreate(&thread, NULL, RunIme, NULL, "Ime_Dialog") != SCE_OK) {
		Logger::Debug("Couldn't run Ime Dialog Thread.\n");
		goto done;
	}

	// Show Dialog.
	imedialog->Show(title, placeholder, resultTextBuf);

	// Wait for thread to be done.
	scePthreadJoin(thread, NULL);

done:
	// Free memory.
	delete imedialog;

	// Return result.
	return resultTextBuf;
}

// Show a Ime Dialog, using a external result buffer.
void PS4Forms::Dialogs::ImeDialog::ShowIme(wchar_t *title, wchar_t *placeholder, wchar_t *resultBuffer) {
	// Clear buffer.
	memset(resultTextBuf, 0, sizeof(resultTextBuf) / sizeof(wchar_t));

	// Initialize a new Ime Dialog.
	imedialog = new Ime();

	// Run a thread and execute 
	if (scePthreadCreate(&thread, NULL, RunIme, NULL, "Ime_Dialog") != SCE_OK) {
		Logger::Debug("Couldn't run Ime Dialog Thread.\n");
		goto done;
	}

	// Show Dialog.
	imedialog->Show(title, placeholder, resultBuffer);

	// Wait for thread to be done.
	scePthreadJoin(thread, NULL);

done:
	// Free memory.
	delete imedialog;
}

ScePthread PS4Forms::Dialogs::MsgDialog::thread = nullptr;

// A Msg Dialog Class instance.
PS4Forms::Msg *PS4Forms::Dialogs::MsgDialog::msgdialog;

// Run the Msg Dialog in a own thread, so we can call the show function.
void *PS4Forms::Dialogs::MsgDialog::RunMsg(void *) {
	msgdialog->exec();
	return 0;
}

// Show message dialog.
DialogResult PS4Forms::Dialogs::MsgDialog::ShowDialog(String message) {
	// Initialize a new Ime Dialog.
	msgdialog = new Msg();

	// Set a result.
	DialogResult _result = DialogResult::Error;

	// Run a thread and execute 
	if (scePthreadCreate(&thread, NULL, RunMsg, NULL, "Msg_Dialog") != SCE_OK) {
		Logger::Debug("Couldn't run Msg Dialog Thread.\n");
		goto done;
	}

	// Show Dialog.
	msgdialog->Show(message.c_str());

	// Wait for thread to be done.
	scePthreadJoin(thread, NULL);

	// Get the status.
	MsgDialogWrapper::Status status;
	status = msgdialog->getStatus();

	// Resolve the result.	
	if (status == MsgDialogWrapper::STATUS_DIALOG_FINISH) {
		uint32_t result = msgdialog->getResult();
		if (result == SCE_MSG_DIALOG_BUTTON_ID_OK) {
			_result = PS4Forms::DialogResult::Ok;
		} else if (result == SCE_MSG_DIALOG_BUTTON_ID_NO) {
			_result = DialogResult::No;
		} else if (result == SCE_MSG_DIALOG_BUTTON_ID_YES) {
			_result = DialogResult::Yes;
		} else if (result == SCE_MSG_DIALOG_BUTTON_ID_INVALID) {
			_result = DialogResult::Cancel;
		}
	}

done:
	// Free memory.
	delete msgdialog;

	// Return result.
	return _result;
}

// Show message dialog, defining specific buttons to use.
DialogResult PS4Forms::Dialogs::MsgDialog::ShowDialog(String message, Buttons buttons, const char *button1, const char *button2) {
	// Initialize a new Ime Dialog.
	msgdialog = new Msg();

	// Set a result.
	DialogResult _result = DialogResult::Error;

	// Run a thread and execute 
	if (scePthreadCreate(&thread, NULL, RunMsg, NULL, "Msg_Dialog") != SCE_OK) {
		Logger::Debug("Couldn't run Msg Dialog Thread.\n");
		goto done;
	}

	// Show Dialog.
	msgdialog->Show(message.c_str(), buttons, button1, button2);

	// Wait for thread to be done.
	scePthreadJoin(thread, NULL);

	// Get the status.
	MsgDialogWrapper::Status status;
	status = msgdialog->getStatus();

	// Resolve the result.	
	if (status == MsgDialogWrapper::STATUS_DIALOG_FINISH) {
		uint32_t result = msgdialog->getResult();
		if (result == SCE_MSG_DIALOG_BUTTON_ID_OK) {
			_result = DialogResult::Ok;
		} else if (result == SCE_MSG_DIALOG_BUTTON_ID_NO) {
			_result = DialogResult::No;
		} else if (result == SCE_MSG_DIALOG_BUTTON_ID_YES) {
			_result = DialogResult::Yes;
		} else if (result == SCE_MSG_DIALOG_BUTTON_ID_INVALID) {
			_result = DialogResult::Cancel;
		} else if (result == SCE_MSG_DIALOG_BUTTON_ID_BUTTON1) {
			_result = DialogResult::Button1;
		} else if (result == SCE_MSG_DIALOG_BUTTON_ID_BUTTON2) {
			_result = DialogResult::Button2;
		}
	}

done:
	// Free memory.
	delete msgdialog;

	// Return a result.
	return _result;
}