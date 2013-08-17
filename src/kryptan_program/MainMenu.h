#ifndef MAINMENU_H
#define	MAINMENU_H

#include "../kryptan_core/core.h"
#include "DialogBase.h"

namespace Kryptan
{
    enum MenuActions
    {
        QUIT,
        NEW_PWD,
        CHANGE_MASTER,
        ABOUT
    };
    
    class MainMenu : DialogBase
    {
    public:
        MainMenu(Core::PwdFile* file);
        virtual ~MainMenu();

        MenuActions Display();
        
        Core::Pwd* GetSelectedPwd();
    private:
        struct point
        {
            point(){y=-1;x=-1;};
            point(int iy, int ix){y=iy; x=ix;};
            int y; 
            int x;
        };

        enum {
            FilterPwds,
            Labels
        } state;

        Core::PwdList* list;
        point posFilter;
        point posLabels;
        point posPwds;
        int currSelectedLabel;
        int currSelectedPwd;
        int firstVisibleLabel;
        Core::SecureString currFilter;
        Core::PwdLabelVector visibleLabels;
        Core::PwdLabelVector selectedLabels;
        Core::PwdLabelVector allLabels;
        Core::PwdList::PwdVector allPwds;

        MainMenu(const MainMenu& orig);
        void HandleKeypressFilterPwds(int c);
        void HandleKeypressLabels(int c);

        virtual void onInitialized(WINDOW* content) override;

        void InitMenuBar();
        void RenderLabelList();
        void RenderFilterBar();
        void RenderPasswordList();

        void doFilter();
    };
}

#endif	/* MAINMENU_H */
