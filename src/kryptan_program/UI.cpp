#include "UI.h"

#include <list>
#include <sstream>
#include <time.h>
#include <string.h>

#define VERSION "3.0"
#define CREATEDBY "Alexander Nilsson"

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#define _X_OPEN_SOURCE_EXTENDED

using namespace Kryptan::Program;
using namespace Kryptan::Core;

Ui* Ui::_instance = NULL;

Ui::Ui() {
	initscr(); /* Start curses mode 		*/
    _instance = this;
    srand((unsigned int) time(0));
    useAntiKeylogger = false;
    atexit(Ui::removeInstance);

	//create persistent panels
	wBlackBackground = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
	pBlackBackground = new_panel(wBlackBackground);
	wMain = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
	pMain = new_panel(wMain);
	wCredits = newwin(1, getmaxx(stdscr), 0, getmaxy(stdscr));
	pCredits = new_panel(wCredits);

	hide_panel(pMain);

	update_panels();
	doupdate();
}

Ui::Ui(const Ui&) {
}

Ui::~Ui() {
}


Ui* Ui::getInstance() {
    if (_instance == NULL)
        return new Ui();
    else
        return _instance;
}

void Ui::removeInstance() {
    if (_instance != NULL) {
        delete _instance;
        _instance = NULL;
        endwin(); /* End curses mode		  */
    }
}


SecureString Ui::GetMasterkey()
{
	//show black background
	hide_panel(pMain);
	show_panel(pBlackBackground);
	show_panel(pCredits);

	WINDOW* window = newwin(40, 10, getmaxx(stdscr)/2 - 20, getmaxy(stdscr)/2 - 5);
	PANEL* panel = new_panel(window);


}