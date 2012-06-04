#ifndef PWD_H
#define PWD_H

#include "SecureString.h"

class Pwd{
	private:
	/* VARIBLES */
		SecureString* description;
		SecureString* username;
		SecureString* password;
	public:
	/* VARABLES */
	private:
	/* METHODS */
	public:
	/* METHODS */
		/* CONSTRUCTORS & DESTRUCTOR*/
		Pwd();
		~Pwd();

		/* Screen dialogs */
		void displayOnScreen();
		void wizardOnScreen(bool editable=true);

		/* GETTERS */
		SecureString* getDescription(){
			return description;
		}

		SecureString* getUsername(){
			return username;
		}

		SecureString* getPassword(){
			return password;
		}

		/* SETTERS */
		void setDescription(SecureString* desc){
			delete description;
			description = desc;
		}

		void setUsername(SecureString* user){
			delete username;
			username = user;
		}

		void setPassword(SecureString* pass){
			delete password;
			password = pass;
		}
};

#endif