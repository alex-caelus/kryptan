#include "PwdFileWorker.h"
#include "ModifiedEncryptor.h"
#include <cryptopp/hex.h>
#include <fstream>
#include <stdexcept>

using namespace Kryptan::Core;
using namespace std;
using namespace CryptoPP;


void PwdFileWorker::ReadFile(string filename, int& length, char*& buffer)
{
	std::ifstream is;
	try
	{
		buffer = 0;

		//open file
		is.open(filename, std::ios::in | std::ios::binary );

		if(!is){
			//file is not found or readable
			throw KryptanFileNotExistException("File not found or not readable");
		}

		// get length of file:
		is.seekg (0, std::ios::end);
		length = (int)is.tellg();
		is.seekg (0, std::ios::beg);

		// allocate memory:
		buffer = new char [length+1];// the buffer is deallocated by Decrypt

		// read data as a block:
		is.read (buffer, length);
		is.close();

		//add '\0' at the end
		buffer[length] = '\0';

	}
	catch(...)
	{
		delete[] buffer;
		//we re-throw to let the caller handle the error
		throw;
	}
}

void PwdFileWorker::WriteFile(string filename, char* content, int length)
{
	std::ofstream os;
	try
	{
		//open file
		os.open(filename, std::ios::out | std::ios::binary | std::ios::trunc );

		if(!os){
			//file is not found or readable
			throw KryptanFileNotExistException("File could not be opened or created for writing");
		}
		
		// read data as a block:
		os.write(content, length);
		os.close();
	}
	catch(KryptanFileNotExistException)
	{
		//just let the caller handle the error
		throw;
	}
	catch(std::exception& e)
	{
		//we re-throw to let the caller handle the error
		throw KryptanBaseException(string("An exception occured while writing to file: ") + e.what());
	}
}

bool PwdFileWorker::FileExists(string filename)
{
	ifstream ifile(filename);
	return ifile;
}

PwdList* PwdFileWorker::ParseFileContents(SecureString content)
{
	PwdList* target = new PwdList();

    char* currTag = GetNextTagStart(content.getUnsecureString());
    char* lastTag = NULL;
    int currTagLength = GetTagLength(currTag);
    enum STATES {NOTSET, ROOT, PASSWORD, LABELS};
    
    STATES currState = NOTSET;
    SecureString currDescription;
    SecureString currUsername;
    SecureString currPassword;
	PwdLabelVector currLabels;
    
    try{
        do{
			switch(currState)
			{
			case NOTSET:
				if(strncmp(currTag, RootTagStart, currTagLength) == 0){
					currState = ROOT;
				}
				else{
					throw runtime_error("Password file is corrupt");
				}
				break;
			case ROOT:
				if(strncmp(currTag, PwdTagStart, currTagLength) == 0){
					currDescription.assign("", 0);
					currUsername.assign("", 0);
					currPassword.assign("", 0);
					currLabels.clear();
					currState = PASSWORD;
				}
				else if(strncmp(currTag, RootTagEnd, currTagLength) == 0){
					currState = NOTSET;
					//we are done :)
				}
				else{
					throw runtime_error("Password file is corrupt");
				}
				break;
			case PASSWORD:
				if(strncmp(currTag, DescriptionTagStart, currTagLength) == 0){
					char* contentBegin = currTag + currTagLength;
					char* contentEnd = GetNextTagStart(contentBegin);
					int contentLength = contentEnd - contentBegin;
					int endLength = GetTagLength(contentEnd);
					if(strncmp(contentEnd, DescriptionTagEnd, endLength) == 0)
					{
						currDescription.assign(UnescapeTags(contentBegin, contentLength));
					}
					else{
						throw runtime_error("Password file is corrupt");
					}
					currTag = contentEnd;
				}
				else if(strncmp(currTag, UsernameTagStart, currTagLength) == 0){
					char* contentBegin = currTag + currTagLength;
					char* contentEnd = GetNextTagStart(contentBegin);
					int contentLength = contentEnd - contentBegin;
					int endLength = GetTagLength(contentEnd);
					if(strncmp(contentEnd, UsernameTagEnd, endLength) == 0)
					{
						currUsername.assign(UnescapeTags(contentBegin, contentLength));
					}
					else{
						throw runtime_error("Password file is corrupt");
					}
					currTag = contentEnd;
				}
				else if(strncmp(currTag, PasswordTagStart, currTagLength) == 0){
					char* contentBegin = currTag + currTagLength;
					char* contentEnd = GetNextTagStart(contentBegin);
					int contentLength = contentEnd - contentBegin;
					int endLength = GetTagLength(contentEnd);
					if(strncmp(contentEnd, PasswordTagEnd, endLength) == 0)
					{
						currPassword.assign(UnescapeTags(contentBegin, contentLength));
					}
					else{
						throw runtime_error("Password file is corrupt");
					}
					currTag = contentEnd;
				}
				else if(strncmp(currTag, LabelTagStart, currTagLength) == 0){
					char* contentBegin = currTag + currTagLength;
					char* contentEnd = GetNextTagStart(contentBegin);
					int contentLength = contentEnd - contentBegin;
					int endLength = GetTagLength(contentEnd);
					if(strncmp(contentEnd, LabelTagEnd, endLength) == 0)
					{
						currLabels.push_back(UnescapeTags(contentBegin, contentLength));
					}
					else{
						throw runtime_error("Password file is corrupt");
					}
					currTag = contentEnd;
				}
				else if(strncmp(currTag, PwdTagEnd, currTagLength) == 0){
					//validate what information we have
					if(currDescription.length() <= 0)
					{
						throw runtime_error("Password file is corrupt");
					}
					if(currPassword.length() <= 0)
					{
						throw runtime_error("Password file is corrupt");
					}
					//and save it
					Pwd* p = target->CreatePwd(currDescription, currUsername, currPassword);

					for(PwdLabelVector::iterator it = currLabels.begin(); it != currLabels.end(); it++)
					{
						target->AddPwdToLabel(p, *it);
					}

					//change state
					currState = ROOT;
				}
				else{
					throw runtime_error("Password file is corrupt");
				}
				break;
			}
        }while((currTag = GetNextTagStart(currTag+1)) != NULL && (currTagLength = GetTagLength(currTag)) != 0);
    }
    catch(...)
    {
		content.UnsecuredStringFinished();
		delete target;
		target = NULL;
        throw;
    }
	content.UnsecuredStringFinished();
    return target;
}

