#ifndef PWDFILE_H
#define PWDFILE_H

#include "PwdTree.h"
#include "Exceptions.h"

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
		SecureString* OldDecrypt(char* data, int length, int NrOfTriesLeft);
		void parse(SecureString*);

	public:
		PwdFile(const char* filename);
		~PwdFile();

		PwdTree* getRootPwdTree();
		
		void save();

		//Needs to call save after use
		void changeMasterKey();

		std::string getClassName();
};


#endif