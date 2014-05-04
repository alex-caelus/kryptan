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

PwdMenu::PwdMenu(Core::PwdFile* file, Pwd* pwd, PwdDataModificationObserver* observer)
    : DialogBase("", getmaxy(stdscr), getmaxx(stdscr), 0, 0, true, None, 0)
{
    //validate input
	if (file == NULL || pwd == NULL)
        throw std::runtime_error("Arguments of PwdMenu must not be NULL");

	this->pwd = pwd;
	this->file = file;
	this->dataModiefiedObserver = observer;

    allLabels = file->GetPasswordList()->AllLabels();
    selectedLabels = pwd->GetLabels();
    currHighlightedLabel = -1;
    firstVisibleLabel = 0;
    passwordVisible = false;
}

PwdMenu::~PwdMenu() 
{
}

void PwdMenu::Display(bool editmode)
{
    state = editmode ? Edit : NoEdit;
	selectedField = Name;
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
                        file->GetPasswordList()->AddPwdToLabel(pwd, newLabel);
						dataModiefiedObserver->PwdDataModified();
                        allLabels = file->GetPasswordList()->AllLabels();
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
					file->GetPasswordList()->DeletePwd(pwd);
					dataModiefiedObserver->PwdDataModified();
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
						if (newName.length() > 0)
						{
							pwd->SetDescription(newName);
							dataModiefiedObserver->PwdDataModified();
						}
                        else
                        {
                            InfoBox i("Edit Description", "Description can not be empty!", false);
                            i.Show();
                        }
                    }
					catch (KryptanDuplicatePwdException &e)
					{
						InfoBox i("Error", e.what(), false);
						i.Show();
					}
                    catch(PromtAbortException){};
                    break;
                case Kryptan::PwdMenu::Username:
                    try{
                        PromptString p("Edit Username", "Please enter the new username, abort with [Esc]",  false);
                        SecureString newusername = p.Prompt();
                        pwd->SetUsername(newusername);
						dataModiefiedObserver->PwdDataModified();
                    }
                    catch(PromtAbortException){};
                    break;
                case Kryptan::PwdMenu::Password:
                    try{
                        PromtOrGeneratePass p;
                        SecureString newpass = p.Prompt();
						if (newpass.length() > 0)
						{
							pwd->SetPassword(newpass);
							dataModiefiedObserver->PwdDataModified();
						}
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
					file->GetPasswordList()->AddPwdToLabel(pwd, allLabels[currHighlightedLabel]);
					dataModiefiedObserver->PwdDataModified();
                }
                else
                {
                    selectedLabels.erase(it);
					file->GetPasswordList()->RemovePwdFromLabel(pwd, allLabels[currHighlightedLabel]);
					dataModiefiedObserver->PwdDataModified();
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
    const char* version = "Version " KRYPTAN_CURRENT_VERSION;
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

    //print black background first
    mvwprintw(w, 0, 0, "\n");
    
	wattron(w, KRYPTAN_EXIT_BUTTON_COLOR);
    mvwprintw(w, 0, CloseStart, Close);
	wattroff(w, KRYPTAN_EXIT_BUTTON_COLOR);

    if (state == PwdMenu::NoEdit)
    {
		wattron(w, KRYPTAN_BUTTON_ROW_COLOR);
        mvwprintw(w, 0, EditModeStart, EditMode);
    }
    else
    {
		wattron(w, KRYPTAN_BUTTON_ROW_COLOR);
        mvwprintw(w, 0, SaveStart, Save);
        mvwprintw(w, 0, NewLabelStart, NewLabel);
        mvwprintw(w, 0, DeleteEntryStart, DeleteEntry);
    }


	wattroff(w, KRYPTAN_BUTTON_ROW_COLOR);
}

