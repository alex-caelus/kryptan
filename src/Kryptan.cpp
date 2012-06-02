#include "Kryptan.h"
#include "Exceptions.h"

Kryptan* Kryptan::instance = NULL;

void Kryptan::run(bool useAntiKeylogging){
	//first make sure we exit correctly
	atexit(Kryptan::exit);
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
		e.displayOnScreen();
	}
}

void Kryptan::exit(){
	//exit program
	delete instance;
	instance = NULL;
}

Kryptan::Kryptan(bool useAntiKeylogging){
	ui = Ui::getInstance();
	ui->setUseAntiKeylogging(useAntiKeylogging);
	file = new PwdFile(PASSWORD_FILE);
}

Kryptan::~Kryptan(){
	delete file;
}

void Kryptan::mainloop(){
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
            case MENU_CHANGE_MASTER:{
                changeMaster();
                break;
            }
            case MENU_SHOW_HELP:{
                showHelp();
                break;
            }
		}
	}while( (choice = ui->mainMenu()) != MENU_QUIT );
}


void Kryptan::showPwd(){
	Pwd* pass = file->getRootPwdTree()->getPasswordOnScreen();
	if(pass){
		pass->displayOnScreen();
	}
}

void Kryptan::addPwd(){
	PwdTree* tree = file->getRootPwdTree()->getTreeOnScreen();
	if(!tree){
		//create new kategory
	}
}

void Kryptan::editPwd(){
}

void Kryptan::removePwd(){
}

void Kryptan::generatePwd(){
}

void Kryptan::changeMaster(){
}

void Kryptan::showHelp(){
}