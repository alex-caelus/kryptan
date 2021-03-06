/*  //
 * File:   main.cpp
 * Author: Alexander-i7
 *
 * Created on den 8 februari 2012, 09:49
 */

#include "Program.h"

#ifdef _WIN32

#include <windows.h>

#ifdef _DEBUG
/** Memory leak detection, visual studio **/
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

/**
 * Hack for linking with old panel.lib 
 */
FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
    return _iob;
}

/**
 * Here is windows (without console window) :)
 **/
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    #ifdef _DEBUG
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    #endif
    /* Parse command line parameters */
    int c;
    bool useAntiKeylogger = false;
    LPWSTR* arguments = CommandLineToArgvW(GetCommandLineW(), &c);

    size_t not_used;
    wchar_t wtext[50];
    mbstowcs_s(&not_used, wtext, "--use-antikeylogging", strlen("--use-antikeylogging"));

    if(c > 1 && wcscmp(arguments[1], wtext) == 0)
        useAntiKeylogger = true;

    Kryptan::Program* p = new Kryptan::Program(useAntiKeylogger);

    //enter main loop
    int exitcode = p->run();

    //delete
    delete p;

    return exitcode;
}

#elif defined __linux__

#include <string.h>

/**
 * Here comes linux :)
 */
int main(int argc, char** argv) {
    bool useAntiKeylogger = false;
    if(argc > 1 && strcmp("--use-antikeylogging", argv[1]) == 0)
        useAntiKeylogger = true;

    return Kryptan::Program(useAntiKeylogger).run();
}

#else
#error Compilation on this platform is unsupported, please feel free to add your \
own support!
#endif