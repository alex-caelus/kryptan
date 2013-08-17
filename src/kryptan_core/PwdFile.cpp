#include "PwdFile.h"
#include "ModifiedEncryptor.h"
#include "PwdFileWorker.h"
#include <cryptopp/hex.h>
#include <fstream>
#include <stdexcept>

using namespace Kryptan::Core;
using namespace std;
using namespace CryptoPP;


PwdFile::PwdFile(string filename)
{
	this->filename = filename;
	list = NULL;
	isOpen = false;
}


PwdFile::~PwdFile(void)
{
	PwdFileWorker::DeletePwdList(list);
}

PwdFile::PwdFile(const PwdFile& obj)
{
}

void PwdFile::CreateNew()
{
	if(isOpen)
		throw logic_error("File is already open!");

	list = PwdFileWorker::ParseFileContents(SecureString(
				"<passwords>\n"
					"<pwd>\n"
						"<description>Google mail</description>\n"
						"<password>qwerty</password>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Hotmail</description>\n"
						"<username>alex.caelus@live.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Stackoverflow</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>online</label>\n"
						"<label>test2</label>\n"
						"<label>online2</label>\n"
						"<label>test3</label>\n"
						"<label>online3</label>\n"
						"<label>test4</label>\n"
						"<label>online4</label>\n"
						"<label>test5</label>\n"
						"<label>online5</label>\n"
						"<label>test6</label>\n"
						"<label>online6</label>\n"
						"<label>test7</label>\n"
						"<label>online7</label>\n"
						"<label>test8</label>\n"
						"<label>online8</label>\n"
						"<label>test9</label>\n"
						"<label>online9</label>\n"
						"<label>test10</label>\n"
						"<label>online10</label>\n"
						"<label>test10</label>\n"
						"<label>online11</label>\n"
						"<label>test11</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin2</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin3</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin4</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin5</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin6</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin7</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin8</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin9</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedin0</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedina</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedins</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedind</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedinf</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkeding</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedinh</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedinj</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedink</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedinl</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedinq</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
					"<pwd>\n"
						"<description>Linkedinw</description>\n"
						"<username>alex.caelus@gmail.com</username>\n"
						"<password>qwerty</password>\n"
						"<label>online</label>\n"
						"<label>test</label>\n"
						"<label>test2</label>\n"
					"</pwd>\n"
				"</passwords>"));

	isOpen = true;
}

void PwdFile::OpenAndParse(SecureString masterkey)
{
	char* encryptedBuffer = 0;
	int encryptedBufferLength;
	try{
		if(isOpen)
			throw logic_error("File is already open!");

		//open file and read it's content
		PwdFileWorker::ReadFile(filename, encryptedBufferLength, encryptedBuffer);

		//decrypt the contents
		SecureString decryptedString = PwdFileWorker::Decrypt(encryptedBuffer, encryptedBufferLength, masterkey);
		
		//delete the encrypted buffer, after we zero it out first
		delete[] encryptedBuffer;

		//parse the contents
        list = PwdFileWorker::ParseFileContents(decryptedString);

		isOpen = true;
	}
	catch(...)
	{
		delete[] encryptedBuffer;
		//just re-throw, we let the caller handle theese errors
		throw;
	}
}

void PwdFile::Save(SecureString masterkey)
{
	if(!isOpen)
		throw logic_error("File must be opened before Save() can be called!");

	SecureString content(RootTagStart);

	auto pwds = list->All();
	for(auto pwd = pwds.begin(); pwd != pwds.end(); pwd++)
	{
		content.append(PwdTagStart);

		//description
		content.append(DescriptionTagStart);
		content.append(PwdFileWorker::EscapeTags((*pwd)->GetDescription()));
		content.append(DescriptionTagEnd);

		//username
		content.append(UsernameTagStart);
		content.append(PwdFileWorker::EscapeTags((*pwd)->GetUsername()));
		content.append(UsernameTagEnd);

		//password
		content.append(PasswordTagStart);
		content.append(PwdFileWorker::EscapeTags((*pwd)->GetPassword()));
		content.append(PasswordTagEnd);

		//labels
		auto labels = (*pwd)->GetLabels();
		for(auto label = labels.begin(); label != labels.end(); label++)
		{
			content.append(LabelTagStart);
			content.append(PwdFileWorker::EscapeTags(*label));
			content.append(LabelTagEnd);
		}
		
		content.append(PwdTagEnd);
	}
	content.append(RootTagEnd);

	int eLength;
    char* encrypted = PwdFileWorker::Encrypt(content, eLength, masterkey);

	PwdFileWorker::WriteFile(filename, encrypted, eLength);
}

PwdList* PwdFile::GetPasswordList()
{
	if(!isOpen)
		throw logic_error("File must be opened before GetPasswordList() can be called!");
	return list;
}

string PwdFile::GetFilename()
{
	return filename;
}

bool PwdFile::IsOpen()
{
	return isOpen;
}

bool PwdFile::Exists()
{
	return PwdFileWorker::FileExists(filename);
}