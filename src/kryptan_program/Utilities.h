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
			
			static char* ReadLine(WINDOW*);
			static char* ReadLinePwd(WINDOW*);
			
			static int CountLines(const char* string);
			static int CountLines(const char* string, int length);
			static int CountLines(std::string string);
			static int CountLines(Core::SecureString string);

			static int CountStrWidth(const char* string);
			static int CountStrWidth(const char* string, int length);
			static int CountStrWidth(std::string string);
			static int CountStrWidth(Core::SecureString string);
            
            static int GetColorPair(int, int);

	private:
			static char* _readLine(WINDOW* w, bool echoStar);
            static std::vector<std::pair<int, int>> colors;
	};

}

#endif