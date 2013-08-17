#include "MainMenu.h"
#include "Utilities.h"

#include <string.h>

using namespace Kryptan;
using namespace Core;

MainMenu::MainMenu(Core::PwdFile* file) 
    : DialogBase("", getmaxy(stdscr), getmaxx(stdscr), 0, 0, true, None, 0)
{
    list = file->GetPasswordList();
    allLabels = file->GetPasswordList()->AllLabels();
    allPwds = file->GetPasswordList()->All();
    currSelectedLabel = -1;
    currSelectedPwd = -1;
    firstVisibleLabel = 0;
}

MainMenu::~MainMenu() 
{
}

MenuActions MainMenu::Display()
{
    state = FilterPwds;
    this->Show();
    
    WINDOW* w = GetWindowPtr();
    //wait for input
    while(true){
        wmove(w, posFilter.y, posFilter.x+currFilter.length());
        int c = wgetch(w);
        if(c == 27)
            return QUIT;
        if(c == KEY_F(1))
            return NEW_PWD;
        if(c == KEY_F(2))
            return CHANGE_MASTER;
        if(c == KEY_F(3))
            return ABOUT;

        switch (state)
        {
        case FilterPwds:
            HandleKeypressFilterPwds(c);
            break;
        case Labels:
            HandleKeypressLabels(c);
            break;

        default:
            //impossible error
            KryptanQuit(2999);
        }
    }
    return QUIT;
}

void MainMenu::HandleKeypressFilterPwds(int c)
{
    WINDOW* w = GetWindowPtr();
    int blue = Utilities::GetColorPair(COLOR_WHITE, COLOR_BLUE);
    switch (c)
    {
    case KEY_RIGHT:
        break;
    case KEY_LEFT:
        //move to labels
        state = Labels;
        currSelectedPwd = -1;
        currSelectedLabel = 0;
        curs_set(0);
        RenderLabelList();
        break;
    case KEY_DOWN:
        //change pwd
        if(currSelectedPwd < (int)allPwds.size()-1)
        {
            currSelectedPwd++;
        }
        break;
    case KEY_UP:
        //change pwd
        if(currSelectedPwd > 0)
        {
            currSelectedPwd--;
        }
        break;
    case '\n':
        //do nothing
        break;
    case KEY_BACKSPACE:
    case '\b':
        //delete a character
        if(currFilter.length() > 0)
        {
            char* filter = currFilter.getUnsecureStringM();
            filter[currFilter.length()-1] = '\0';
            currFilter.UnsecuredStringFinished();
            mvwaddch(w, posFilter.y, posFilter.x+currFilter.length(), ' ' | COLOR_PAIR(blue));
            wmove(w, posFilter.y, posFilter.x+currFilter.length());
            doFilter();
        }
        break;
    default:
        char str[2];
        str[0] = (char)c;
        str[1] = 0;
        currFilter.append(str, 0, false);
        mvwaddch(w, posFilter.y, posFilter.x+currFilter.length()-1, c | COLOR_PAIR(blue));
        currSelectedPwd = 0;
        doFilter();
        break;
    }
    RenderPasswordList();
}

void MainMenu::HandleKeypressLabels(int c)
{
    WINDOW* w = GetWindowPtr();
    switch (c)
    {
    case KEY_RIGHT:
        //move to labels
        state = FilterPwds;
        currSelectedLabel = -1;
        currSelectedPwd = 0;
        curs_set(1);
        RenderPasswordList();
        break;
    case KEY_DOWN:
        //change pwd
        if(currSelectedLabel < (int)visibleLabels.size()-1)
        {
            currSelectedLabel++;
        }
        break;
    case KEY_UP:
        //change pwd
        if(currSelectedLabel > 0)
        {
            currSelectedLabel--;
        }
        break;
    case ' ':
    case '\n':
    {
        auto it = std::find(selectedLabels.begin(), selectedLabels.end(), allLabels[currSelectedLabel]);
        if(selectedLabels.end() == it)
        {
            selectedLabels.push_back(allLabels[currSelectedLabel]);
        }
        else
        {
            selectedLabels.erase(it);
        }
        doFilter();
        RenderPasswordList();
        break;
    }
    default:
        break;
    }
    RenderLabelList();
}

Pwd* MainMenu::GetSelectedPwd()
{
    return NULL;
}

void MainMenu::onInitialized(WINDOW* content)
{
    InitMenuBar();
    RenderLabelList();
    RenderFilterBar();
    RenderPasswordList();
    //add credits to the bottom of the screen
    mvwprintw(content, getmaxy(content)-1, 1, "Program created by: Alexander Nilsson");
    const char* version = "Version 3.0";
    mvwprintw(content, getmaxy(content)-1, getmaxx(content)-strlen(version)-1, version);
    //move cursor to filter
    wmove(content, posFilter.y, posFilter.x);
}

