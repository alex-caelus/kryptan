#include "Prompts.h"
#include "Utilities.h"
#include "MessageBoxes.h"
#include "string.h"

using namespace Kryptan;
using namespace Core;

SecureString GeneratePassword(int length, bool useSpecialChars);

PromptPass::PromptPass(std::string title, std::string message, bool modal)
    : DialogBase(title, 
                 Utilities::CountLines(message, getmaxx(stdscr)-2)+2, 
                 Utilities::CountStrWidth(message), 
                 getmaxy(stdscr)/2 - (Utilities::CountLines(message, getmaxx(stdscr)-2)+2)/2, 
                 getmaxx(stdscr)/2 - Utilities::CountStrWidth(message)/2, modal, Plain, 1)
{
    this->message = message;
}

SecureString PromptPass::Prompt()
{
    DialogBase::Show();
    WINDOW* w = GetWindowPtr();
    wprintw(w, (message + "\n\n").c_str());
    char* str = Utilities::ReadLinePwd(w, true);
    if(str == 0)
        throw PromtAbortException();
    SecureString s(str);
    DialogBase::Hide();
    return s;
}


PromptString::PromptString(std::string title, std::string message, bool modal)
    : DialogBase(title, 
                 Utilities::CountLines(message, getmaxx(stdscr)-2)+2, 
                 Utilities::CountStrWidth(message), 
                 getmaxy(stdscr)/2 - (Utilities::CountLines(message, getmaxx(stdscr)-2)+2)/2, 
                 getmaxx(stdscr)/2 - Utilities::CountStrWidth(message)/2, modal, Plain, 1)
{
    this->message = message;
}

SecureString PromptString::Prompt()
{
    DialogBase::Show();
    WINDOW* w = GetWindowPtr();
    message += "\n\n";
    wprintw(w, "%s", message.c_str());
    char* str = Utilities::ReadLine(w, true);
    if(str == 0)
        throw PromtAbortException();
    SecureString s(str);
    DialogBase::Hide();
    return s;
}

PromtOrGeneratePass::PromtOrGeneratePass()
    : DialogBase("New Password", 
                 5, 
                 23, 
                 getmaxy(stdscr)/2 - 5/2, 
                 getmaxx(stdscr)/2 - 23/2, false, Plain, 1)
{
}

SecureString PromtOrGeneratePass::Prompt()
{
    try
    {
        DialogBase::Show();
        WINDOW* w = GetWindowPtr();

        int bg = Utilities::GetColorPair(COLOR_WHITE, COLOR_BLACK);
        int fg = Utilities::GetColorPair(COLOR_BLACK, COLOR_WHITE);

        bool descided = false;
        bool generate = true;
        keypad(w, true);
        noecho();
        curs_set(0);
        while(!descided)
        {
            wattron(w, COLOR_PAIR(bg));
            mvwprintw(w, 0, 0, "Press [Esc] to abort\n\n");

            if(generate)
                wattron(w, COLOR_PAIR(fg));
            wprintw(w, "[Generate new password]\n\n");
            if(generate)
                wattroff(w, COLOR_PAIR(fg));

            if(!generate)
                wattron(w, COLOR_PAIR(fg));
            wprintw(w, "[Input custom password]");
            if(!generate)
                wattroff(w, COLOR_PAIR(fg));

            switch (wgetch(w))
            {
            case 27:
                //esc
                throw PromtAbortException();
                break;
            case KEY_UP:
                generate = true;
                break;
            case KEY_DOWN:
                generate = false;
                break;
            case '\n':
                descided = true;
                break;
            default:
                break;
            }
        }
        DialogBase::Hide();
        if(generate)
        {
            //generate new password
            SecureString y("y");
            SecureString n("n");
            SecureString useSpecials;
            do
            {
                PromptString p("New Password", "Use special characters? (y/n)", false);
                useSpecials = p.Prompt();
            }while(!(useSpecials == y || useSpecials == n));

            int length = -1;
            do
            {
                PromptString p("New Password", "New password length?", false);
                SecureString sLength = p.Prompt();
                length = atoi(sLength.getUnsecureString());
                sLength.UnsecuredStringFinished();
            }while(length <= 0);

            SecureString newpwd = GeneratePassword(length, useSpecials == y);
            
            SecureString keep;
            do{
                std::string s = "The generated password is:\n\n";
                s += newpwd.getUnsecureString();
                s += "\n\nDo want to save this password? (y/n)";
                newpwd.UnsecuredStringFinished();

                PromptString p("New Password", s, false);
                keep = p.Prompt();
            }while(!(keep == y || keep == n));

            if(keep == y)
                return newpwd;
            else
                throw PromtAbortException();
        }
        else
        {
            PromptString p("New Password", "Please enter the new password, abort with [Esc]", false);
            return p.Prompt();
        } 
    }
    catch(PromtAbortException)
    {
        //just rethrow
        throw;
    }
}

SecureString GeneratePassword(int length, bool useSpecialChars)
{
    const static char* azAZ09 = "0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
    const static char* SPECIALS = "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    const static int azAZ09length = strlen(azAZ09);
    const static int SPECIALSlength = strlen(SPECIALS);

    char* newPwdChar = new char[length+1];
    for(int i=0; i<length; i++){
        int j;
        if(useSpecialChars){
            j = rand()%(azAZ09length+SPECIALSlength);
        } else {
            j = rand()%(azAZ09length);
        }
        if(j < azAZ09length)
            newPwdChar[i] = azAZ09[j];
        else
            newPwdChar[i] = SPECIALS[j-azAZ09length];
    }
    newPwdChar[length] = '\0';
    return SecureString(newPwdChar); //securely deletes the 'newPwdChar' array
}