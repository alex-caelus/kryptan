#include "MainMenu.h"
#include "Utilities.h"

#include <string.h>
#include <cmath>

using namespace Kryptan;
using namespace Core;


#ifndef _WIN32
#define A_ITALIC A_BOLD
#endif

MainMenu::MainMenu(Core::PwdFile* file) 
    : DialogBase("", getmaxy(stdscr), getmaxx(stdscr), 0, 0, true, None, 0)
{
    list = file->GetPasswordList();
    currHighlightedLabel = -1;
    currHighlightedPwd = -1;
    firstVisibleLabel = 0;
    firstVisiblePwd = 0;
}

MainMenu::~MainMenu() 
{
}

MenuActions MainMenu::Display()
{
    doFilter();
    state = FilterPwds;
    this->Show();

    RenderFilterBar();
    RenderLabelList();
    RenderPasswordList();
    
    WINDOW* w = GetWindowPtr();
    MenuActions action = NO_ACTION;
    
    //enable function keys
    keypad(w, TRUE);
    curs_set(2);

    //wait for input
    while(action == NO_ACTION){
        wmove(w, posFilter.y, posFilter.x+currFilter.length());
        int c = wgetch(w);
        if(c == 27)
            action = QUIT;
        else if(c == KEY_F(1))
            action = NEW_PWD;
        else if(c == KEY_F(2))
            action = CHANGE_MASTER;
        else if(c == KEY_F(3))
            action = ABOUT;
        else
        {   
            switch (state)
            {
            case FilterPwds:
                action = HandleKeypressFilterPwds(c);
                break;
            case Labels:
                action = HandleKeypressLabels(c);
                break;

            default:
                //impossible error
                KryptanQuit(2999);
            }
        }
    }
    return action;
}

MenuActions MainMenu::HandleKeypressFilterPwds(int c)
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
        currHighlightedPwd = -1;
        currHighlightedLabel = 0;
        curs_set(0);
        RenderLabelList();
        break;
    case KEY_DOWN:
        //change pwd
        if(currHighlightedPwd < (int)allPwds.size()-1)
        {
            currHighlightedPwd++;
        }
        break;
    case KEY_UP:
        //change pwd
        if(currHighlightedPwd > 0)
        {
            currHighlightedPwd--;
        }
        break;
    case '\n':
        if(currHighlightedPwd >= 0)
            return OPEN_PWD;
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
        currHighlightedPwd = 0;
        doFilter();
        break;
    }
    RenderPasswordList();

    return NO_ACTION;
}

