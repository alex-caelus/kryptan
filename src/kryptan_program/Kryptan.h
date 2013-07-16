#ifndef KRYPTAN_H
#define KRYPTAN_H

#include "CONSTANTS.h"
#include "..\kryptan_core\PwdFile.h"
#include "UI.h"

namespace Kryptan{
	namespace Program{

		class Kryptan{
			private:
			/* VARIBLES */
				static Kryptan* instance;
				::Kryptan::Core::PwdFile* file;
				Ui* ui;

			public:
			/* VARABLES */

			private:
			/* METHODS */
				Kryptan(bool useAntiKeylogging);
				~Kryptan();

				void mainloop();

				void showPwd();
				void addPwd();
				void editPwd();
				void removePwd();
				void removeTree();
				void generatePwd();
				void changeMaster();
				void showHelp();

			public:
			/* METHODS */
				static void run(bool useAntiKeylogging);
				static void exit();

				string getClassName(){return "Kryptan";};
		};

	}
}

#endif