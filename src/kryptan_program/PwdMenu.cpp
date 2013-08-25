#include "PwdMenu.h"
#include "Utilities.h"
#include "Prompts.h"
#include "MessageBoxes.h"

#include <string.h>
#include <cmath>
#include <stdexcept>

#ifndef _WIN32
#define A_ITALIC A_BOLD
#endif

using namespace Kryptan;
using namespace Core;

PwdMenu::PwdMenu(Core::PwdList* list, Pwd* pwd) 
    : DialogBase("", getmaxy(stdscr), getmaxx(stdscr), 0, 0, true, None, 0)
{
    //validate input
    if(list == NULL || pwd == NULL)
        throw std::runtime_error("Arguments of PwdMenu must not be NULL");
    allLabels = list->AllLabels();
    selectedLabels = pwd->GetLabels();
    currHighlightedLabel = -1;
    firstVisibleLabel = 0;
    passwordVisible = false;
    this->pwd = pwd;
    this->list = list;
}

PwdMenu::~PwdMenu() 
{
}

void PwdMenu::Display(bool editmode)
{
    state = editmode ? Edit : NoEdit;
    this->Show();
    
    WINDOW* w = GetWindowPtr();
    

    int c = 0;
    while(true)
    {
        //do not show cursor
        curs_set(0);
        //enable function keys
        keypad(w, TRUE);
        //disable echo
        noecho();
        c = wgetch(w);
        switch (c)
        {
        case 27:
            return;
            break;
        case KEY_F(1):
            if(state == NoEdit)
            {
                state = Edit;
                selectedField = Name;
            }
            else
            {
                state = NoEdit;
                firstVisibleLabel = 0;
                currHighlightedLabel = -1;
            }
            break;
        case KEY_F(2):
            if(state != NoEdit)
            {
                try{
                    PromptString p("New Label", "Please enter the new label name, abort with [Esc]",  false);
                    SecureString newLabel = p.Prompt();
                    if (newLabel.length() > 0)
                    {
                        list->AddPwdToLabel(pwd, newLabel);
                        allLabels = list->AllLabels();
                        selectedLabels = pwd->GetLabels();
                        currHighlightedLabel = -1;
                        firstVisibleLabel = 0; 
                    }
                }
                catch(PromtAbortException){};
            }
            break;
        case KEY_F(3):
            if(state != NoEdit)
            {
                PromptString p("Delete Password", "Are you sure you want to delete this password?\nIf you are sure, type \"yes\" below.", false);
                SecureString yes("yes");
                if(yes == p.Prompt())
                {
                    list->DeletePwd(pwd);
                    //no password  to display, return to main manu
                    return;
                }
            }
            break;
        case KEY_LEFT:
            if(state == Edit)
            {
                state = EditLabels;
                currHighlightedLabel = 0;
            }
            break;
        case KEY_RIGHT:
            if(state == EditLabels)
            {
                state = Edit;
                selectedField = Name;
            }
            break;
        case KEY_DOWN:
            if(state == Edit)
            {
                if(selectedField == Name)
                    selectedField = Username;
                else if(selectedField == Username)
                    selectedField = Password;
            }
            else if(state == EditLabels)
            {
                if(currHighlightedLabel < (int)allLabels.size()-1)
                {
                    currHighlightedLabel++;
                }
            }
            break;
        case KEY_UP:
            if(state == Edit)
            {
                if(selectedField == Username)
                    selectedField = Name;
                else if(selectedField == Password)
                    selectedField = Username;
            }
            else if(state == EditLabels)
            {
                if(currHighlightedLabel > 0)
                {
                    currHighlightedLabel--;
                }
            }
            break;
        case ' ':
            if(state == NoEdit)
            {
                passwordVisible = !passwordVisible;
                break;
            }
            //continue as if enter was pressed.
        case '\n':
            if(state == Edit)
            {
                switch (selectedField)
                {
                case Kryptan::PwdMenu::Name:
                    try{
                        PromptString p("Edit Description", "Please enter the new description, abort with [Esc]",  false);
                        SecureString newName = p.Prompt();
                        if(newName.length() > 0)
                            pwd->SetDescription(newName);
                        else
                        {
                            InfoBox i("Edit Description", "Description can not be empty!", false);
                            i.Show();
                        }
                    }
                    catch(PromtAbortException){};
                    break;
                case Kryptan::PwdMenu::Username:
                    try{
                        PromptString p("Edit Username", "Please enter the new username, abort with [Esc]",  false);
                        SecureString newusername = p.Prompt();
                        pwd->SetUsername(newusername);
                    }
                    catch(PromtAbortException){};
                    break;
                case Kryptan::PwdMenu::Password:
                    try{
                        PromtOrGeneratePass p;
                        SecureString newpass = p.Prompt();
                        if(newpass.length() > 0)
                            pwd->SetPassword(newpass);
                        else
                        {
                            InfoBox i("New Password", "Password can not be empty!", false);
                            i.Show();
                        }
                    }
                    catch(PromtAbortException){};
                    break;
                default:
                    break;
                }
            }
            else if(state == EditLabels)
            {
                auto it = std::find(selectedLabels.begin(), selectedLabels.end(), allLabels[currHighlightedLabel]);
                if(selectedLabels.end() == it)
                {
                    selectedLabels.push_back(allLabels[currHighlightedLabel]);
                    list->AddPwdToLabel(pwd, allLabels[currHighlightedLabel]);
                }
                else
                {
                    selectedLabels.erase(it);
                    list->RemovePwdFromLabel(pwd, allLabels[currHighlightedLabel]);
                }
            }

        default:
            break;
        }
        RenderMenuBar();
        RenderLabelList();
        RenderPasswordDetails();
    }

    this->Hide();
    return;
}

