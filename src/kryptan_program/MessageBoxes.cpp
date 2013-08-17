#include "MessageBoxes.h"
#include "Utilities.h"

using namespace Kryptan;

InfoBox::InfoBox(std::string title, std::string message, bool modal)
	: DialogBase(title, 
				 Utilities::CountLines(message), 
				 Utilities::CountStrWidth(message), 
				 getmaxy(stdscr)/2 - Utilities::CountLines(message)/2, 
				 getmaxx(stdscr)/2 - Utilities::CountStrWidth(message)/2, modal, Plain, 1)
{
    this->message = message;
}

#define cnf "\n\nPress [ENTER] to confirm, press any other key to abort."

ConfirmBox::ConfirmBox(std::string title, std::string message, bool modal)
	: DialogBase(title, 
				 Utilities::CountLines(message+cnf), 
				 Utilities::CountStrWidth(message+cnf), 
				 getmaxy(stdscr)/2 - (Utilities::CountLines(message+cnf))/2, 
				 getmaxx(stdscr)/2 - Utilities::CountStrWidth(message+cnf)/2, modal, Plain, 1)
{
    this->message = message+cnf;
}
