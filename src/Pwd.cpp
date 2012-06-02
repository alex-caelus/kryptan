#include "Pwd.h"


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
}


void Pwd::wizardOnScreen(){
}