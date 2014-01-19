#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>
#include "../kryptan_core/core.h"
#include "common.h"

namespace Kryptan{

    class Utilities{
        public:
            static void Sleep(int milliseconds);
            
            static char* ReadLine(WINDOW*, bool canabort = false);
            static char* ReadLinePwd(WINDOW*, bool canabort = false);

            static int PrintMultiline(WINDOW* w, int y, int x, int maxwidth, int maxHeight, const char* string, int length);
            
            static int CountLines(const char* string, int maxwidth);
            static int CountLines(const char* string, int length, int maxwidth);
            static int CountLines(std::string string, int maxwidth);
            static int CountLines(Core::SecureString string, int maxwidth);

            static int CountStrWidth(const char* string);
            static int CountStrWidth(const char* string, int length);
            static int CountStrWidth(std::string string);
            static int CountStrWidth(Core::SecureString string);
            
            static int GetColorPair_DEP(int, int);

    private:
            static char* _readLine(WINDOW* w, bool echoStar, bool canabort);
            static std::vector<std::pair<int, int>> colors;
    };

}

#endif