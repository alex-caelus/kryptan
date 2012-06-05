#include "UI.h"

#include <list>
#include <sstream>
#include <time.h>
#include <string.h>

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

#define _X_OPEN_SOURCE_EXTENDED

Ui* Ui::_instance = NULL;

Ui::Ui() {
	initscr(); /* Start curses mode 		*/
    _instance = this;
    srand((unsigned int) time(0));
    useAntiKeylogger = false;
    atexit(Ui::removeInstance);
}

Ui::Ui(const Ui& orig) {
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

/**
 * This prints an error to the user and exits the application
 * @param msg
 */
void Ui::UnrecoverableError(UiElement msg) {
    Error(msg);
    exit(EXIT_FAILURE);
}

/**
 * This prints an error to the user
 * @param msg
 */
void Ui::Error(UiElement msg) {
    showDialog(msg, UiElement(ERROR_CAPTION));
}

void Ui::showDialog(UiElement message, UiElement caption, bool centered, bool waitForKeypress, bool hideCursor, bool clearScreen) {
    UiElementList vec;
    vec.push_back(message);
    showDialog(vec, caption, centered, waitForKeypress, hideCursor, 0, clearScreen);
}

void Ui::showDialog(UiElementList message, UiElement caption, bool centered, bool waitForKeypress, bool hideCursor, int minWidth, bool clearScreen) {
    const int HPADDING = 1;
    const int WPADDING = 2;
    const int TPADDING = 2;
    if (clearScreen)
        clear(); /* clear the screen */
    if (hideCursor)
        curs_set(0);

    int srow, scol; /* to store the number of rows and columns*/
    getmaxyx(stdscr, srow, scol); /* get the number of rows and columns */

    //find max width of all rows
    unsigned int strMaxWidth = minWidth;
    int strHeight = message.size();

    for (int i = 0; i < strHeight; i++) {
        if (message[i].getStringLength() > strMaxWidth)
            strMaxWidth = message[i].getStringLength();
    }

    //Also check the caption width
    if (caption.getStringLength() + TPADDING * 2 > strMaxWidth)
        strMaxWidth = caption.getStringLength() + TPADDING * 2;

    int colMin = scol / 2 - strMaxWidth / 2 - (WPADDING + 1);
    int colMax = scol / 2 + strMaxWidth / 2 + (WPADDING + 1);
    int rowMin = round(srow / 2.0 - strHeight / 2.0 - (HPADDING + 1));
    int rowMax = round(srow / 2.0 + strHeight / 2.0 + (HPADDING + 1)) - 1; //-1 removes a bug
    //create top and bottom border
    for (int i = colMin; i <= colMax; i++) {
        move(rowMin, i);
        printw("*");
        move(rowMax, i);
        printw("*");
    }
    //create left and right border
    for (int i = rowMin; i <= rowMax; i++) {
        move(i, colMin);
        printw("*");
        move(i, colMax);
        printw("*");
    }

    //prints caption
	mvprintw(rowMin, scol / 2 - caption.getStringLength() / 2, "%s", caption.getString()); 
	caption.stringDone(); //secures the string again IMPORTANT
	
    //print message
    for (int i = 0; i < strHeight; i++) {
        int row = i + rowMin + (HPADDING + 1);
        int col;
        if (centered)
            col = scol / 2 - message[i].getStringLength() / 2;
        else
            col = colMin + (WPADDING + 1);

		//do the actual printing with correct text attributes set
		char* string = message[i].getString();
		int offset = message[i].getAttributesOffset();
		mvprintw(row, col, "%.*s", offset, string);
		attrset(message[i].getAttributes());
		printw("%s", &(string[offset]));
		message[i].stringDone(); //Secures the string again IMPORTANT
		standend();
    }


    /* save our position for input selection later*/
    int selRow, selCol;
    getyx(stdscr, selRow, selCol);

    /* print the footer at the bottom of the screen */
    mvprintw(srow - 2, 0, VERSION, srow, scol);
    mvprintw(srow - 1, 0, CREATEDBY, srow, scol);
    move(selRow, selCol);
    refresh();
    if (waitForKeypress)
        getch();
    if (hideCursor)
        curs_set(1);
}

int Ui::PromtList(UiElementList message, UiElement caption) {
    int choice = -1;
    /* Fix the vector... by adding an empty row and space for selection input*/
    if (message.size() > MAXLISTHEIGHT + RUBBEREFFEKT) {
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        bool finished = false;
        UiElementList::iterator start = message.begin();
        UiElementList::iterator uplimit = message.begin();
        UiElementList::iterator downlimit = message.end();
        UiElementList::iterator end;
        unsigned int strMaxWidth = 0;
        for (unsigned int i = 0; i < message.size(); i++) {
			if (message[i].getStringLength() > strMaxWidth)
                strMaxWidth = message[i].getStringLength();
        }
        while (!finished) {
            end = start + MAXLISTHEIGHT;
			std::list<UiElement> subList(start, end);

			{
				int nrOfDots = start - uplimit;
				char* dots = new char[nrOfDots+1];
				memset(dots, '.', nrOfDots);
				dots[nrOfDots] = '\0';
				subList.push_front(UiElement(dots));
			}

			{
				int nrOfDots = downlimit - end;
				char* dots = new char[nrOfDots+1];
				memset(dots, '.', nrOfDots);
				dots[nrOfDots] = '\0';
				subList.push_back(UiElement(dots));
			}

            subList.push_back(UiElement(""));
            subList.push_back(UiElement(MAKE_YOUR_SELECTION));
            showDialog(UiElementList(subList.begin(), subList.end()), caption, false, false, false, strMaxWidth);
            if (choice >= 0) {
                printw("%d", choice);
            }
            int c = getch();
            if (c == KEY_UP) {
                if (start != uplimit)
                    start = start - 1;
            } else if (c == KEY_DOWN) {
                if (end != downlimit)
                    start = start + 1;
            } else if ((c == KEY_ENTER || c == '\n') && choice > -1) {
                finished = true;
            } else if (c == KEY_BACKSPACE || c == 127 || c == '\b') {
                if (choice < 10)
                    choice = -1;
                else
                    choice /= 10;
            } else if (c >= '0' && c <= '9') {
                std::stringstream tmp;
                if (choice > 0)
                    tmp << choice;
                tmp << (char) c;
                tmp >> choice;
            }
        }
        keypad(stdscr, FALSE);
        echo();
        nocbreak();
    } else {
		message.push_back(UiElement(""));
        message.push_back(UiElement(MAKE_YOUR_SELECTION));
        /* Start dialog */
        showDialog(message, caption, false, false, false, 4);
        /* Get choice */
        scanw("%d", &choice);
    }
    return choice;
}

SecureString* Ui::PromtString(UiElement message, UiElement caption, bool canBeEmpty) {
    UiElementList vec;
    vec.push_back(message);
    return PromtString(vec, caption, canBeEmpty);
}

SecureString* Ui::PromtString(UiElementList message, UiElement caption, bool canBeEmpty) {
    message.push_back(UiElement(""));
    showDialog(message, caption, false, false, false);
	char* input = new char[80];
	getstr(input);
	SecureString* retstr = new SecureString(input, 80, true);
	if (!canBeEmpty && retstr->length() == 0)
        return PromtString(message, caption, canBeEmpty);
    return retstr;
}

SecureString* Ui::PromtPwd(UiElement message, UiElement caption) {
    UiElementList vec;
    vec.push_back(message);
    return PromtPwd(vec, caption);
}

SecureString* Ui::PromtPwd(UiElementList message, UiElement caption) {
    if (useAntiKeylogger)
        return PromtPwdAntiKeylogger(message, caption);
    SecureString* pwd;
    noecho();
    cbreak();
    pwd = PromtString(message, caption, false);
    nocbreak();
    echo();
    return pwd;
}

SecureString* Ui::PromtPwdAntiKeylogger(UiElementList message, UiElement caption) {
    const std::string REF = PASSWORD_CHARACTERS_ALL;
	SecureString* pwd = new SecureString();
    char c;
    noecho();
    cbreak();
    do {
        clear();
        std::string vigenere = REF;
        for (unsigned int i = 0; i < vigenere.length(); i++) {
            int r = rand() % vigenere.length();
            char t = vigenere[i];
            vigenere[i] = vigenere[r];
            vigenere[r] = t;
        }
        static const int STEP = 5;
        for (unsigned int i = STEP / 2; i < REF.length(); i += STEP) {
            mvprintw(1, i, "|");
        }
        mvprintw(0, 0, "%s", vigenere.c_str());
        mvprintw(2, 0, "%s", REF.c_str());
        //mvprintw(10, 10, "%s", pwd.c_str()); //used for debugging only!
        showDialog(message, caption, false, false, false, 30, false);
        refresh();
        c = getch();
        if (c == KEY_BACKSPACE || c == 127) {
			if (pwd->length() != 0){
				pwd->getUnsecureStringM()[pwd->length()-1] = '\0';
				pwd->UnsecuredStringFinished();
			}
        } else {
            int i = vigenere.find(c);
            if (i != (int) vigenere.npos) {
				char* add = new char[2];
				add[0] = REF[i];
				add[1] = '\0';
				pwd->append(add, 1, false);
            }
        }
    } while (c != '\n');
    nocbreak();
    echo();
    return pwd;
}

int Ui::PromtInt(UiElement message, UiElement caption, int min, int max) {
    UiElementList vec;
    vec.push_back(message);
    return PromtInt(vec, caption, min, max);
}

int Ui::PromtInt(UiElementList message, UiElement caption, int min, int max) {
    message.push_back("");
    showDialog(message, caption, true, false, false);
    int ans;
    int length = scanw("%d", &ans);
    if (length <= 0 || ans < min || ans > max)
        return PromtInt(message, caption, min, max);
    return ans;
}

bool Ui::PromtBool(UiElement message, UiElement caption, char True, char False) {
    UiElementList vec;
    vec.push_back(message);
    return PromtBool(vec, caption, True, False);
}

bool Ui::PromtBool(UiElementList message, UiElement caption, char True, char False) {
    /* dialog */
    showDialog(message, caption, true, false, true);
    /* get answer */
    char ans;
    scanw("%c", &ans);
    /* validate */
    if (ans == True)
        return true;
    else if (ans == False)
        return false;
	else
		return PromtBool(message, caption, True, False);
}

/**
 * This displays the main menu.
 * @return 
 */
MENU_CHOICES Ui::mainMenu() {
    MENU_CHOICES MAP[] = {
        MENU_NULL, //Index zero is not used!
        MENU_SHOW_PWD,
        MENU_ADD_PWD,
        MENU_GENERATE_PWD,
        MENU_EDIT_PWD,
        MENU_REMOVE_PWD,
		MENU_REMOVE_TREE,
        MENU_CHANGE_MASTER,
        MENU_SHOW_HELP,
        MENU_QUIT
    };
    int MAPlength = sizeof (MAP) / sizeof (MENU_CHOICES);
    UiElementList list;
    list.push_back(UiElement("1. " MENUSTRING_SHOW_PWD));
    list.push_back(UiElement("2. " MENUSTRING_ADD_PWD));
    list.push_back(UiElement("3. " MENUSTRING_GENERATE_PWD));
    list.push_back(UiElement("4. " MENUSTRING_EDIT_PWD));
    list.push_back(UiElement("5. " MENUSTRING_REMOVE_PWD));
    list.push_back(UiElement("6. " MENUSTRING_REMOVE_TREE));
    list.push_back(UiElement("7. " MENUSTRING_CHANGE_MASTER));
    list.push_back(UiElement("8. " MENUSTRING_SHOW_HELP));
    list.push_back(UiElement("9. " MENUSTRING_QUIT));
    int c = PromtList(list, MENUTITLE);
    if (c <= 0 || c >= MAPlength)
        return mainMenu();
    else
        return MAP[c];
}
