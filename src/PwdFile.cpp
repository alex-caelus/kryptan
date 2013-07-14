#include "PwdFile.h"

#include <fstream>

#include "UI.h"
#include "Exceptions.h"
#include <string.h>

#ifdef _DEBUG
//#define _DEBUG_NOENCRYPTION
#endif


PwdFile::PwdFile(const char* fname){
	masterkey = new SecureString((char*)"", 0, false);
	Root = new PwdTree(NULL);

	this->filename = fname;
	bool success=false;
	while(!success){
		try{
			parse(read());
			//okay file read and parsed :)
			//we're done!
			success = true;
		}
		catch(FileReadException &e){
			//The file was not found or readable
			e.displayOnScreen();

			try{
				if(Ui::getInstance()->PromtBool(UiElement(CREATE_NEW_PWD_FILE_Y_N), UiElement(CAPTION_NEW_PWD_FILE))){
					//So create a new file instead
					changeMasterKey();
					save();
				} else {
					//if not we give up
					throw UnresolvableException(ERROR_NO_PWD_FILE, this);
				}
			}
			catch(FileWriteException& e){
				//We could not create a file
				e.displayOnScreen();
				//Soo we give up!
				throw UnresolvableException(ERROR_NO_PWD_FILE, this);
			}
			//...and try again
		}
		catch(UnresolvableException &e){
			//just rethrow exception
			throw e;
		}
		catch(BaseException &e ){
			//This catches all other exceptions
			e.displayOnScreen();
			throw UnresolvableException(ERROR_NO_PWD_FILE, this);
		}
	}
	//inform the user of our success :)
	Ui::getInstance()->showDialog(UiElement(WELCOME_MSG), UiElement(CAPTION_WELCOME));
}

PwdFile::~PwdFile(){
	//Do not delete filename it's from an exterior source
	delete masterkey;
	delete Root;
}

PwdTree* PwdFile::getRootPwdTree(){
	return Root;
}

std::string PwdFile::getClassName() {
	return "PwdFile";
}

void PwdFile::save(){
	std::ofstream of;

	of.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	if(!of){
		throw FileWriteException(ERROR_CAPTION, ERROR_WRITE_FILE);
	}

	SecureString* contents = new SecureString();

	contents->append((char*)PASSWORD_FILE_VERSION_NUMBER NEWLINE, 0, false);

	//Get all information as string
	Root->writeTreeSecureString(contents);

	int elen;

	char* encryptedBuff = Encrypt(contents, elen);

	of.write(encryptedBuff, elen);

	//secure the information
	delete contents;
	memset(encryptedBuff, 0, elen);
	delete[] encryptedBuff;

	of.close();
}


SecureString* PwdFile::read(){
	int length;
	char* buffer;
	std::ifstream is;

	//open file
	is.open(filename, std::ios::in | std::ios::binary );

	if(!is){
		//file is not found or readable
		throw FileReadException(ERROR_CAPTION, ERROR_READ_FILE);
	}

	// get length of file:
	is.seekg (0, std::ios::end);
	length = (int)is.tellg();
	is.seekg (0, std::ios::beg);

	// allocate memory:
	buffer = new char [length+1];// the buffer is deallocated by Decrypt

	// read data as a block:
	is.read (buffer,length);
	is.close();

	//add '\0' at the end
	buffer[length] = '\0';

	//Decrypt the input (not realy neccesary since the contents is still encrypted)
	return Decrypt(buffer, length+1, MAX_FAILED_ATTEMPTS);
}


