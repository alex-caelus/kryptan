#ifndef PWDMENU_H
#define	PWDMENU_H

#include "../kryptan_core/core.h"
#include "DialogBase.h"

namespace Kryptan
{    
    class PwdMenu : DialogBase
    {
    public:
        PwdMenu(Core::PwdList* list, Core::Pwd* pwd);
        virtual ~PwdMenu();

        void Display(bool editmode = false);

    private:
        struct point
        {
            point(){y=-1;x=-1;};
            point(int iy, int ix){y=iy; x=ix;};
            int y; 
            int x;
        };

        enum {
            NoEdit,
            Edit,
            EditLabels
        } state;

        enum {
            Name,
            Username,
            Password
        } selectedField;

        Core::Pwd* pwd;
        Core::PwdList* list;
        point posLabels;
        point posPwd;
        int currHighlightedLabel;
        int firstVisibleLabel;
        bool passwordVisible;
        Core::PwdLabelVector selectedLabels;
        Core::PwdLabelVector allLabels;

        PwdMenu(const PwdMenu& orig);

        virtual void onInitialized(WINDOW* content) override;

        void RenderMenuBar();
        void RenderLabelList();
        void RenderPasswordDetails();

        void doFilter();
    };
}

#endif	/* PWDMENU_H */