char* PwdFileWorker::GetNextTagStart(const char* pos)
{
	char* p = (char*)pos;
    for(; *p != '<' && *p != '\0'; p++){};
    return *p == '\0' ? NULL : p;
}

int PwdFileWorker::GetTagLength(const char* tagStart)
{
	char* p = (char*)tagStart;
    for(; *p != '>' && *p != '\0'; p++){}
    return p - tagStart + 1;
}

SecureString PwdFileWorker::EscapeTags(const SecureString& str)
{
	//we add a extra buffer length of 20 as a qualified 
	//guess, it doesn't realy matter since
	//SecureString is dynamic
	int l = str.length();
	SecureString copy(l + 20);

	for(int i=0; i < l; i++)
	{
		char c = str.at(i);
		if(c == '<')
		{
			const char* lt = "&lt;";
			copy.append(lt);
		}
		else if(c == '>')
		{
			const char* gt = "&gt;";
			copy.append(gt);
		}
		else if(c == '&')
		{
			const char* and = "&amp;";
			copy.append(and);
		}
		else
		{
			copy.append(&c, 1, false);		
		}
	}

	return copy;
}

SecureString PwdFileWorker::UnescapeTags(const char* str, int l)
{
	const char* lt = "<";
	const char* gt = ">";
	const char* and = "&";
	
	SecureString copy(l);

	for(int i=0; i < l; i++)
	{
		char c = str[i];
		if(c == '&')
		{
			if(l - i >= 3)
			{
				//&lt;
				if(str[i+1] == 'l' && str[i+2] == 't' && str[i+3] == ';')
				{
					copy.append(lt);
					i += 3;
					continue;
				}
				//&gt;
				else if(str[i+1] == 'g' && str[i+2] == 't' && str[i+3] == ';')
				{
					copy.append(gt);
					i += 3;
					continue;
				}
			}
			if(l - i >= 4)
			{
				//&amp;
				if(str[i+1] == 'a' && str[i+2] == 'm' && str[i+3] == 'p' && str[i+4] == ';')
				{
					copy.append(and);
					i += 4;
					continue;
				}
			}
		}
		copy.append(&c, 1, false);
	}

	return copy;
}

char* PwdFileWorker::Encrypt(SecureString data, int& encryptedLength, SecureString masterkey) {
    try {
        ModifiedEncryptorWithMAC encryptor(masterkey.getUnsecureString(), new HexEncoder());
        encryptor.Put((byte*) data.getUnsecureString(), data.length());
        encryptor.MessageEnd();

        encryptedLength = (int) encryptor.MaxRetrievable();
        char* newBuff = new char[encryptedLength + 1];
        encryptor.Get((byte*) newBuff, encryptedLength);
        newBuff[encryptedLength] = 0;
        masterkey.UnsecuredStringFinished();
        data.UnsecuredStringFinished();
        return newBuff;
    } catch (Exception const& e) {
		data.UnsecuredStringFinished();
		masterkey.UnsecuredStringFinished();
        throw runtime_error((char*) e.GetWhat().c_str());
        return NULL;
    }
}

SecureString PwdFileWorker::Decrypt(char* encryptedBuffer, int encryptedBufferLength, SecureString masterkey)
{
	char* unsafeDecrypt = 0;
	int outputLength;

    if (masterkey.length() == 0) {
        throw runtime_error("Master key is zero in length");
    }

    try {
		ModifiedDecryptorWithMAC* p = new ModifiedDecryptorWithMAC(masterkey.getUnsecureString());
        HexDecoder decryptor(p);
        decryptor.Put((byte*) encryptedBuffer, encryptedBufferLength);
        decryptor.MessageEnd();

        outputLength = (int)decryptor.MaxRetrievable();
        unsafeDecrypt = new char[outputLength + 1];
        decryptor.Get((byte*) unsafeDecrypt, outputLength);
        unsafeDecrypt[outputLength] = 0;

		//delete
        masterkey.UnsecuredStringFinished();
        return SecureString(unsafeDecrypt); //This also deletes 'unsafeDecrypt' securely
    } 
    catch (ModifiedDecryptor::KeyBadErr) 
    {
		masterkey.UnsecuredStringFinished(); //We need to use this again
		delete[] unsafeDecrypt;

		throw runtime_error("Wrong master key!");
    } 
    catch (ModifiedDecryptorWithMAC::MACBadErr) 
    {
		masterkey.UnsecuredStringFinished(); //Secure the master key
		delete[] unsafeDecrypt;

        throw runtime_error("Filecontent is corrupt");
    } 
    catch (...) 
    {
		masterkey.UnsecuredStringFinished(); //Secure the master key
		delete[] unsafeDecrypt;

		//rethrow
		throw;
    }
}

void PwdFileWorker::DeletePwdList(PwdList* list)
{
	delete list;
}