void PwdFile::parse(SecureString* filecontent){
	delete Root;
	filecontent->resetLinefeedPosition();

	//Define the parsestates
	enum {
		GET_VERSION,
		GET_NEXTENTRY,
		GET_TREENAME,
		GET_DESCRIPTION,
		GET_PASSWORD,
		GET_USERNAME
	} state = GET_VERSION;

	const char* currentLine;

	//Create a new root
	Root = new PwdTree(NULL);
	Root->setName(new SecureString((char*)ROOT_TREE_NAME, 0, false));

	PwdTree* currentTree = Root;
	Pwd* currentPwd = NULL;

	bool keepParsing = true;
	//Parse the file
	while(keepParsing){
		//get next line
		currentLine = filecontent->getUnsecureNextline();

		switch(state){
			case GET_VERSION:
				if( strcmp(currentLine, PASSWORD_FILE_VERSION_NUMBER) != 0){
					throw FileParseException(ERROR_FILE_VERSION, ERROR_FILE_VERSION_HELP);
				}
				state = GET_NEXTENTRY;
				break;
			case GET_NEXTENTRY:
				//Are we done?
				if(strlen(currentLine) == 0)
					keepParsing = false;
				if( strcmp(currentLine, TREESTART) == 0){
					//create new tree
					PwdTree* newTree = new PwdTree(currentTree);
					//add it to the current tree
					currentTree->addSubTree(newTree);
					//set the new tree as current
					currentTree = newTree;
					//get tree name
					state = GET_TREENAME;
				}
				if( strcmp(currentLine, TREEEND) == 0){
					//Move up to parent
					currentTree = currentTree->getParent();
				}
				if( strcmp(currentLine, PASSWORDSTART) == 0){
					//New password
					currentPwd = new Pwd();
					//Add it to the tree
					currentTree->addPassword(currentPwd);
					//Add information to the password
					state = GET_DESCRIPTION;
				}
				if( strcmp(currentLine, PASSWORDEND) == 0){
					//We do nothing, just keep reading the next line
				}
				break;
			case GET_TREENAME:
				{
					//Create a SecureString containing the name, but do not delete the line (yet), hence the 'false' argument
					SecureString* name = new SecureString(currentLine);
					currentTree->setName(name);
					state = GET_NEXTENTRY;
				}
				break;
			case GET_DESCRIPTION:
				{
					SecureString* desc = new SecureString(currentLine);
					currentPwd->setDescription(desc);
					state = GET_PASSWORD;
				}
				break;
			case GET_PASSWORD:
				{
					SecureString* pass = new SecureString(currentLine);
					currentPwd->setPassword(pass);
					state = GET_USERNAME;
				}
				break;
			case GET_USERNAME:
				if(strcmp(currentLine, PASSWORDEND) != 0){
					SecureString* user = new SecureString(currentLine);
					currentPwd->setUsername(user);
				}
				state = GET_NEXTENTRY;
				break;
		}

		//Delete the line, and get ready to read the next line
		filecontent->UnsecuredStringFinished();
	}
}

void PwdFile::changeMasterKey(){
	bool allowed = false;
	Ui* ui = Ui::getInstance();

	//is allowd to change key
	if(masterkey->length() == 0)
		allowed = true;
	else {
		SecureString* confirmOld = ui->PromtPwd(UiElement(OLD_MASTER_PWD), UiElement(CAPTION_EDIT_MASTER));
		if(masterkey->equals(*confirmOld)){
			allowed = true;
		}
		delete confirmOld;
	}

	//change the key
	if(allowed){
		SecureString* newMaster = ui->PromtPwd(UiElement(NEW_MASTER_PWD), UiElement(CAPTION_EDIT_MASTER));
		SecureString* newConfirm= ui->PromtPwd(UiElement(CONFIRM_MASTER_PWD), UiElement(CAPTION_EDIT_MASTER));
		while(!newMaster->equals(*newConfirm)){
			ui->Error(ERROR_KEY_CONFIRM);
			delete newMaster;
			delete newConfirm;
			newMaster = ui->PromtPwd(UiElement(NEW_MASTER_PWD), UiElement(CAPTION_EDIT_MASTER));
			newConfirm= ui->PromtPwd(UiElement(CONFIRM_MASTER_PWD), UiElement(CAPTION_EDIT_MASTER));
		}
		delete masterkey;
		masterkey = newMaster;
		delete newConfirm;
	}
}

#ifndef _DEBUG_NOENCRYPTION

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

#include "cryptography.h"
#include <cryptopp/hex.h>
#include <stdlib.h>

using namespace CryptoPP; // Cryptlib source uses a namespace

#endif

char* PwdFile::Encrypt(SecureString* data, int& encryptedLength) {
#ifdef _DEBUG_NOENCRYPTION
    char* oldBuff = data->getUnsecureString();
    char* newBuff = new char[data->length() + 1];
    strcpy(newBuff, oldBuff);
    data->UnsecuredStringFinished();
    encryptedLength = data->length();
    return newBuff;
#else
    try {
        ModifiedEncryptorWithMAC encryptor(masterkey->getUnsecureString(), new HexEncoder());
        encryptor.Put((byte*) data->getUnsecureString(), data->length());
        encryptor.MessageEnd();

        encryptedLength = (int) encryptor.MaxRetrievable();
        char* newBuff = new char[encryptedLength + 1];
        encryptor.Get((byte*) newBuff, encryptedLength);
        newBuff[encryptedLength] = 0;
        masterkey->UnsecuredStringFinished();
        data->UnsecuredStringFinished();
        return newBuff;
    } catch (Exception const& e) {
        throw FileEncryptException(ERROR_ENCRYPTION_CAPTION, (char*) e.GetWhat().c_str());
        return NULL;
    }
#endif
}

