#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN32
#include <panel.h>
#else
#include <ncursesw/panel.h>
#endif
#undef MOUSE_MOVED

#if( CHTYPE_LONG >= 2)
#define KRYPTAN_EXIT_BUTTON_COLOR A_RGB(31, 31, 31, 20, 0, 0)
#define KRYPTAN_BUTTON_ROW_COLOR A_RGB(31, 31, 31, 0, 0, 31)
#define KRYPTAN_CONTENT_COLOR A_RGB(31, 31, 31, 6, 10, 0)
#define KRYPTAN_CONTENT_SELECTED_COLOR A_RGB(6, 10, 0, 255, 255, 255)
#else
#define KRYPTAN_EXIT_BUTTON_COLOR COLOR_PAIR(Utilities::GetColorPair(COLOR_WHITE, COLOR_RED))
#define KRYPTAN_BUTTON_ROW_COLOR COLOR_PAIR(Utilities::GetColorPair(COLOR_WHITE, COLOR_BLUE))
#define KRYPTAN_CONTENT_COLOR COLOR_PAIR(Utilities::GetColorPair(COLOR_WHITE, COLOR_GREEN))
#define KRYPTAN_CONTENT_SELECTED_COLOR COLOR_PAIR(Utilities::GetColorPair(COLOR_GREEN, COLOR_WHITE))
#endif

namespace Kryptan
{
    //this is a class used to quit the program unexpectedly, it does not inherit
    //from std::exception since it should not be caught except at the highest
    //level
    class KryptanQuit 
    { 
    public:
        KryptanQuit(int exitcode)
        {
            code = exitcode;
        }
        
        int ExitCode()
        {
            return code;
        }
    private:
        int code;
    };
}

#endif