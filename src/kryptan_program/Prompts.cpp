#include "Prompts.h"
#include "Utilities.h"

using namespace Kryptan;
using namespace Core;

PromptPass::PromptPass(std::string title, std::string message, bool modal)
	: DialogBase(title, 
				 Utilities::CountLines(message)+2, 
				 Utilities::CountStrWidth(message), 
				 getmaxy(stdscr)/2 - (Utilities::CountLines(message)+2)/2, 
				 getmaxx(stdscr)/2 - Utilities::CountStrWidth(message)/2, modal, Plain, 1)
{
    this->message = message;
}

SecureString PromptPass::Prompt()
{
    DialogBase::Show();
    WINDOW* w = GetWindowPtr();
	wprintw(w, (message + "\n\n").c_str());
    wrefresh(w);
	SecureString s(Utilities::ReadLinePwd(w));
    DialogBase::Hide();
    return s;
}