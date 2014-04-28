#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#ifdef _WIN32
#define PDC_WIDE
#include <panel.h>
#else
#include <ncursesw/panel.h>
#endif
#undef MOUSE_MOVED

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
        virtual ~DialogBase();
		WINDOW* GetWindowPtr();
 
    protected:
        void Show();
        void Hide();

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
        bool isShowing;
        
        void doInit();
        void doBorder();
        void doTitle();
        
        virtual void onShow() {};
        virtual void onHide() {};
        virtual void onModalCreated(WINDOW*) {};
        virtual void onBorderDraw(WINDOW*) {};
        virtual void onTitleDraw(WINDOW*) {};
        virtual void onInitialized(WINDOW*) {};
    };

}

#endif