SecureString* PwdFile::Decrypt(char* data, int length, int nrOfTriesLeft) {
#ifdef _DEBUG_NOENCRYPTION
    //data must be deallocated by SecureString
    return new SecureString(data, length, true);
#else
    ModifiedDecryptorWithMAC *p;
    Ui* ui = Ui::getInstance();

    if (masterkey->length() == 0) {
        masterkey = ui->PromtPwd(UiElement(PROMT_PWD), UiElement(CAPTION_DECRYPT));
    }

    try {
        HexDecoder decryptor(p = new ModifiedDecryptorWithMAC(masterkey->getUnsecureString()));
        decryptor.Put((byte*) data, length);
        decryptor.MessageEnd();

        int outputLength = (int)decryptor.MaxRetrievable();
        char* unsafeDecrypt = new char[outputLength + 1];
        decryptor.Get((byte*) unsafeDecrypt, outputLength);
        unsafeDecrypt[outputLength] = 0;

        masterkey->UnsecuredStringFinished();

        return new SecureString(unsafeDecrypt); //This deletes 'unsafeDecrypt' safely
    } 
    catch (ModifiedDecryptor::KeyBadErr) 
    {
		masterkey->UnsecuredStringFinished(); //We need to use this again
        return OldDecrypt(data, length, nrOfTriesLeft); //We will also try the old decryption
    } 
    catch (ModifiedDecryptorWithMAC::MACBadErr) 
    {
		masterkey->UnsecuredStringFinished(); //Secure the master key
        masterkey->assign((char*)"", 0, false);
        throw FileDecryptException(ERROR_DECRYPTION_CAPTION, ERROR_MAC_VALIDATION);
        return NULL;
    } 
    catch (Exception const& e) 
    {
		masterkey->UnsecuredStringFinished(); //Secure the master key
        masterkey->assign((char*)"", 0, false);
        throw FileDecryptException(ERROR_DECRYPTION_CAPTION, (char*) e.GetWhat().c_str());
        return NULL;
    }
#endif
}

#include <cryptopp/default.h>

SecureString* PwdFile::OldDecrypt(char* data, int length, int nrOfTriesLeft) {
#ifdef _DEBUG_NOENCRYPTION
    //data must be deallocated by SecureString
    return new SecureString(data, length, true);
#else
    DefaultDecryptorWithMAC *p;
    Ui* ui = Ui::getInstance();
    nrOfTriesLeft--;

    if (masterkey->length() == 0) {
        masterkey = ui->PromtPwd(UiElement(PROMT_PWD), UiElement(CAPTION_DECRYPT));
    }

    try {
        HexDecoder decryptor(p = new DefaultDecryptorWithMAC(masterkey->getUnsecureString()));
        decryptor.Put((byte*) data, length);
        decryptor.MessageEnd();

        int outputLength = (int)decryptor.MaxRetrievable();
        char* unsafeDecrypt = new char[outputLength + 1];
        decryptor.Get((byte*) unsafeDecrypt, outputLength);
        unsafeDecrypt[outputLength] = 0;

        masterkey->UnsecuredStringFinished();

        return new SecureString(unsafeDecrypt);
    } catch (DefaultDecryptor::KeyBadErr) {
        masterkey->UnsecuredStringFinished();
        mySleep(1000);
        ui->Error(UiElement(ERROR_MASTER_KEY));
        masterkey->assign((char*)"", 0, false);
        if (nrOfTriesLeft) {
            return Decrypt(data, length, nrOfTriesLeft);
        } else {
            throw UnresolvableException(ERROR_MAX_FAILED_DECRYPTS, this);
        }
    } catch (DefaultDecryptorWithMAC::MACBadErr) {
        masterkey->UnsecuredStringFinished();
        masterkey->assign((char*)"", 0, false);
        throw FileDecryptException(ERROR_DECRYPTION_CAPTION, ERROR_MAC_VALIDATION);
        return NULL;
    } catch (Exception const& e) {
        masterkey->UnsecuredStringFinished();
        masterkey->assign((char*)"", 0, false);
        throw FileDecryptException(ERROR_DECRYPTION_CAPTION, (char*) e.GetWhat().c_str());
        return NULL;
    }
#endif
}
