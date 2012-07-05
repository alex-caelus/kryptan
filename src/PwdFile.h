#ifndef PWDFILE_H
#define PWDFILE_H

#include "PwdTree.h"

class PwdFile{
	/* VARIBLES */
	private:
	/* VARABLES */
		PwdTree* Root;
		const char* filename;
		SecureString* masterkey;

	public:
	/* METHODS */
	private:
	/* METHODS */
		SecureString* read();
		char* Encrypt(SecureString* data, int& encryptedLength);
		SecureString* Decrypt(char* data, int length, int NrOfTriesLeft);
		void parse(SecureString*);
	public:
		PwdFile(const char* filename);
		~PwdFile();

		PwdTree* getRootPwdTree();
		
		void save();

		void changeMasterKey();
};


#endif