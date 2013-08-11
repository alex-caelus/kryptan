#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN32
#define PDC_WIDE
#include <pdcurses/panel.h>
//Removes compiler warning
#undef MOUSE_MOVED
#else
#include <panel.h>
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