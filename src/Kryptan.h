#ifndef KRYPTAN_H
#define KRYPTAN_H

#include "CONSTANTS.h"
#include "PwdFile.h"
#include "Ui.h"


class Kryptan{
	private:
	/* VARIBLES */
		static Kryptan* instance;
		PwdFile* file;
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
		void generatePwd();
		void changeMaster();
		void showHelp();

	public:
	/* METHODS */
		static void run(bool useAntiKeylogging);
		static void exit();
};


#endif

#if 0
	private:
	/* VARIBLES */
	public:
	/* VARABLES */
	private:
	/* METHODS */
	public:
	/* METHODS */
#endif