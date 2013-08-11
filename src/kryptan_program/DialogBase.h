#ifndef DIALOG_H
#define DIALOG_H

#include <string>

#ifdef _WIN32
#define PDC_WIDE
#include <pdcurses/panel.h>
//Removes compiler warning
#undef MOUSE_MOVED
#else
#include <panel.h>
#endif

namespace Kryptan
{
	enum BorderStyle
	{
		Plain,
		None
	};
	
	class DialogBase
	{

	public:
		DialogBase(std::string title, int height, int width, int starty, int startx, bool modal,  BorderStyle border, int padding);
		~DialogBase();

	protected:
		void Show();
		void Hide();
		WINDOW* GetWindowPtr();

	private:
		WINDOW* window;
		PANEL* panel;
		WINDOW* windowModal;
		PANEL* panelModal;
		WINDOW* windowBorder;
		PANEL* panelBorder;
		std::string title;
		BorderStyle border;
		bool modal;
		int height;
		int width;
		int starty;
		int startx;
		int padding;
		bool isInitialized;
		
		void doInit();
		void doBorder();
		void doTitle();
		
		virtual void onShow() {};
		virtual void onHide() {};
		virtual void onModalCreated(WINDOW* modal) {};
		virtual void onBorderDraw(WINDOW* border) {};
		virtual void onTitleDraw(WINDOW* border) {};
	};

}

#endif