/* 
 * File:   Cryptospwdus.cpp
 * Author: Alexander-i7
 * 
 * Created on den 8 februari 2012, 09:51
 */


#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "Kryptan.h"

/* OS-independent sleep function */
#ifdef _WIN32
    #include <windows.h>
    inline void mySleep(unsigned pMilliseconds)
    {
        ::Sleep(pMilliseconds);
    }
#else
    #include <unistd.h>
    inline void mySleep(unsigned pMilliseconds)
    {
        static const unsigned MilliToMicro = 1000;
        ::usleep(pMilliseconds * MilliToMicro);
    }
#endif

bool Kryptan::hasRun = false;
UI* Kryptan::ui = NULL;
PasswordList* Kryptan::list = NULL;
EncryptedFile* Kryptan::pwdfile = NULL;

Kryptan::Kryptan() {
}

Kryptan::Kryptan(const Kryptan& orig) {
}

Kryptan::~Kryptan() {
}

void Kryptan::createNewFile(){
    bool createNew = ui->PromtBool(CREATE_NEW_PWD_FILE_Y_N , CAPTION_NEW_PWD_FILE);
    if(createNew && pwdfile->ready()){
		createNew = ui->PromtBool(WARNING_ERASE_CONTENT, CAPTION_NEW_PWD_FILE);
    }
    if(createNew){
        string master  = ui->PromtPwd(NEW_MASTER_PWD, CAPTION_NEW_PWD_FILE);
		string confirm = ui->PromtPwd(CONFIRM_MASTER_PWD, CAPTION_NEW_PWD_FILE);
        while(master != confirm){
            ui->Error(ERROR_KEY_CONFIRM);
			master  = ui->PromtPwd(NEW_MASTER_PWD, CAPTION_NEW_PWD_FILE);
			confirm = ui->PromtPwd(CONFIRM_MASTER_PWD, CAPTION_NEW_PWD_FILE);
        }
        if(pwdfile->createFile(master)){
            return;
        }
        ui->Error(pwdfile->getLastError());
    }
    ui->UnrecoverableError(ERROR_NO_PWD_FILE);
}

void Kryptan::setUp(){
    /* Try to open the password file */
    bool newFile = false;
    pwdfile = new EncryptedFile(PASSWORD_FILE);
    if(!pwdfile->ready()){
        ui->Error(pwdfile->getLastError());
        createNewFile();
        newFile = true;
    }
    
    /* Try to decrypt the file */
    int attempts = 0;
    while(!pwdfile->hasKey()){
		bool success = pwdfile->setKey(ui->PromtPwd(PROMT_PWD, CAPTION_DECRYPT));
		ui->showDialog(MSG_DECRYPTING, CAPTION_DECRYPT, true, false);
		mySleep(1000);
		if(!success){
            if(pwdfile->isCorrupt()){
                ui->Error(pwdfile->getLastError());
                ui->Error(ERROR_PWD_FILE_CORRUPT);
                createNewFile();
            } else {
                attempts++;
                ui->Error(pwdfile->getLastError());
                if(attempts >= MAX_FAILED_ATTEMPTS){
                    ui->UnrecoverableError(ERROR_MAX_FAILED_DECRYPTS);
                }
            }
        }
    }
    
    /* Try to parse the file */
    list = new PasswordList(pwdfile);
    if(!list->parse()){
        ui->Error(list->getLastError());
        if(ui->PromtBool(CREATE_FILESTRUCTURE_Y_N, CAPTION_FILE_STRUCTURE) &&
			( newFile || ui->PromtBool(CREATE_FILESTRUCTURE_CONFIRM_Y_N, CAPTION_FILE_STRUCTURE))){
            /* this recreates the file structure (but without any entries) */
            if(!list->save()){
                ui->UnrecoverableError(list->getLastError());
            }
        } else {
			ui->UnrecoverableError(ERROR_NO_PWD_FILE);
        }
    }
}

void Kryptan::run(bool useAntiKeylogging){
    ui = UI::getInstance();
    if(hasRun){
        ui->Error(ERROR_SINGELTON);
        return;
    }
    hasRun = true;
	if(useAntiKeylogging)
        ui->setUseAntiKeylogging(true);

    setUp();
    
    ui->showDialog(WELCOME_MSG, CAPTION_WELCOME);
    
    /* we show all passwords, as a convinience, before we display the menu*/
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
    
    ui->removeInstance();
}

void Kryptan::showPwd(){
    vector<string> tmp;
    vector<string> tmp2 = list->getList();
    tmp.push_back("0. " MENUSTRING_GO_TO_MAIN);
    tmp.push_back(MENU_SEPPARATOR);
    tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
	int index = ui->PromtList(tmp, CAPTION_SHOW_PWD); 
    if(index > 0 && index <= list->getSize()){
        if(list->hasUsername(index)){
            vector<string> tmp;
            tmp.push_back(list->getUsername(index));
            tmp.push_back(list->getPassword(index));
            ui->showDialog(tmp, list->getDescription(index));
        } else {
            ui->showDialog(list->getPassword(index), list->getDescription(index));
        }
    }
}