void PwdMenu::onInitialized(WINDOW* content)
{
    RenderMenuBar();
    RenderLabelList();
    RenderPasswordDetails();
    //add credits to the bottom of the screen
    mvwprintw(content, getmaxy(content)-1, 1, "Program created by: Alexander Nilsson");
    const char* version = "Version 3.0";
    mvwprintw(content, getmaxy(content)-1, getmaxx(content)-strlen(version)-1, version);
}

void PwdMenu::RenderMenuBar()
{
    const char* Close = "Close Details(esc)";
    const int CloseStart = 1;
    const char* EditMode = "Edit Mode(F1)";
    const int EditModeStart = CloseStart + 18 +1;
    
    const char* Save = "Close Edit Mode(F1)";
    const int SaveStart = CloseStart + 18 +1;
    const char* NewLabel = "New Label(F2)";
    const int NewLabelStart = SaveStart + 19 +1;
    const char* DeleteEntry = "Delete Entry(F3)";
    const int DeleteEntryStart = NewLabelStart + 13 +1;

    WINDOW* w = GetWindowPtr();
    
    //enable function keys
    keypad(w, TRUE);
    
    int red = Utilities::GetColorPair(COLOR_WHITE, COLOR_RED);
    int blue = Utilities::GetColorPair(COLOR_WHITE, COLOR_BLUE);

    //print black background first
    mvwprintw(w, 0, 0, "\n");
    
    wattron(w, COLOR_PAIR(red));
    mvwprintw(w, 0, CloseStart, Close);
    wattroff(w, COLOR_PAIR(red));

    if (state == PwdMenu::NoEdit)
    {
        wattron(w, COLOR_PAIR(blue));
        mvwprintw(w, 0, EditModeStart, EditMode);
    }
    else
    {
        wattron(w, COLOR_PAIR(blue));
        mvwprintw(w, 0, SaveStart, Save);
        mvwprintw(w, 0, NewLabelStart, NewLabel);
        mvwprintw(w, 0, DeleteEntryStart, DeleteEntry);
    }


    wattroff(w, COLOR_PAIR(blue));
}

