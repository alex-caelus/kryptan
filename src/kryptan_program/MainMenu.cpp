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
    this->file = file;
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
		else if (c == KEY_F(3))
			action = SYNC;
		else if (c == KEY_F(4))
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
            mvwaddch(w, posFilter.y, posFilter.x+currFilter.length(), ' ' | KRYPTAN_CONTENT_COLOR);
            wmove(w, posFilter.y, posFilter.x+currFilter.length());
            doFilter();
        }
        break;
    default:
        char str[2];
        str[0] = (char)c;
        str[1] = 0;
		currFilter.append(str, 0, false);
		mvwaddch(w, posFilter.y, posFilter.x + currFilter.length() - 1, c | KRYPTAN_CONTENT_COLOR);
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
	const char* version = "Version " KRYPTAN_CURRENT_VERSION;
    mvwprintw(content, getmaxy(content)-1, getmaxx(content)-strlen(version)-1, version);
    //move cursor to filter
    wmove(content, posFilter.y, posFilter.x);
}

void MainMenu::InitMenuBar()
{
    const char Quit[] = "Quit(esc)";
    const int QuitStart = 1;
    const char NewPwd[] = "New Password(F1)";
	const int NewPwdStart = QuitStart + sizeof(Quit);
    const char Masterkey[] = "Change Masterkey(F2)";
	const int MasterkeyStart = NewPwdStart + sizeof(NewPwd);
	const char Syncronize[] = "Syncronize(F3)";
	const int SyncronizeStart = MasterkeyStart + sizeof(Masterkey);
    const char About[] = "About(F4)";
	const int AboutStart = SyncronizeStart + sizeof(Syncronize);

    WINDOW* w = GetWindowPtr();
    
	wattron(w, KRYPTAN_EXIT_BUTTON_COLOR);
    mvwprintw(w, 0, QuitStart, Quit);
    wattroff(w, KRYPTAN_EXIT_BUTTON_COLOR);

	wattron(w, KRYPTAN_BUTTON_ROW_COLOR);
	mvwprintw(w, 0, NewPwdStart, NewPwd);
	mvwprintw(w, 0, MasterkeyStart, Masterkey);
	mvwprintw(w, 0, SyncronizeStart, Syncronize);
    mvwprintw(w, 0, AboutStart, About);


	wattroff(w, KRYPTAN_BUTTON_ROW_COLOR);
}

void MainMenu::RenderLabelList()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 2;
    const int startx = 1;
    const int width = 30;
    int height = getmaxy(w) - 3;
    int nrOfRows = height - 3;
    wattron(w, KRYPTAN_CONTENT_COLOR | A_BOLD);
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
        wchar_t format[20];
#ifdef _WIN32
        swprintf_s<20>(format, L"[%%c] %%-%d.%dS", width-7, width-7);
#else
        swnprintf(format, 20, L"[%%c] %%-%d.%dS", width-7, width-7);
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
				wattron(w, KRYPTAN_CONTENT_SELECTED_COLOR);
			int arrLen = allLabels[i].length() + 5;
			wchar_t* label = new wchar_t[arrLen];
			swprintf(label, arrLen, format, isSelected ? '#' : ' ' , (wchar_t*)allLabels[i].getUnsecureString());
			mvwaddwstr(w, posLabels.y + j, posLabels.x, label);
			memset(label, 0, arrLen);
			delete[] label;
            if(i == currHighlightedLabel && state == Labels)
				wattron(w, KRYPTAN_CONTENT_COLOR);
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

	wattroff(w, KRYPTAN_CONTENT_COLOR | A_BOLD);
}

void MainMenu::RenderFilterBar()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 2;
    const int startx = 32;
    int width = getmaxx(w) - 33;
    wattron(w, KRYPTAN_CONTENT_COLOR);
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

	//print current filter
	wchar_t* toPrint = new wchar_t[currFilter.length() + 1];
	swprintf(toPrint, currFilter.length() + 1, L"%S", (wchar_t*)currFilter.getUnsecureString());
	waddwstr(w, toPrint);
	memset(toPrint, 0, currFilter.length() + 1);
	delete[] toPrint;
	currFilter.UnsecuredStringFinished();
    
	wattroff(w, KRYPTAN_CONTENT_COLOR);
}

void MainMenu::RenderPasswordList()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 4;
    const int startx = 32;
    int width = getmaxx(w) - 33;
    int height = getmaxy(w) - 5;
    int nrOfRows = height - 3;
    wattron(w, KRYPTAN_CONTENT_COLOR | A_BOLD);
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
	wchar_t format[20];
#ifdef _WIN32
    swprintf_s<20>(format, L"%%-%d.%dS", width-2, width-2);
#else
        snprintf(format, 20, L"%%-%d.%dS", width-2, width-2);
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
				wattron(w, KRYPTAN_CONTENT_SELECTED_COLOR);
            }
            SecureString name = allPwds[i]->GetDescription();
			int arrLen = name.length() + 1;
			wchar_t* nameArr = new wchar_t[arrLen];
			swprintf(nameArr, arrLen, format, (wchar_t*)name.getUnsecureString());
			mvwaddwstr(w, posPwds.y + j, posPwds.x, nameArr);
			memset(nameArr, 0, arrLen);
			delete[] nameArr;
            if(i == currHighlightedPwd && state == FilterPwds)
            {
				wattron(w, KRYPTAN_CONTENT_COLOR);
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
    
	wattroff(w, KRYPTAN_CONTENT_COLOR | A_BOLD);
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

    allPwds = file->GetPasswordList()->Filter(currFilter, selectedLabels);
    allLabels = file->GetPasswordList()->AllLabels();
}