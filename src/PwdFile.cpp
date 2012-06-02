#include "PwdFile.h"

#include <fstream>

#include "Ui.h"
#include "Exceptions.h"

#define _DEBUG_NOENCRYPTION


PwdFile::PwdFile(char* fname){
	masterkey = NULL;
	Root = NULL;

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
					createNewFile();
				} else {
					//if not we give up
					throw UnresolvableException(ERROR_NO_PWD_FILE);
				}
			}
			catch(FileWriteException& e){
				//We could not create a file
				e.displayOnScreen();
				//Soo we give up!
				throw UnresolvableException(ERROR_NO_PWD_FILE);
			}
			//...and try again
		}
		catch(BaseException &e ){
			//This catches all other exceptions
			e.displayOnScreen();
			throw UnresolvableException(ERROR_NO_PWD_FILE);
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

void PwdFile::save(){
	std::ofstream of;

	of.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	if(!of){
		throw FileWriteException(ERROR_CAPTION, ERROR_WRITE_FILE);
	}

	//Code to save the entire thing goes here!
}

void PwdFile::createNewFile(){
	std::ofstream of;

	of.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);

	if(!of){
		throw FileWriteException(ERROR_CAPTION, ERROR_CREATE_FILE);
	}

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
	length = is.tellg();
	is.seekg (0, std::ios::beg);

	// allocate memory:
	buffer = new char [length+1];// the buffer is deallocated by Decrypt

	// read data as a block:
	is.read (buffer,length);
	is.close();

	//add '\0' at the end
	buffer[length] = '\0';

	//Decrypt the input (not realy neccesary since the contents is still encrypted)
	return Decrypt(buffer, length+1);
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

	char* currentLine;

	//Create a new root
	Root = new PwdTree(NULL);
	Root->setName(new SecureString(ROOT_TREE_NAME, 0, false));

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
					throw FileParseException("Password file version mismatch", "Visit http://www.caelus.org/proj/kryptan for help");
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
					SecureString* name = new SecureString(currentLine, 0, false);
					currentTree->setName(name);
					state = GET_NEXTENTRY;
				}
				break;
			case GET_DESCRIPTION:
				{
					SecureString* desc = new SecureString(currentLine, 0, false);
					currentPwd->setDescription(desc);
					state = GET_PASSWORD;
				}
				break;
			case GET_PASSWORD:
				{
					SecureString* pass = new SecureString(currentLine, 0, false);
					currentPwd->setPassword(pass);
					state = GET_USERNAME;
				}
				break;
			case GET_USERNAME:
				if(strcmp(currentLine, PASSWORDEND) != 0){
					SecureString* user = new SecureString(currentLine, 0, false);
					currentPwd->setUsername(user);
				}
				state = GET_NEXTENTRY;
				break;
		}

		//Delete the line, and get ready to read the next line
		filecontent->UnsecuredStringFinished();
	}
}


char* PwdFile::Encrypt(SecureString* data){
	return NULL;
}


SecureString* PwdFile::Decrypt(char* data, int length){
	//data must be deallocated by SecureString
#ifdef _DEBUG_NOENCRYPTION
	return new SecureString(data, length, true);
#else
	//Decrypt code goes here
#endif
}