void PwdMenu::RenderLabelList()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 2;
    const int startx = 1;
    const int width = 30;
    int height = getmaxy(w) - 3;
    int nrOfRows = height - 4;
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
    posLabels = point(starty+3, startx+1);

    PwdLabelVector printLabels;
    if(state == NoEdit)
    {
        printLabels.assign(selectedLabels.begin(), selectedLabels.end());
    }
    else
    {
        printLabels.assign(allLabels.begin(), allLabels.end());
    }
    
    if (printLabels.size() > 0)
    {
        //create printf format string
        char format[20];
        if(state == NoEdit)
        {
#ifdef _WIN32
            sprintf_s<20>(format, "%%-%d.%ds", width-3, width-3);
#else
            snprintf(format, 20, "%%-%d.%ds", width-3, width-3);
#endif
        }
        else
        {
#ifdef _WIN32
            sprintf_s<20>(format, "[%%c] %%-%d.%ds", width-7, width-7);
#else
            snprintf(format, 20, "[%%c] %%-%d.%ds", width-7, width-7);
#endif
        }

        int visibleLabelEnd = std::min( firstVisibleLabel + nrOfRows, (int)printLabels.size());

        if(currHighlightedLabel >= visibleLabelEnd)
        {
            visibleLabelEnd = currHighlightedLabel+1;
            firstVisibleLabel = visibleLabelEnd - nrOfRows;
        }

        if(currHighlightedLabel >= 0 && currHighlightedLabel < firstVisibleLabel)
        {
            firstVisibleLabel = currHighlightedLabel;
            visibleLabelEnd = std::min( firstVisibleLabel + nrOfRows, (int)printLabels.size());
        }

        for(int i=firstVisibleLabel, j=0; i < visibleLabelEnd; i++, j++)
        {
            if(state == NoEdit)
            {
                mvwprintw(w, posLabels.y+j, posLabels.x, format , printLabels[i].getUnsecureString());
            }
            else
            {
                bool isSelected = std::find(selectedLabels.begin(), selectedLabels.end(), printLabels[i]) != selectedLabels.end();
                if(i == currHighlightedLabel && state == EditLabels)
                    wattron(w, COLOR_PAIR(fg));
                mvwprintw(w, posLabels.y+j, posLabels.x, format, isSelected ? '#' : ' ' , printLabels[i].getUnsecureString());
                if(i == currHighlightedLabel && state == EditLabels)
                    wattron(w, COLOR_PAIR(bg));
            }
            printLabels[i].UnsecuredStringFinished();
        }
        
        //print scrollbar
        if (nrOfRows < (int)printLabels.size())
        {
            double scale = nrOfRows/(double)printLabels.size();
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
    std::string header = "Labels";
    mvwprintw(w, posLabels.y-2, posLabels.x-1+ width/2 - (header.length()/2), header.c_str());
    wattroff(w, A_UNDERLINE | A_ITALIC);

    wattroff(w, COLOR_PAIR(bg) | A_BOLD);
}

void PwdMenu::RenderPasswordDetails()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 2;
    const int startx = 32;
    int width = getmaxx(w) - 33;
    int contentWidth = width - 2;
    int height = getmaxy(w) - 3;
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
    posPwd = point(starty+1, startx+1);
    
    SecureString name = pwd->GetDescription();
    SecureString username = pwd->GetUsername();
    SecureString password = pwd->GetPassword();

    int y = posPwd.y;
    int x = posPwd.x;
        
    wattron(w, A_UNDERLINE | A_ITALIC);
    mvwprintw(w, y++, x+(contentWidth/2)-2, "Name"); //print middle
    wattroff(w, A_UNDERLINE | A_ITALIC);
    if(state == Edit && selectedField == Name) wattron(w, COLOR_PAIR(fg) | A_BOLD);
    y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w)-y-1, name.getUnsecureString(), name.length());
    if(state == Edit && selectedField == Name) wattron(w, COLOR_PAIR(bg) | A_BOLD);
    name.UnsecuredStringFinished();
    
    y++; //extra space

    wattron(w, A_UNDERLINE | A_ITALIC);
    mvwprintw(w, y++, x+(contentWidth/2)-4, "Username"); //print middle
    wattroff(w, A_UNDERLINE | A_ITALIC);
    if(state == Edit && selectedField == Username) wattron(w, COLOR_PAIR(fg) | A_BOLD);
    y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w)-y-1, username.getUnsecureString(), username.length());
    username.UnsecuredStringFinished();
    if(state == Edit && selectedField == Username) wattron(w, COLOR_PAIR(bg) | A_BOLD);
    
    y++; //extra space
        
    wattron(w, A_UNDERLINE | A_ITALIC);
    mvwprintw(w, y++, x+(contentWidth/2)-4, "Password"); //print middle
    wattroff(w, A_UNDERLINE | A_ITALIC);
    if(state == Edit && selectedField == Password) wattron(w, COLOR_PAIR(fg) | A_BOLD);

    if(passwordVisible || state != NoEdit)
        y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w)-y-1, password.getUnsecureString(), password.length());
    else
    {
        wattroff(w, A_BOLD);
        y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w)-y-1, "<press-space-to-toggle>", 23);
        wattron(w, A_BOLD);
    }
        
    password.UnsecuredStringFinished();
    if(state == Edit && selectedField == Password) wattron(w, COLOR_PAIR(bg) | A_BOLD);
    
    wattroff(w, COLOR_PAIR(bg) | A_BOLD);
}

