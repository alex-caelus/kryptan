#include "DialogBase.h"

#include <stdexcept>
#include "common.h"


using namespace Kryptan;

DialogBase::DialogBase(std::string title, int height, int width, int starty, int startx, bool modal,  BorderStyle border, int padding)
{
	this->title = title;
	this->height = height;
	this->width = std::min(std::max(width, (int)title.length()), getmaxx(stdscr) - (border != None ? 1 : 0)*2 - padding*2);
	this->starty = std::max(starty, (border != None ? 1 : 0) + padding);
	this->startx = std::max(startx, (border != None ? 1 : 0) + padding);
	this->modal = modal;
	this->border = border;
	this->padding = padding;
	this->window = NULL;
	this->isInitialized = false;
	this->panel = NULL;
	this->windowModal = NULL;
	this->panelModal = NULL;
	this->windowBorder = NULL;
	this->panelBorder = NULL;
    this->isShowing = false;
}

DialogBase::~DialogBase()
{
	Hide();
	if(modal)
	{
		del_panel(panelModal);
		delwin(windowModal);
	}
	if(border != None)
	{
		del_panel(panelBorder);
		delwin(windowBorder);
	}
	del_panel(panel);
	delwin(window);
}

void DialogBase::Show()
{
	if(!isInitialized)
		doInit();
    
    if(isShowing)
        return;
    isShowing = true;
	
	if(modal)
	{
		show_panel(panelModal);
		top_panel(panelModal);
	}
	if(border != None)
	{
		show_panel(panelBorder);
		top_panel(panelBorder);
	}

	show_panel(panel);
	top_panel(panel);

	update_panels();

	onShow();
	
	wrefresh(windowModal);
	wrefresh(windowBorder);
	wrefresh(window);
}

void DialogBase::Hide()
{
    if(!isShowing)
        return;
    isShowing = false;
	if(modal)
	{
		hide_panel(panelModal);
	}
	if(border != None)
	{
		hide_panel(panelBorder);
	}
	hide_panel(panel);
	update_panels();

	onHide();

	doupdate();
}

WINDOW* DialogBase::GetWindowPtr()
{
	if(window == NULL)
		throw std::logic_error("Not initialized");
	return window;
}

void DialogBase::doInit()
{
	int maxy = getmaxy(stdscr);
	int maxx = getmaxx(stdscr);
	//int starty = maxy/2-height/2;
	//int startx = maxx/2 - width/2;
	if(modal)
	{
		//modal window
		windowModal = newwin(maxy, maxx, 0, 0);
		panelModal = new_panel(windowModal);
		hide_panel(panelModal);
		onModalCreated(windowModal);
	}
	if(border != None)
	{
		//border window
		windowBorder = newwin(height + (padding+1)*2, width+ (padding+1)*2, starty - (padding+1), startx - (padding+1));
		panelBorder = new_panel(windowBorder);
		hide_panel(panelBorder);
		//draw border
		onBorderDraw(windowBorder);
		doBorder();
	}
	//DialogBase
	window = newwin(height, width, starty, startx);
	panel = new_panel(window);
	hide_panel(panel);

	//Add title
	onTitleDraw(window);
	doTitle();
    
    isInitialized = true;
    
    //initialization done
    onInitialized(window);
}

void DialogBase::doBorder()
{
	switch (border)
	{
	case Kryptan::Plain:
		box(windowBorder, 0, 0);
		break;
	case Kryptan::None:
		break;
	default:
		throw std::logic_error("Unknown border type!");
		break;
	}
}

void DialogBase::doTitle()
{
	if(border != None)
	{
		mvwprintw(windowBorder, 0, (width+ (padding+1)*2)/2 - title.length()/2, title.c_str());
	}
	else if(title.length() > 0)
	{
		throw std::logic_error("BorderStyle.None cannot have a title!");
	}
}