#ifndef MESSAGEBOXES_H
#define MESSAGEBOXES_H

#include "DialogBase.h"

namespace Kryptan
{
	class InfoBox : DialogBase
	{
	public:
		InfoBox(std::string title, std::string message, bool modal = true);
		void Show()
        {
            DialogBase::Show();
            //print message
            WINDOW* w = GetWindowPtr();
            wprintw(w, message.c_str());
            wrefresh(w);
            //press any key
            curs_set(0);
            getch();
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
            char c = getch();
            return c == '\n';
        }
	private:
		std::string message;
    };
};

#endif