#ifndef UI_H
#define UI_H

#include "CONSTANTS.h"
#include "UIElement.h"
#include "climits"

class Ui{
	private:
	/* VARIBLES */
		static Ui* _instance;
		bool useAntiKeylogger;
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
		void UnrecoverableError(UiElement msg);
		void Error(UiElement msg);
		void showDialog(UiElement message, UiElement caption, bool centered = true, bool waitForKeypress = true, bool hideCursor = true, bool clearScreen = true);
		void showDialog(UiElementList message, UiElement caption, bool centered = true, bool waitForKeypress = true, bool hideCursor = true, int minWidth = 30, bool clearScreen = true);
		  
		int PromtInt(UiElement msg, UiElement caption, int min = INT_MIN, int max = INT_MAX);
		int PromtInt(UiElementList msg, UiElement caption, int min = INT_MIN, int max = INT_MAX);
		bool PromtBool(UiElement msg, UiElement caption, char True = 'y', char False = 'n');
		bool PromtBool(UiElementList msg, UiElement caption, char True = 'y', char False = 'n');
		SecureString* PromtString(UiElement msg, UiElement caption, bool canBeEmpty = false);
		SecureString* PromtString(UiElementList msg, UiElement caption, bool canBeEmpty = false);
		SecureString* PromtPwd(UiElement msg, UiElement caption);
		SecureString* PromtPwd(UiElementList msg, UiElement caption);
		SecureString* PromtPwdAntiKeylogger(UiElementList msg, UiElement caption);
		int PromtList(UiElementList message, UiElement caption);
    
		void setUseAntiKeylogging(bool use){
			useAntiKeylogger = use;
		};
    
		MENU_CHOICES mainMenu();
};

#endif