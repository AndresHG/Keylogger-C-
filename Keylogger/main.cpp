
#include <iostream>
#include <Windows.h>
#include "Helper.h"
#include "Base64.h"
#include "KeyConstants.h"
#include "IO.h"
#include "Timer.h"

using namespace std;

int main() {
	MSG  Msg;

	//Hace que nuestra consola sea invisible
	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return 0;
}