MenuActions MainMenu::HandleKeypressLabels(int c)
{
    switch (c)
    {
    case KEY_RIGHT:
        //move to labels
        state = FilterPwds;
        currHighlightedLabel = -1;
        currHighlightedPwd = 0;
        curs_set(1);
        RenderPasswordList();
        break;
    case KEY_DOWN:
        //change pwd
        if(currHighlightedLabel < (int)allLabels.size()-1)
        {
            currHighlightedLabel++;
        }
        break;
    case KEY_UP:
        //change pwd
        if(currHighlightedLabel > 0)
        {
            currHighlightedLabel--;
        }
        break;
    case ' ':
    case '\n':
    {
        auto it = std::find(selectedLabels.begin(), selectedLabels.end(), allLabels[currHighlightedLabel]);
        if(selectedLabels.end() == it)
        {
            selectedLabels.push_back(allLabels[currHighlightedLabel]);
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
    return NO_ACTION;
}

Pwd* MainMenu::GetSelectedPwd()
{
    if(currHighlightedPwd >= 0 && currHighlightedPwd < (int)allPwds.size())
        return allPwds[currHighlightedPwd];
    else
        return 0;
}

void MainMenu::onInitialized(WINDOW* content)
{
    InitMenuBar();
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
    int nrOfRows = height - 3;
    int bg = Utilities::GetColorPair(COLOR_WHITE, COLOR_GREEN);
    int fg = Utilities::GetColorPair(COLOR_GREEN, COLOR_BLACK);
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
    posLabels = point(starty+2, startx+1);
    
    if (allLabels.size() > 0)
    {
        //create printf format string
        char format[20];
#ifdef _WIN32
        sprintf_s<20>(format, "[%%c] %%-%d.%ds", width-7, width-7);
#else
        snprintf(format, 20, "[%%c] %%-%d.%ds", width-7, width-7);
#endif

        int visibleLabelEnd = std::min( firstVisibleLabel + nrOfRows, (int)allLabels.size());

        if(currHighlightedLabel >= visibleLabelEnd)
        {
            visibleLabelEnd = currHighlightedLabel+1;
            firstVisibleLabel = visibleLabelEnd - nrOfRows;
        }

        if(currHighlightedLabel >= 0 && currHighlightedLabel < firstVisibleLabel)
        {
            firstVisibleLabel = currHighlightedLabel;
            visibleLabelEnd = std::min( firstVisibleLabel + nrOfRows, (int)allLabels.size());
        }

        for(int i=firstVisibleLabel, j=0; i < visibleLabelEnd; i++, j++)
        {
            bool isSelected = std::find(selectedLabels.begin(), selectedLabels.end(), allLabels[i]) != selectedLabels.end();
            if(i == currHighlightedLabel && state == Labels)
                wattron(w, COLOR_PAIR(fg));
            mvwprintw(w, posLabels.y+j, posLabels.x, format, isSelected ? '#' : ' ' , allLabels[i].getUnsecureString());
            if(i == currHighlightedLabel && state == Labels)
                wattron(w, COLOR_PAIR(bg));
            allLabels[i].UnsecuredStringFinished();
        }
        
        //print scrollbar
        if (nrOfRows < (int)allLabels.size())
        {
            double scale = nrOfRows/(double)allLabels.size();
            int x = posLabels.x+width-2;
            int ymax = posLabels.y+nrOfRows;
            int scrollHeight = std::max(1, (int)std::floor(scale * nrOfRows + 0.5));
            int scrollStart = (int)std::floor(firstVisibleLabel*scale + 0.5) + posLabels.y;
            for(int y=posLabels.y; y<ymax; y++)
            {
                if(y < scrollStart || y >= scrollStart + scrollHeight)
                    mvwaddch(w, y, x, '|');
                else
                    mvwaddch(w, y, x, '#');
            } 
        }
    }

    //draw header
    wattron(w, A_UNDERLINE | A_ITALIC);
    std::string header = "Filter by labels";
    mvwprintw(w, posLabels.y-2, posLabels.x-1+ width/2 - (header.length()/2), header.c_str());
    wattroff(w, A_UNDERLINE | A_ITALIC);

    wattroff(w, COLOR_PAIR(bg) | A_BOLD);
}

void MainMenu::RenderFilterBar()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 2;
    const int startx = 32;
    int width = getmaxx(w) - 33;
    int bg = Utilities::GetColorPair(COLOR_WHITE, COLOR_BLUE);
    wattron(w, COLOR_PAIR(bg));
    //draw line
    for(int j = startx; j < startx+width; j++)
    {
        mvwaddch(w, starty, j, ' ');
    }
    //Add label
    mvwprintw(w, starty, startx, "Filter: ");
    currFilter.UnsecuredStringFinished();
    
    //store label filter position
    posFilter = point(getcury(w), getcurx(w));
    wprintw(w, "%s", currFilter.getUnsecureString());
    currFilter.UnsecuredStringFinished();
    
    wattroff(w, COLOR_PAIR(bg));
}

void MainMenu::RenderPasswordList()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 4;
    const int startx = 32;
    int width = getmaxx(w) - 33;
    int height = getmaxy(w) - 5;
    int nrOfRows = height - 3;
    int bg = Utilities::GetColorPair(COLOR_WHITE, COLOR_GREEN);
    int fg = Utilities::GetColorPair(COLOR_GREEN, COLOR_BLACK);
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
    posPwds = point(starty+2, startx+1);
    
    //create printf format string
    char format[20];
#ifdef _WIN32
    sprintf_s<20>(format, "%%-%d.%ds", width-2, width-2);
#else
        snprintf(format, 20, "%%-%d.%ds", width-2, width-2);
#endif
    
    if(currHighlightedPwd == -1 && allPwds.size() > 0)
        currHighlightedPwd = 0;

    if(currHighlightedPwd >= (int)allPwds.size())
        currHighlightedPwd = allPwds.size()-1;

    if (allPwds.size() > 0)
    {
        int visiblePwdEnd = std::min( firstVisiblePwd + nrOfRows, (int)allPwds.size());

        if(currHighlightedPwd >= visiblePwdEnd)
        {
            visiblePwdEnd = currHighlightedPwd+1;
            firstVisiblePwd = visiblePwdEnd - nrOfRows;
            firstVisiblePwd = firstVisiblePwd < 0 ? 0 : firstVisiblePwd;
        }

        if(currHighlightedPwd >= 0 && currHighlightedPwd < firstVisiblePwd)
        {
            firstVisiblePwd = currHighlightedPwd;
            visiblePwdEnd = std::min( firstVisiblePwd + nrOfRows, (int)allPwds.size());
        }


        //print all pwds

        for(int i=firstVisiblePwd, j=0; i < visiblePwdEnd; i++, j++)
        {
            if(i == currHighlightedPwd && state == FilterPwds)
            {
                wattron(w, COLOR_PAIR(fg));
            }
            SecureString name = allPwds[i]->GetDescription();
            mvwprintw(w, posPwds.y+j, posPwds.x, format, name.getUnsecureString());
            if(i == currHighlightedPwd && state == FilterPwds)
            {
                wattron(w, COLOR_PAIR(bg));
            }
            name.UnsecuredStringFinished();
        }
        
        //print scrollbar
        if (nrOfRows < (int)allPwds.size())
        {
            double scale = (nrOfRows)/(double)allPwds.size();
            int x = posPwds.x+width-2;
            int ymax = posPwds.y+nrOfRows;
            int scrollHeight = std::max(1, (int)std::floor(scale * nrOfRows + 0.5));
            int scrollStart = (int)std::floor(firstVisiblePwd*scale + 0.5) + posPwds.y;
            for(int y=posPwds.y; y<ymax; y++)
            {
                if(y < scrollStart || y >= scrollStart + scrollHeight)
                    mvwaddch(w, y, x, '|');
                else
                    mvwaddch(w, y, x, '#');
            } 
        }
    }

    //draw header
    wattron(w, A_UNDERLINE | A_ITALIC);
    std::string header = "Filter results";
    mvwprintw(w, posPwds.y-2, posPwds.x-1+ width/2 - (header.length()/2), header.c_str());
    wattroff(w, A_UNDERLINE | A_ITALIC);
    
    wattroff(w, COLOR_PAIR(bg) | A_BOLD);
}

void MainMenu::doFilter()
{
    //remove labels that are not in allLabels
    PwdLabelVector copy(selectedLabels.begin(), selectedLabels.end());
    selectedLabels.clear();
    
    for(auto it=copy.begin(); it != copy.end(); it++)
    {
        if(find(allLabels.begin(), allLabels.end(), *it) != allLabels.end())
            selectedLabels.push_back(*it);
    }

    allPwds = list->Filter(currFilter, selectedLabels);
    allLabels = list->AllLabels();
}