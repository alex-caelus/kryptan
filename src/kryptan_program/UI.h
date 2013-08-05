#ifndef UI_H
#define UI_H

#include "UIElement.h"
#include "climits"
#include "../kryptan_core/SecureString.h"

namespace Kryptan
{
	namespace Program
	{
		class Ui{
			private:
			/* VARIBLES */
				static Ui* _instance;
				bool useAntiKeylogger;
				
				WINDOW* wBlackBackground;
				WINDOW* wMain;
				WINDOW* wCredits;
				PANEL* pBlackBackground;
				PANEL* pMain;
				PANEL* pCredits;

			public:
			/* VARABLES */
			private:
			/* METHODS */
				Ui();
				Ui(const Ui& orig);
				~Ui();

			public:
			/* METHODS */
				static Ui* getInstance();
				static void removeInstance();

				SecureString GetMasterkey();
		};
	}
}

#endif