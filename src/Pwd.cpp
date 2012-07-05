#include "Pwd.h"
#include "UI.h"


/* CONSTRUCTORS & DESTRUCTOR*/
Pwd::Pwd(){
	description = NULL;
	username = NULL;
	password = NULL;
}

Pwd::~Pwd(){
	delete description;
	delete username;
	delete password;
}

/* Screen dialogs */
void Pwd::displayOnScreen(){
	UiElementList list;
	
	//add username
	if(username)
		list.push_back(UiElement(username, false));

	//add password
	list.push_back(UiElement(password, false));

	//show the password!
	Ui::getInstance()->showDialog(list, UiElement(description, false));
}


void Pwd::wizardOnScreen(bool editable){
	Ui* ui = Ui::getInstance();
	UiElement caption(PWD_WIZARD_CAPTION);
	//get description
	if(!description || (editable && ui->PromtBool(UiElement(CHANGE_DESCRIPTION_QUESTION), UiElement(PWD_WIZARD_CAPTION)))){
		SecureString* desc = ui->PromtString(UiElement(GET_DESCRIPTION_QUESTION), caption);
		setDescription(desc);
	}
	//get username
	if(!username || (editable && ui->PromtBool(UiElement(CHANGE_USERNAME_QUESTION), UiElement(PWD_WIZARD_CAPTION)))){
		SecureString* uname = ui->PromtString(UiElement(GET_USERNAME_QUESTION), caption, true);
		if(uname->length() == 0){
			delete uname;
			uname = NULL;
		}
		setUsername(uname);
	}
	//get password
	if(!password || (editable && ui->PromtBool(UiElement(CHANGE_PASSWORD_QUESTION), UiElement(PWD_WIZARD_CAPTION)))){
		SecureString* pass = NULL;
		SecureString* conf = NULL;
		bool cont = true;
		while(cont){
			pass = ui->PromtPwd(UiElement(ADD_PWD_PWD), caption);
			conf = ui->PromtPwd(UiElement(ADD_PWD_CONFIRM), caption);
			if(!pass->equals(*conf)){
				ui->Error(UiElement(ERROR_PWD_CONFIRM));
				delete pass;
			} else {
				cont = false;
			}
			delete conf;
		}
		setPassword(pass);
	}
}