void Kryptan::addPwd(){
	string desc = ui->PromtString(ADD_PWD_DESC, CAPTION_ADD_PWD);
    string uname = ui->PromtString(ADD_PWD_USERNAME, CAPTION_ADD_PWD, true);
    string pwd  = ui->PromtPwd(ADD_PWD_PWD, CAPTION_ADD_PWD);
    string conf = ui->PromtPwd(ADD_PWD_CONFIRM, CAPTION_ADD_PWD);
    while(pwd != conf){
        ui->Error(ERROR_PWD_CONFIRM);
        pwd  = ui->PromtPwd(ADD_PWD_PWD, CAPTION_ADD_PWD);
        conf = ui->PromtPwd(ADD_PWD_CONFIRM, CAPTION_ADD_PWD);
    }
    if(!list->newPassword(desc, uname, pwd))
        ui->Error(list->getLastError());
}

void Kryptan::editPwd(){
    vector<string> tmp;
    vector<string> tmp2 = list->getList();
    tmp.push_back("0. " MENUSTRING_GO_TO_MAIN);
    tmp.push_back(MENU_SEPPARATOR);
    tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
    int index = ui->PromtList(tmp, CAPTION_EDIT_PWD); 
    if(index > 0 && index <= list->getSize()){
        string cap = CAPTION_EDIT_SHORT + list->getDescription(index);
        string pwd  = ui->PromtPwd(ADD_PWD_PWD, cap);
        string conf = ui->PromtPwd(ADD_PWD_CONFIRM, cap);
        while(pwd != conf){
            ui->Error(ERROR_PWD_CONFIRM);
            pwd  = ui->PromtPwd(ADD_PWD_PWD, cap);
            conf = ui->PromtPwd(ADD_PWD_CONFIRM, cap);
        }
        if(!list->setPassword(index, pwd))
            ui->Error(list->getLastError());
    }
}

void Kryptan::removePwd(){
    vector<string> tmp;
    vector<string> tmp2 = list->getList();
    tmp.push_back("0. " MENUSTRING_GO_TO_MAIN);
    tmp.push_back(MENU_SEPPARATOR);
    tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
    int index = ui->PromtList(tmp, CAPTION_REMOVE_PWD); 
    if(index > 0 && index <= list->getSize()){
        bool conf = ui->PromtBool(DELETE_PWD_Y_N, CAPTION_REMOVE_PWD_SHORT + list->getDescription(index));
        if(conf){
            if(!list->deletePassword(index))
                ui->Error(list->getLastError());
        }
    }
}

void Kryptan::generatePwd(){
    const static string azAZ09 = PASSWORD_CHARACTERS_ALNUM;
    const static string SPECIALS = PASSWORD_CHARACTERS_SPECIALS;
    const static int azAZ09length = azAZ09.length();
    const static int SPECIALSlength = SPECIALS.length();
    bool useSpecials = ui->PromtBool(USE_SPECIALS_Y_N, CAPTION_GENERATE_PWD);
    int minLength = ui->PromtInt(MIN_LENGTH_Y_N, CAPTION_GENERATE_PWD);
    int maxLength = ui->PromtInt(MAX_LENGTH_Y_N, CAPTION_GENERATE_PWD, minLength);
    int length = minLength;
    if(maxLength != minLength)
        length = rand()%(maxLength-minLength)+minLength;
    char* newPwdChar = new char[length+1];
    for(int i=0; i<length; i++){
        int j;
        if(useSpecials){
            j = rand()%(azAZ09length+SPECIALSlength);
        } else {
            j = rand()%(azAZ09length);
        }
        if(j < azAZ09length)
            newPwdChar[i] = azAZ09[j];
        else
            newPwdChar[i] = SPECIALS[j-azAZ09length];
    }
    newPwdChar[length] = '\0';
    string newPwd(newPwdChar);
    vector<string> text;
    text.push_back(GEN_PWD_RESULT);
    text.push_back(newPwd);
    text.push_back(GEN_PWD_SAVE_Y_N);
    if(ui->PromtBool(text, CAPTION_GENERATE_PWD)){
        string desc = ui->PromtString(ADD_PWD_DESC, CAPTION_ADD_PWD);
        string uname = ui->PromtString(ADD_PWD_USERNAME, CAPTION_ADD_PWD, true);
        if(!list->newPassword(desc, uname, newPwd))
            ui->Error(list->getLastError());
    }
}

void Kryptan::changeMaster(){
    string newMASTER = ui->PromtPwd(NEW_MASTER_PWD, CAPTION_EDIT_MASTER);
    string conf = ui->PromtPwd(CONFIRM_MASTER_PWD, CAPTION_EDIT_MASTER);
    while(newMASTER != conf){
        ui->Error(ERROR_KEY_CONFIRM);
        newMASTER = ui->PromtPwd(NEW_MASTER_PWD, CAPTION_EDIT_MASTER);
        conf = ui->PromtPwd(CONFIRM_MASTER_PWD, CAPTION_EDIT_MASTER);
    }
    if(!pwdfile->createFile(newMASTER))
        ui->Error(pwdfile->getLastError());
    else if(!list->save())
        ui->Error(list->getLastError());
}

void Kryptan::showHelp(){
    const char* helptext[] = HELPTEXT;
    vector<string> text = vector<string>(helptext, helptext + sizeof(helptext)/sizeof(char*) );
    ui->showDialog(text, CAPTION_HELP);
}