void MainMenu::InitMenuBar()
{
    const char* Quit = "Quit(esc)";
    const int QuitStart = 1;
    const char* NewPwd = "New Password(F1)";
    const int NewPwdStart = QuitStart + 9 +1;
    const char* Masterkey = "Change Masterkey(F2)";
    const int MasterkeyStart = NewPwdStart + 16 + 1;
    const char* About = "About(F3)";
    const int AboutStart = MasterkeyStart + 20 + 1;

    WINDOW* w = GetWindowPtr();
    
    //enable function keys
    keypad(w, TRUE);
    
    int red = Utilities::GetColorPair(COLOR_WHITE, COLOR_RED);
    int blue = Utilities::GetColorPair(COLOR_WHITE, COLOR_BLUE);

    wattron(w, COLOR_PAIR(red));
    mvwprintw(w, 0, QuitStart, Quit);
    wattroff(w, COLOR_PAIR(red));

    wattron(w, COLOR_PAIR(blue));
    mvwprintw(w, 0, NewPwdStart, NewPwd);
    mvwprintw(w, 0, MasterkeyStart, Masterkey);
    mvwprintw(w, 0, AboutStart, About);


    wattroff(w, COLOR_PAIR(blue));
}

void MainMenu::RenderLabelList()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 2;
    const int startx = 1;
    const int width = 30;
    int height = getmaxy(w) - 3;
    int bg = Utilities::GetColorPair(COLOR_WHITE, COLOR_GREEN);
    int fg = Utilities::GetColorPair(COLOR_GREEN, COLOR_WHITE);
    wattron(w, COLOR_PAIR(bg) | A_BOLD);
    //draw square
    for(int i = starty; i < starty + height; i++)
    {
        for(int j = startx; j < startx+width; j++)
        {
            mvwaddch(w, i, j, ' ');
        }
    }

    //store pwd pos
    posLabels = point(starty+1, startx+1);
    
    //create printf format string
    char format[20];
    sprintf_s<20>(format, "[%%c] %%-%ds", width-7);

    
    for(int i=0; i < (int)allLabels.size(); i++)
    {
        bool isSelected = selectedLabels.size() == 0 || std::find(selectedLabels.begin(), selectedLabels.end(), allLabels[i]) != selectedLabels.end();
        if(i == currSelectedLabel && state == Labels)
            wattron(w, COLOR_PAIR(fg));
        mvwprintw(w, posLabels.y+i, posLabels.x, format, isSelected ? '#' : ' ' , allLabels[i].getUnsecureString());
        if(i == currSelectedLabel && state == Labels)
            wattron(w, COLOR_PAIR(bg));
        allLabels[i].UnsecuredStringFinished();
    }

    wattroff(w, COLOR_PAIR(bg) | A_BOLD);
}

void MainMenu::RenderFilterBar()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 2;
    const int startx = 32;
    int width = getmaxx(w) - 33;
    int height = 1;
    int bg = Utilities::GetColorPair(COLOR_WHITE, COLOR_BLUE);
    wattron(w, COLOR_PAIR(bg));
    //draw line
    for(int j = startx; j < startx+width; j++)
    {
        mvwaddch(w, starty, j, ' ');
    }
    //Add label
    mvwprintw(w, starty, startx, "Filter: ");
    
    //store label filter position
    posFilter = point(getcury(w), getcurx(w));
    
    wattroff(w, COLOR_PAIR(bg));
}

void MainMenu::RenderPasswordList()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 4;
    const int startx = 32;
    int width = getmaxx(w) - 33;
    int height = getmaxy(w) - 5;
    int bg = Utilities::GetColorPair(COLOR_WHITE, COLOR_GREEN);
    int fg = Utilities::GetColorPair(COLOR_GREEN, COLOR_WHITE);
    wattron(w, COLOR_PAIR(bg) | A_BOLD);
    //draw square
    for(int i = starty; i < starty + height; i++)
    {
        for(int j = startx; j < startx+width; j++)
        {
            mvwaddch(w, i, j, ' ');
        }
    }

    //store pwd pos
    posPwds = point(starty+1, startx+1);
    
    //create printf format string
    char format[20];
    sprintf_s<20>(format, "%%-%ds", width-2);

    //print all labels
    if(currSelectedPwd == -1 && allPwds.size() > 0)
        currSelectedPwd = 0;

    for(int i=0; i < (int)allPwds.size(); i++)
    {
        if(i == currSelectedPwd && state == FilterPwds)
        {
            wattron(w, COLOR_PAIR(fg));
        }
        SecureString name = allPwds[i]->GetDescription();
        mvwprintw(w, posPwds.y+i, posPwds.x, format, name.getUnsecureString());
        if(i == currSelectedPwd && state == FilterPwds)
        {
            wattron(w, COLOR_PAIR(bg));
        }
        name.UnsecuredStringFinished();
    }
    
    wattroff(w, COLOR_PAIR(bg) | A_BOLD);
}

void MainMenu::doFilter()
{
    allPwds = list->Filter(currFilter, selectedLabels);
}