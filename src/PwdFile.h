#ifndef PWDFILE_H
#define PWDFILE_H

#include "PwdTree.h"

class PwdFile{
	/* VARIBLES */
	private:
	/* VARABLES */
		PwdTree* Root;
		char* filename;
		SecureString* masterkey;

	public:
	/* METHODS */
	private:
	/* METHODS */
		SecureString* read();
		char* Encrypt(SecureString* data);
		SecureString* Decrypt(char* data, int length);
		void parse(SecureString*);
		void createNewFile();
	public:
		PwdFile(char* filename);
		~PwdFile();

		PwdTree* getRootPwdTree();
		
		void save();
};


#endif