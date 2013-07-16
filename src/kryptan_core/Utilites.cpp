#include "Utilities.h"



#ifdef _WIN32
    #include <windows.h>
    inline void mySleep(unsigned pMilliseconds)
    {
        ::Sleep(pMilliseconds);
    }
#else
    #include <unistd.h>
    inline void mySleep(unsigned pMilliseconds)
    {
        static const unsigned MilliToMicro = 1000;
        ::usleep(pMilliseconds * MilliToMicro);
    }
#endif

using namespace Kryptan::Core;

void Utilities::Sleep(int milliseconds){
	mySleep(milliseconds);
}