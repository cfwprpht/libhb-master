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

#include <stdio.h>
#include <libsysmodule.h>
#include <sampleutil.h>
#include <user_service.h>
#include "ime_keyboard.h"
#include "ime_dialog_outer.h"
#include "msg_dialog_outer.h"
#include "ps4_forms.h"
#include "defines.h"

namespace ssi = sce::SampleUtil::Input;
using namespace PS4Forms;

namespace PS4Forms {
	class Ime : public sce::SampleUtil::SampleSkeleton {
	private:
		ImeDialogWrapper *imeDialog;

	public:
		Ime() : imeDialog(NULL) {}
		~Ime() {}
		virtual int initialize(void);		
		virtual int update(void);
		virtual void render(void);
		virtual int finalize(void);
		virtual void Show(wchar_t *title, wchar_t *placeholder, wchar_t *resultBuffer);
		virtual int exec(void);
	};

	class ImeKeyboard : public sce::SampleUtil::SampleSkeleton {
	private:
		SceUserServiceUserId userId;
		uint32_t state;
		LibImeKeyboardWrapper *imeKeyboard;
		bool startflag;

	public:
		ImeKeyboard() : userId(SCE_USER_SERVICE_USER_ID_INVALID), state(0), imeKeyboard(NULL), startflag(true) {}
		~ImeKeyboard() {}
		virtual int initialize(void);
		uint32_t getState(void) const;
		virtual int update(void);
		virtual void render(void);
		virtual int finalize(void);
		virtual int exec(void);
	};

	class Msg : public sce::SampleUtil::SampleSkeleton {
	private:
		MsgDialogWrapper *msgDialog;	

	public:
		Msg() {}
		~Msg() {}
		uint32_t getResult() const { return msgDialog->getResult(); }
		MsgDialogWrapper::Status getStatus() const { return msgDialog->getStatus(); }
		virtual int initialize(void);
		virtual int update(void);
		virtual void render(void);
		virtual int finalize(void);
		virtual void Show(const char *message);
		virtual void Show(const char *message, Buttons buttons, const char *button1, const char *button2);
		virtual int exec(void);
	};

	namespace Dialogs {
		class Dialog {

		};

		class ImeDialog {
		private:
			static ScePthread thread;
			static Ime *imedialog;
			static void *RunIme(void *);

		public:
			ImeDialog() {}
			~ImeDialog() {}
			static wchar_t *ShowIme(wchar_t *title, wchar_t *placeholder);
			static void ShowIme(wchar_t *title, wchar_t *placeholder, wchar_t *resultBuffer);
		};

		class MsgDialog {
		private:
			static ScePthread thread;
			static Msg *msgdialog;
			static void *RunMsg(void *);

		public:
			MsgDialog() {}
			~MsgDialog() {}
			static DialogResult ShowDialog(String message);
			static DialogResult ShowDialog(String message, Buttons buttons, const char *button1, const char *button2);
		};
	}
}