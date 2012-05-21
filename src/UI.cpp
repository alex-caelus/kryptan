/* 
 * File:   UI.cpp
 * Author: Alexander-i7
 * 
 * Created on den 8 februari 2012, 09:58
 */

#include "UI.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <vector>
#include <list>
#include <time.h>

#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

using std::stringstream;
using std::list;
using std::string;



#define _X_OPEN_SOURCE_EXTENDED

#ifdef _WIN32
#define PDC_WIDE
//#define USE_KEYBOARD_LAYOUT_TRANSLATION_WIN_SWE //fixes windows keyboard layout error
#include <pdcurses/curses.h>
#else
/*
 * Here comes linux :)
 */
#include <curses.h>
#endif

UI* UI::_instance = NULL;

UI::UI() {
    initscr(); /* Start curses mode 		*/
    _instance = this;
    atexit(removeInstance);
    srand((unsigned int) time(0));
    useAntiKeylogger = false;
}

UI::UI(const UI& orig) {
}

UI::~UI() {
}

UI* UI::getInstance() {
    if (_instance == NULL)
        return new UI();
    else
        return _instance;
}

void UI::removeInstance() {
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
void UI::UnrecoverableError(string msg) {
    Error(msg);
    exit(EXIT_FAILURE);
}

/**
 * This prints an error to the user
 * @param msg
 */
void UI::Error(string msg) {
    showDialog(msg, ERROR_CAPTION);
}

#if 0
string UI::getOneLine() {
    noecho();
    cbreak();
    string ret;
    unsigned int maxLength = 0;
    int y, x;
    getyx(stdscr, y, x);
    int c = '\0';
    while (true) {
        do{
            c = translateKey(getch());
        }while(c == 27); //escape sequence follows
        
        if (c == KEY_BACKSPACE || c == 127 || c == '\b') {
            if (ret.length() > 0) {
                ret.erase(ret.end() - 1);
            }
        } else if (c == KEY_ENTER || c == '\n') {
            break; //Do not add '\n' to the string!
        } else {
            ret += (char)c;
        }
        if (echoKeys) {
            if (ret.length() > maxLength)
                maxLength = ret.length();
            string erase(maxLength, ' ');
            mvprintw(y, x, "%s", erase.c_str());
            mvprintw(y, x, "%s", ret.c_str());
        }
    }
    
    echo();
    nocbreak();
    return ret;
}
#endif

void UI::showDialog(string message, string caption, bool centered, bool waitForKeypress, bool hideCursor, bool clearScreen) {
    vector<string> vec;
    vec.push_back(message);
    showDialog(vec, caption, centered, waitForKeypress, hideCursor, 0, clearScreen);
}

void UI::showDialog(vector<string> message, string caption, bool centered, bool waitForKeypress, bool hideCursor, int minWidth, bool clearScreen) {
    const int HPADDING = 1;
    const int WPADDING = 2;
    const int TPADDING = 2;
    if (clearScreen)
        clear(); /* clear the screen */
    if (hideCursor)
        curs_set(0);

    int srow, scol; /* to store the number of rows and */
    getmaxyx(stdscr, srow, scol); /* get the number of rows and columns */
    //find max width of all rows
    unsigned int strMaxWidth = minWidth;
    int strHeight = message.size();
    for (int i = 0; i < strHeight; i++) {
        if (message[i].length() > strMaxWidth)
            strMaxWidth = message[i].length();
    }
    //Also check the caption width
    if (caption.length() + TPADDING * 2 > strMaxWidth)
        strMaxWidth = caption.length() + TPADDING * 2;

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
    mvprintw(rowMin, scol / 2 - caption.length() / 2, "%s", caption.c_str());
    //print message
    attron(A_BOLD); /* cut bold on */
    for (int i = 0; i < strHeight; i++) {
        int row = i + rowMin + (HPADDING + 1);
        int col;
        if (centered)
            col = scol / 2 - message[i].length() / 2;
        else
            col = colMin + (WPADDING + 1);
        mvprintw(row, col, "%s", message[i].c_str());
    }
    attroff(A_BOLD); /* Switch it off once we are done*/
    /* save our position for input selection later*/
    int selRow, selCol;
    getyx(stdscr, selRow, selCol);
    /* print the message at the center of the screen */
    mvprintw(srow - 2, 0, VERSION, srow, scol);
    mvprintw(srow - 1, 0, CREATEDBY, srow, scol);
    move(selRow, selCol);
    refresh();
    if (waitForKeypress)
        getch();
    if (hideCursor)
        curs_set(1);
}

int UI::PromtList(vector<string> message, string caption) {
    int choice = -1;
    /* Fix the vector... by adding an empty row and space for selection input*/
    if (message.size() > MAXLISTHEIGHT + RUBBEREFFEKT) {
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        bool finished = false;
        vector<string>::iterator start = message.begin();
        vector<string>::iterator uplimit = message.begin();
        vector<string>::iterator downlimit = message.end();
        vector<string>::iterator end;
        unsigned int strMaxWidth = 0;
        for (unsigned int i = 0; i < message.size(); i++) {
            if (message[i].length() > strMaxWidth)
                strMaxWidth = message[i].length();
        }
        while (!finished) {
            end = start + MAXLISTHEIGHT;
            list<string> subList(start, end);
            string dots = "";
            for (vector<string>::iterator it = message.begin(); it != start; it++) {
                dots += '.';
            }
            subList.push_front(dots);

            dots = "";
            for (vector<string>::iterator it = end; it != message.end(); it++) {
                dots += '.';
            }
            subList.push_back(dots);

            subList.push_back("");
            subList.push_back(MAKE_YOUR_SELECTION);
            showDialog(vector<string > (subList.begin(), subList.end()), caption, false, false, false, strMaxWidth);
            if (choice >= 0) {
                printw("%i", choice);
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
                stringstream tmp;
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
        message.push_back("");
        message.push_back(MAKE_YOUR_SELECTION);
        /* Start dialog */
        showDialog(message, caption, false, false, false, 4);
        /* Get choice */
        char arg1[] = "%i";
        scanw(arg1, &choice);
    }
    return choice;
}

string UI::PromtString(string message, string caption, bool canBeEmpty) {
    vector<string> vec;
    vec.push_back(message);
    return PromtString(vec, caption, canBeEmpty);
}

string UI::PromtString(vector<string> message, string caption, bool canBeEmpty) {
    message.push_back("");
    showDialog(message, caption, false, false, false);
    char str[80];
	getstr(str);
	string ret(str);
    if (!canBeEmpty && ret.empty())
        return PromtString(message, caption, canBeEmpty);
    return ret;
}

string UI::PromtPwd(string message, string caption) {
    vector<string> vec;
    vec.push_back(message);
    return PromtPwd(vec, caption);
}

string UI::PromtPwd(vector<string> message, string caption) {
    if (useAntiKeylogger)
        return PromtPwdAntiKeylogger(message, caption);
    string pwd;
    noecho();
    cbreak();
    pwd = PromtString(message, caption, false);
    nocbreak();
    echo();
    return pwd;
}

string UI::PromtPwdAntiKeylogger(vector<string> message, string caption) {
    const string REF = PASSWORD_CHARACTERS_ALL;
    string pwd = "";
    char c;
    noecho();
    cbreak();
    do {
        clear();
        string vigenere = REF;
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
            if (!pwd.empty())
                pwd.erase(pwd.end() - 1);
        } else {
            int i = vigenere.find(c);
            if (i != (int) vigenere.npos) {
                pwd += REF[i];
            }
        }
    } while (c != '\n');
    nocbreak();
    echo();
    return pwd;
}

int UI::PromtInt(string message, string caption, int min, int max) {
    vector<string> vec;
    vec.push_back(message);
    return PromtInt(vec, caption, min, max);
}

int UI::PromtInt(vector<string> message, string caption, int min, int max) {
    message.push_back("");
    showDialog(message, caption, false, false, false);
    int ans;
    char arg1[] = "%i";
    int length = scanw(arg1, &ans);
    if (length <= 0 || ans < min || ans > max)
        return PromtInt(message, caption, min, max);
    return ans;
}

bool UI::PromtBool(string message, string caption, char True, char False) {
    vector<string> vec;
    vec.push_back(message);
    return PromtBool(vec, caption, True, False);
}

bool UI::PromtBool(vector<string> message, string caption, char True, char False) {
    /* settings */
    /* dialog */
    showDialog(message, caption, false, false, true);
    /* get asnwer */
    char ans;
    char arg1[] = "%c";
    scanw(arg1, &ans);
    /* cleanup */
    /* validate */
    if (ans == True)
        return true;
    else if (ans == False)
        return false;
    return PromtBool(message, caption, True, False);
}

/**
 * This displays the main menu.
 * @return 
 */
MENU_CHOICES UI::mainMenu() {
    MENU_CHOICES MAP[] = {
        MENU_NULL, //Index zero is not used!
        MENU_SHOW_PWD,
        MENU_ADD_PWD,
        MENU_GENERATE_PWD,
        MENU_EDIT_PWD,
        MENU_REMOVE_PWD,
        MENU_CHANGE_MASTER,
        MENU_SHOW_HELP,
        MENU_QUIT
    };
    int MAPlength = sizeof (MAP) / sizeof (MENU_CHOICES);
    vector<string> list;
    list.push_back("1. " MENUSTRING_SHOW_PWD);
    list.push_back("2. " MENUSTRING_ADD_PWD);
    list.push_back("3. " MENUSTRING_GENERATE_PWD);
    list.push_back("4. " MENUSTRING_EDIT_PWD);
    list.push_back("5. " MENUSTRING_REMOVE_PWD);
    list.push_back("6. " MENUSTRING_CHANGE_MASTER);
    list.push_back("7. " MENUSTRING_SHOW_HELP);
    list.push_back("8. " MENUSTRING_QUIT);
    int c = PromtList(list, MENUTITLE);
    if (c <= 0 || c >= MAPlength)
        return mainMenu();
    else
        return MAP[c];
}
