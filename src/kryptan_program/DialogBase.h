#ifndef DIALOG_H
#define DIALOG_H

#include <string>
#include <panel.h>
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
        bool isShowing;
        
        void doInit();
        void doBorder();
        void doTitle();
        
        virtual void onShow() {};
        virtual void onHide() {};
        virtual void onModalCreated(WINDOW* modal) {};
        virtual void onBorderDraw(WINDOW* border) {};
        virtual void onTitleDraw(WINDOW* border) {};
        virtual void onInitialized(WINDOW* content) {};
    };

}

#endif