void PwdMenu::RenderLabelList()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 2;
    const int startx = 1;
    const int width = 30;
    int height = getmaxy(w) - 3;
    int nrOfRows = height - 4;
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
        wchar_t format[20];
        if(state == NoEdit)
        {
#ifdef _WIN32
            swprintf(format, 20, L"%%-%d.%dS", width-3, width-3);
#else
            swprintf(format, 20, L"%%-%d.%ds", width-3, width-3);
#endif
        }
        else
        {
#ifdef _WIN32
            swprintf(format, 20, L"[%%c] %%-%d.%dS", width-7, width-7);
#else
            swprintf(format, 20, L"[%%c] %%-%d.%ds", width-7, width-7);
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
			int arrLen = printLabels[i].length() + 5;
            if(state == NoEdit)
            {
				wchar_t* label = new wchar_t[arrLen];
				swprintf(label, arrLen, format, printLabels[i].getUnsecureString());
                label[arrLen-1] = 0;
				mvwaddwstr(w, posLabels.y + j, posLabels.x, label);
				memset(label, 0, arrLen);
				delete[] label;
            }
            else
            {
                bool isSelected = std::find(selectedLabels.begin(), selectedLabels.end(), printLabels[i]) != selectedLabels.end();
                if(i == currHighlightedLabel && state == EditLabels)
					wattron(w, KRYPTAN_CONTENT_SELECTED_COLOR);
				wchar_t* label = new wchar_t[arrLen];
				swprintf(label, arrLen, format, isSelected ? '#' : ' ', printLabels[i].getUnsecureString());
                label[arrLen-1] = 0;
				mvwaddwstr(w, posLabels.y + j, posLabels.x, label);
				memset(label, 0, arrLen);
				delete[] label;
                if(i == currHighlightedLabel && state == EditLabels)
					wattron(w, KRYPTAN_CONTENT_COLOR);
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
				if (y < scrollStart || y >= scrollStart + scrollHeight)
					mvwaddwstr(w, y, x, L"▒");
				else
					mvwaddwstr(w, y, x, L"█");
            } 
        }
    }

    //draw header
    wattron(w, A_UNDERLINE | A_ITALIC);
    std::string header = "Labels";
    mvwprintw(w, posLabels.y-2, posLabels.x-1+ width/2 - (header.length()/2), header.c_str());
    wattroff(w, A_UNDERLINE | A_ITALIC);

	wattroff(w, KRYPTAN_CONTENT_COLOR | A_BOLD);
}

void PwdMenu::RenderPasswordDetails()
{
    WINDOW* w = GetWindowPtr();
    const int starty = 2;
    const int startx = 32;
    int width = getmaxx(w) - 33;
    int contentWidth = width - 2;
    int height = getmaxy(w) - 3;
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
    posPwd = point(starty+1, startx+1);
    
    SecureString name = pwd->GetDescription();
    SecureString username = pwd->GetUsername();
	SecureString password = pwd->GetPassword();
	std::string timeCreated = pwd->GetTimeCreatedString();
	std::string timeModified = pwd->GetTimeLastModifiedString();

    int y = posPwd.y;
    int x = posPwd.x;
        
    wattron(w, A_UNDERLINE | A_ITALIC);
    mvwprintw(w, y++, x+(contentWidth/2)-2, "Name"); //print middle
    wattroff(w, A_UNDERLINE | A_ITALIC);
	if (state == Edit && selectedField == Name) wattron(w, KRYPTAN_CONTENT_SELECTED_COLOR | A_BOLD);
    y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w) - y - 1, name.getUnsecureString(), Utilities::CountCharacters(name));
	if (state == Edit && selectedField == Name) wattron(w, KRYPTAN_CONTENT_COLOR | A_BOLD);
    name.UnsecuredStringFinished();
    
    y++; //extra space

    wattron(w, A_UNDERLINE | A_ITALIC);
    mvwprintw(w, y++, x+(contentWidth/2)-4, "Username"); //print middle
    wattroff(w, A_UNDERLINE | A_ITALIC);
	if (state == Edit && selectedField == Username) wattron(w, KRYPTAN_CONTENT_SELECTED_COLOR | A_BOLD);
	y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w) - y - 1, username.getUnsecureString(), username.length());
    username.UnsecuredStringFinished();
	if (state == Edit && selectedField == Username) wattron(w, KRYPTAN_CONTENT_COLOR | A_BOLD);
    
    y++; //extra space
        
    wattron(w, A_UNDERLINE | A_ITALIC);
    mvwprintw(w, y++, x+(contentWidth/2)-4, "Password"); //print middle
    wattroff(w, A_UNDERLINE | A_ITALIC);
	if (state == Edit && selectedField == Password) wattron(w, KRYPTAN_CONTENT_SELECTED_COLOR | A_BOLD);

    if(passwordVisible || state != NoEdit)
		y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w) - y - 1, password.getUnsecureString(), password.length());
    else
    {
        wattroff(w, A_BOLD);
		y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w) - y - 1, "<press-space-to-toggle>", 23);
        wattron(w, A_BOLD);
    }
        
    password.UnsecuredStringFinished();
	if (state == Edit && selectedField == Password) wattron(w, KRYPTAN_CONTENT_COLOR | A_BOLD);


	y++; //extra space

	wattron(w, A_UNDERLINE | A_ITALIC);
	mvwprintw(w, y++, x + (contentWidth / 2) - 9, "Last modified date"); //print middle
	wattroff(w, A_UNDERLINE | A_ITALIC);
	y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w) - y - 1, timeModified.c_str(), timeModified.length());

	y++; //extra space

	wattron(w, A_UNDERLINE | A_ITALIC);
	mvwprintw(w, y++, x + (contentWidth / 2) - 6, "Created date"); //print middle
	wattroff(w, A_UNDERLINE | A_ITALIC);
	y += Utilities::PrintMultiline(w, y, x, contentWidth, getmaxy(w) - y - 1, timeCreated.c_str(), timeCreated.length());
    
    wattroff(w, KRYPTAN_CONTENT_COLOR | A_BOLD);
}

