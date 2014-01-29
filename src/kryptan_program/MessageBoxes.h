#ifndef MESSAGEBOXES_H
#define MESSAGEBOXES_H

#include "DialogBase.h"

namespace Kryptan
{
    class InfoBox : DialogBase
    {
    public:
        InfoBox(std::string title, std::string message, bool modal = true);
        void Show(bool waitForInput = true)
        {
            DialogBase::Show();
            //print message
            WINDOW* w = GetWindowPtr();
            wprintw(w, message.c_str());
            wrefresh(w);
            //press any key
			if (waitForInput)
			{
				curs_set(0);
				getch();
			}
        }
    private:
        std::string message;
    };
    
    class ConfirmBox : DialogBase
    {
    public:
        ConfirmBox(std::string title, std::string message, bool modal = true);
        bool Confirm()
        {
            DialogBase::Show();
            //print message
            WINDOW* w = GetWindowPtr();
            wprintw(w, message.c_str());
            wrefresh(w);
            //wait for confirmation
            curs_set(0);
            int c = getch();
            return c == '\n' || c == KEY_ENTER || c == 0x1CB;//numpad enter 
        }
    private:
        std::string message;
    };
};

#endif