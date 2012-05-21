/* 
 * File:   UI.h
 * Author: Alexander-i7
 *
 * Created on den 8 februari 2012, 09:58
 */

#ifndef UI_H
#define	UI_H

#include "CONSTANTS.h"

#include <string>
#include <vector>
#include <limits.h>

using std::string;
using std::vector;


class UI {
public:
    static UI* getInstance();
    static void removeInstance();
    void UnrecoverableError(string msg);
    void Error(string msg);
    void showDialog(string message, string caption, bool centered = true, bool waitForKeypress = true, bool hideCursor = true, bool clearScreen = true);
    void showDialog(vector<string> message, string caption, bool centered = true, bool waitForKeypress = true, bool hideCursor = true, int minWidth = 30, bool clearScreen = true);
    
    int PromtInt(string msg, string caption, int min = INT_MIN, int max = INT_MAX);
    int PromtInt(vector<string> msg, string caption, int min = INT_MIN, int max = INT_MAX);
    bool PromtBool(string msg, string caption, char True = 'y', char False = 'n');
    bool PromtBool(vector<string> msg, string caption, char True = 'y', char False = 'n');
    string PromtString(string msg, string caption, bool canBeEmpty = false);
    string PromtString(vector<string> msg, string caption, bool canBeEmpty = false);
    string PromtPwd(string msg, string caption);
    string PromtPwd(vector<string> msg, string caption);
    string PromtPwdAntiKeylogger(vector<string> msg, string caption);
    int PromtList(vector<string> message, string caption);
    
    void setUseAntiKeylogging(bool use){
        useAntiKeylogger = use;
    };
    
    MENU_CHOICES mainMenu();
private:
    static UI* _instance;
    bool useAntiKeylogger;

    UI();
    UI(const UI& orig);
    ~UI();
};

#endif