/*  //
 * File:   main.cpp
 * Author: Alexander-i7
 *
 * Created on den 8 februari 2012, 09:49
 */

#include "Kryptan.h"

#ifdef _WIN32

#include <windows.h>

/**
 * Here is windows (without console window) :)
 **/
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	/* Parse command line parameters */
	int c;
	bool useAntiKeylogger = false;
	LPWSTR* arguments = CommandLineToArgvW(GetCommandLineW(), &c);

	size_t not_used;
	wchar_t wtext[50];
	mbstowcs_s(&not_used, wtext, ANTI_KEYLOGGING_ARGUMENT, strlen(ANTI_KEYLOGGING_ARGUMENT));

	if(c > 1 && wcscmp(arguments[1], wtext) == 0)
		useAntiKeylogger = true;

	Kryptan::run(useAntiKeylogger);
    return 0;
}

#else

#include <string.h>

/**
 * Here comes linux :)
 */
int main(int argc, char** argv) {
	bool useAntiKeylogger = false;
	if(argc > 1 && strcmp(ANTI_KEYLOGGING_ARGUMENT, argv[1]) == 0)
		useAntiKeylogger = true;
    Kryptan::run(useAntiKeylogger);
    return 0;
}


#endif