#include "Utilities.h"

#include <string.h> 

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

char* Utilities::ReadLine(WINDOW* w)
{
	echo();
    curs_set(1);
	return _readLine(w, false);
}

char* Utilities::ReadLinePwd(WINDOW* w)
{
	noecho();
    curs_set(1);
	return _readLine(w, true);
}

char* Utilities::_readLine(WINDOW* w, bool echoStar){
    char * line = (char*)malloc(100);
	char * linep = line;

    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = wgetch(w);
		

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

		*line++ = c;
		
		if(c == '\b' || c == 127){ //delete or backspace
			if(line > linep){
				line -= 2;
				mvwdelch(w, getcury(w), getcurx(w)-1);
				wrefresh(w);
			}
		}
		else if(echoStar){
			waddch(w, '*');
			wrefresh(w);
		}
    }
    *line = '\0';
    return linep;
}

int Utilities::CountLines(const char* str, int length)
{
	int lines = 1;
	for(char* p = (char *)str; p - str < length && p != '\0'; p++)
	{
		if(*p == '\n') lines++;
	}
	return lines;
}

int Utilities::CountLines(const char* str)
{
	return CountLines(str, strlen(str));
}

int Utilities::CountLines(string str)
{
	return CountLines(str.c_str(), str.length());
}

int Utilities::CountLines(Core::SecureString str)
{
	int i = CountLines(str.getUnsecureString(), str.length());
	str.UnsecuredStringFinished();
	return i;
}

int Utilities::CountStrWidth(const char* str, int length)
{
	int curWith = 0;
	int maxWith = 0;
	for(char* p = (char *)str; p - str < length && p != '\0'; p++)
	{
		if(*p == '\n')
		{
			curWith = 0;
		}
		curWith++;
		if(curWith > maxWith) 
			maxWith = curWith;
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