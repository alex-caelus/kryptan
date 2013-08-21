#include "Utilities.h"

#include <string.h> 
#include "common.h"

using namespace std;

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

using namespace Kryptan;

void Utilities::Sleep(int milliseconds){
    mySleep(milliseconds);
}

char* Utilities::ReadLine(WINDOW* w, bool canabort)
{
    return _readLine(w, false, canabort);
}

char* Utilities::ReadLinePwd(WINDOW* w, bool canabort)
{
    return _readLine(w, true, canabort);
}

char* Utilities::_readLine(WINDOW* w, bool echoStar, bool canabort){
    char * line = (char*)malloc(100);
    char * linep = line;

    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;
    
    noecho();
    curs_set(1);

    for(;;) {
        c = wgetch(w);
        
        if(canabort && c == 27)
        {
            //escape key pressed
            free(linep);
            return 0;
        }

        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = (char*)realloc((void*)linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if(c == '\n')
            break;

        *line++ = (char)c;
        
        if(c == '\b' || c == 127){ //delete or backspace
            if(line-2 >= linep){
                line -= 2;
                mvwdelch(w, getcury(w), getcurx(w)-1);
            }
        }
        else if(echoStar){
            waddch(w, '*');
        }
        else
        {
            waddch(w, c);
        }
    }
    *line = '\0';
    return linep;
}

int Utilities::PrintMultiline(WINDOW* w, int y, int x, int maxWidth, int maxHeight, const char* string, int length)
{
    int lines = 1;
    if(length < maxWidth)
    {
        //print spaces first
        for(int i = x; i < x+maxWidth; i++)
        {
            mvwaddch(w, y, i, ' ');
        }
        mvwprintw(w, y, x+maxWidth/2-length/2, string);
    }
    else
    {
        int maxX =x + maxWidth;
        for(int i=0; length > 0 || i % maxWidth != 0; i++, length--)
        {
            mvwaddch(w, y, x++, (length > 0) ? string[i] : ' ');
            if(x>=maxX && length > 0)
            {
                x -= maxWidth;
                y++;
                if(lines >= maxHeight)
                {
                    return lines;
                }
                lines++;
            }
        }
    }
    return lines;
}

int Utilities::CountLines(const char* str, int length, int maxwidth)
{
    int lines = 0;
    char* lastLinebreak = (char*)str;
    for(char* p = (char *)str; p - str < length && p != '\0'; p++)
    {
        if(*p == '\n' || (p-lastLinebreak) % maxwidth == 0) 
        {
            lastLinebreak = p;
            lines++;
        }
    }
    return lines;
}

int Utilities::CountLines(const char* str, int maxwidth)
{
    return CountLines(str, strlen(str), maxwidth);
}

int Utilities::CountLines(string str, int maxwidth)
{
    return CountLines(str.c_str(), str.length(), maxwidth);
}

int Utilities::CountLines(Core::SecureString str, int maxwidth)
{
    int i = CountLines(str.getUnsecureString(), str.length(), maxwidth);
    str.UnsecuredStringFinished();
    return i;
}

int Utilities::CountStrWidth(const char* str, int length)
{
    int curWith = 0;
    int maxWith = 0;
    for(char* p = (char *)str; p - str < length && p != '\0'; p++)
    {
        curWith++;
        if(curWith > maxWith) 
            maxWith = curWith;
        if(*p == '\n')
        {
            curWith = 0;
        }
    }
    return maxWith;
}

int Utilities::CountStrWidth(const char* str)
{
    return CountStrWidth(str, strlen(str));
}

int Utilities::CountStrWidth(string str)
{
    return CountStrWidth(str.c_str(), str.length());
}

int Utilities::CountStrWidth(Core::SecureString str)
{
    int i = CountStrWidth(str.getUnsecureString(), str.length());
    str.UnsecuredStringFinished();
    return i;
}

vector<pair<int, int>> Utilities::colors;

int Utilities::GetColorPair(int fg, int bg)
{
    if(!has_colors())
        return -1;
    int index = 0;
    //if not initialized
    if(colors.size() == 0)
    {
        //add default color pair
        colors.push_back(pair<int, int>(COLOR_WHITE, COLOR_BLACK));
    }
    auto res = std::find(colors.begin(), colors.end(), pair<int, int>(fg, bg));
    if(res == colors.end())
    {
        index = colors.size();
        init_pair((short)index, (short)fg, (short)bg);
        colors.push_back(pair<int, int>(fg, bg));
        return index;
    }
    else
    {
        return res - colors.begin();
    }
}