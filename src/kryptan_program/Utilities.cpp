#include "Utilities.h"

#include <string.h> 
#include <sstream>
#include <qrencode.h>
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
    if(CountCharacters(string, length) < maxWidth)
    {
        //print spaces first
        for(int i = x; i < x+maxWidth; i++)
        {
            mvwaddch(w, y, i, ' ');
        }
		wchar_t* toPrint = new wchar_t[length+2];
#ifdef _WIN32
		swprintf(toPrint, length+1, L"%S", (wchar_t*)string);
#else
        swprintf(toPrint, length+1, L"%s", (wchar_t*)string);
#endif
        toPrint[length+1] = 0;
		mvwaddwstr(w, y, x + maxWidth / 2 - length / 2, toPrint);
		memset(toPrint, 0, length+1);
		delete[] toPrint;
    }
    else
    {
#ifdef _WIN32
        int maxX = x + maxWidth;
        for(int i=0; length > 0 || i % maxWidth != 0; i++, length--)
        {
            wchar_t toPrint[2]; //char + NULL;
            swprintf(toPrint, 2, L"%C", (length > 0) ? string[i] : ' ');
            mvwaddwstr(w, y, x++, toPrint);
            memset(toPrint, 0, 2);
            if (x >= maxX && length > 0)
            {
                x -= maxWidth;
                y++;
                if (lines >= maxHeight)
                {
                    return lines;
                }
                lines++;
            }
        }
#else
        lines = 0;
        char* startLine;
        int charsInLine = 0;
        for(int byte=0; byte < length; byte++)
        {
            char c = string[byte];
            //if c is first byte of a utf-8 character
            if ((c & 0xC0) != 0x80)
            {
                if (charsInLine == 0)
                {
                    startLine = const_cast<char*>(string + byte);
                }
                charsInLine++;
                if (c == '\n' || charsInLine % maxWidth == 0 || byte >= length-1)
                {
                    //print
                    wchar_t* toPrint = new wchar_t[charsInLine + 2];
                    swprintf(toPrint, charsInLine + 1, L"%.*s", charsInLine, (wchar_t*)startLine);
                    toPrint[length + 1] = 0;
                    mvwaddwstr(w, y+lines, x, toPrint);
                    memset(toPrint, 0, charsInLine + 1);
                    delete[] toPrint;

                    //prepare for next line
                    charsInLine = 0;
                    lines++;

                    if (lines >= maxHeight)
                        return lines;
                }
            }
        }
#endif
    }
    return lines;
}

int Utilities::CountCharacters(const char* data, int dataLength)
{
#ifdef _WIN32
    //count characters according to windows current default code page
    return MultiByteToWideChar(CP_ACP, 0, data, dataLength, 0, 0);
#else
    //count utf-8 characters
    int count = 0;
    const char *p = data;
    while (*p != 0)
    {
        if (((*p) & 0xC0) != 0x80)
            count++;
        ++p;
    }
    return count;
#endif
}

int Utilities::CountCharacters(Caelus::Utilities::SecureString& data)
{
    int ret = CountCharacters(data.getUnsecureString(), data.length());
    data.UnsecuredStringFinished();
    return ret;
}

int Utilities::CountLines(const char* str, int length, int maxwidth)
{
    int lines = 1;
#ifdef _WIN32
    //windows uses code pages (single byte)
    char* lastLinebreak = (char*)str;
    for(char* p = (char *)str; p - str < length && p != '\0'; p++)
    {
        if(*p == '\n' || (p-lastLinebreak) % maxwidth == 0) 
        {
            lastLinebreak = p;
            lines++;
        }
    }
#else
    //linux uses utf-8, which may be multiple byte per character
    int nCharsInLine = 0;
    for (int byte = 0; byte < length; byte++)
    {
        char c = str[byte];
        //if c is first byte of a utf-8 character
        if ((c & 0xC0) != 0x80)
        {
            nCharsInLine++;
            if (c == '\n' || nCharsInLine % maxwidth == 0)
            {
                nCharsInLine = 0;
                lines++;
            }
        }
    }
#endif
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

int Utilities::CountLines(Caelus::Utilities::SecureString str, int maxwidth)
{
    int i = CountLines(str.getUnsecureString(), CountCharacters(str), maxwidth);
    str.UnsecuredStringFinished();
    return i;
}

int Utilities::CountStrWidth(const char* str, int length)
{
#ifdef _WIN32
    //windows uses code pages (single byte)
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
#else
    //linux uses utf-8, which may be multiple byte per character
    int curWith = 0;
    int maxWith = 0;
    for (int byte = 0; byte < length; byte++)
    {
        char c = str[byte];
        //if c is first byte of a utf-8 character
        if ((c & 0xC0) != 0x80)
        {
            curWith++;
            if (curWith > maxWith)
                maxWith = curWith;
            if (c == '\n')
            {
                curWith = 0;
            }
        }
    }
    return maxWith;
#endif
}

int Utilities::CountStrWidth(const char* str)
{
    return CountStrWidth(str, strlen(str));
}

int Utilities::CountStrWidth(string str)
{
    return CountStrWidth(str.c_str(), str.length());
}

int Utilities::CountStrWidth(Caelus::Utilities::SecureString str)
{
    int i = CountStrWidth(str.getUnsecureString(), str.length());
    str.UnsecuredStringFinished();
    return i;
}

vector<pair<int, int>> Utilities::colors;

int Utilities::GetColorPair_DEP(int fg, int bg)
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
