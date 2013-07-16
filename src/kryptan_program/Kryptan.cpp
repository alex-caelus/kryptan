#include "Kryptan.h"
#include "..\kryptan_core\Exceptions.h"
#include <string.h>

using namespace ::Kryptan::Program;
using namespace ::Kryptan::Core;

typedef ::Kryptan::Program::Kryptan K;

K* K::instance = NULL;

void K::run(bool useAntiKeylogging){
	//first make sure we exit correctly
	atexit(K::exit);
	//then we run the show
	try{
		if(instance){
			throw UnresolvableException(ERROR_SINGELTON);
		}
		//Set up and read password file
		instance = new Kryptan(useAntiKeylogging);
		//run program
		instance->mainloop();
	}
	catch(UnresolvableException &e){
		//e.displayOnScreen();
	}
}

void K::exit(){
	//exit program
	delete instance;
	instance = NULL;
}

K::Kryptan(bool useAntiKeylogging){
	ui = Ui::getInstance();
	ui->setUseAntiKeylogging(useAntiKeylogging);
	file = new PwdFile(PASSWORD_FILE);
}

K::~Kryptan(){
	delete file;
}

void K::mainloop(){
	enum MENU_CHOICES choice = MENU_SHOW_PWD;
	do{
		switch(choice){
            case MENU_SHOW_PWD:{
                showPwd();
                break;
            }
            case MENU_ADD_PWD:{
                addPwd();
                break;
            }
            case MENU_GENERATE_PWD:{
                generatePwd();
                break;
            }
            case MENU_EDIT_PWD:{
                editPwd();
                break;
            }
            case MENU_REMOVE_PWD:{
                removePwd();
                break;
            }
			case MENU_REMOVE_TREE:{
                removeTree();
                break;
            }
            case MENU_CHANGE_MASTER:{
                changeMaster();
                break;
            }
            case MENU_SHOW_HELP:{
                showHelp();
                break;
            }
                    default:
                        break;
		}
	}while( (choice = ui->mainMenu()) != MENU_QUIT );
}


void K::showPwd(){
	//Pwd* pass = file->getRootPwdTree()->getPasswordOnScreen(CAPTION_SHOW_PWD);
	//if(pass){
	//	pass->displayOnScreen();
	//}
}

void K::addPwd(){
	//PwdTree* tree = file->getRootPwdTree()->getTreeOnScreen(CAPTION_ADD_PWD, true, ADD_NEW_PASSWORD_HERE);
	//if(tree){
	//	Pwd* newPwd = new Pwd();
	//	tree->addPassword(newPwd);
	//	newPwd->wizardOnScreen();
	//	file->getRootPwdTree()->sortEntireSubTree();
	//	file->save();
	//}
}

void K::editPwd(){
	//Pwd* pass = file->getRootPwdTree()->getPasswordOnScreen(CAPTION_EDIT_PWD);
	//if(pass){
	//	if(ui->PromtBool(UiElement(MOVE_PASSWORD), UiElement(CAPTION_EDIT_PWD))){
	//		file->getRootPwdTree()->removePassword(pass);
	//		PwdTree* tree = file->getRootPwdTree()->getTreeOnScreen(CAPTION_EDIT_PWD, true, ADD_NEW_PASSWORD_HERE);
	//		tree->addPassword(pass);
	//	}
	//	pass->wizardOnScreen();
	//	file->getRootPwdTree()->sortEntireSubTree();
	//	file->save();
	//}
}

void K::removePwd(){
	//Pwd* pass = file->getRootPwdTree()->getPasswordOnScreen(CAPTION_REMOVE_PWD);
	//if(pass){
	//	UiElementList list;
	//	list.push_back(UiElement(REMOVE_PWD, A_NORMAL));
	//	list.push_back(UiElement(pass->getDescription(), false));
	//	list.push_back(UiElement(CONFIRM_QUESTION, A_NORMAL));

	//	if( ui->PromtBool(list, UiElement(CAPTION_REMOVE_PWD)) &&
	//		file->getRootPwdTree()->removePassword(pass))
	//	{
	//		file->save();
	//	}
	//}
}

void K::removeTree(){
	//PwdTree* tree = file->getRootPwdTree()->getTreeOnScreen(CAPTION_REMOVE_TREE, false, REMOVE_THIS_CATEGORY);
	//if(tree){
	//	if(tree->empty()){
	//		UiElementList list;
	//		list.push_back(UiElement(REMOVE_TREE_CONFIRM_1));
	//		list.push_back(UiElement(tree->getName(), false));
	//		list.push_back(UiElement(CONFIRM_QUESTION));

	//		if( ui->PromtBool(list, UiElement(CAPTION_REMOVE_TREE)) &&
	//			file->getRootPwdTree()->removeSubTree(tree))
	//		{
	//			file->save();
	//		}
	//	} else {
	//		ui->Error(UiElement(ERROR_TREE_NOT_EMPTY_ON_REMOVE));
	//	}
	//}
}

void K::generatePwd(){
	//const static char* azAZ09 = PASSWORD_CHARACTERS_ALNUM;
	//const static char* SPECIALS = PASSWORD_CHARACTERS_SPECIALS;
	//const static int azAZ09length = strlen(azAZ09);
	//const static int SPECIALSlength = strlen(SPECIALS);
	//bool useSpecials = ui->PromtBool(UiElement(USE_SPECIALS_Y_N), UiElement(CAPTION_GENERATE_PWD));
	//int length = ui->PromtInt(UiElement(GENERATE_PWD_LENGTH), UiElement(CAPTION_GENERATE_PWD));
 //   char* newPwdChar = new char[length+1];
 //   for(int i=0; i<length; i++){
 //       int j;
 //       if(useSpecials){
 //           j = rand()%(azAZ09length+SPECIALSlength);
 //       } else {
 //           j = rand()%(azAZ09length);
 //       }
 //       if(j < azAZ09length)
 //           newPwdChar[i] = azAZ09[j];
 //       else
 //           newPwdChar[i] = SPECIALS[j-azAZ09length];
 //   }
 //   newPwdChar[length] = '\0';
	//SecureString* newPassword = new SecureString(newPwdChar); //securely deletes the 'newPwdChar' array
	//UiElementList text;
	//text.push_back(UiElement(GEN_PWD_RESULT));
	//text.push_back(UiElement(newPassword, false));
	//text.push_back(UiElement(GEN_PWD_SAVE_Y_N));
	//if(ui->PromtBool(text, UiElement(CAPTION_GENERATE_PWD))){
	//	PwdTree* tree = file->getRootPwdTree()->getTreeOnScreen(CAPTION_ADD_PWD, true, ADD_NEW_PASSWORD_HERE);
	//	if(tree){
	//		Pwd* newPwd = new Pwd();
	//		tree->addPassword(newPwd);
	//		newPwd->setPassword(newPassword);
	//		newPwd->wizardOnScreen(false);
	//		file->getRootPwdTree()->sortEntireSubTree();
	//		file->save();
	//	}
	//}
}

void K::changeMaster(){
	file->changeMasterKey();
	file->save();
}

void K::showHelp(){
    const char* helptext[] = HELPTEXT;
	UiElementList text;

	int i=0;
	while(helptext[i] != 0){
		text.push_back(UiElement(helptext[i]));
		i++;
	}
	
    ui->showDialog(text, CAPTION_HELP);
}