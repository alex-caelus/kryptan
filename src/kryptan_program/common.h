#ifndef COMMON_H
#define COMMON_H

#include <panel.h>
#undef MOUSE_MOVED

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