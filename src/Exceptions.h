#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstdlib>
#include "UI.h"
#include <string>

using std::string;

class CanThrowUnresolvableExceptions{
public:
	virtual string getClassName() = 0;
};

class BaseException : public std::exception{
	protected:
		const char* msg;
		const char* capt;
	
	public:
		BaseException(const char* caption, const char* message){
			msg = message;
			capt = caption;
		}

		const char* getMessage(){
			return msg;
		}

		const char* getCaption(){
			return capt;
		}

		virtual void displayOnScreen(){
			Ui::getInstance()->showDialog(UiElement(msg), UiElement(capt));
		}
};

class UnresolvableException: public BaseException{
	CanThrowUnresolvableExceptions* src;
	public:
		UnresolvableException(const char* msg, CanThrowUnresolvableExceptions* src) : BaseException(ERROR_CAPTION, msg){this->src = src;};
		void setSrc(CanThrowUnresolvableExceptions* src){this->src = src;};
		CanThrowUnresolvableExceptions* getSrc(){return src;}
};

class FileException : public BaseException{
	public:
		FileException(const char* caption, const char* message):BaseException(caption, message) {}
};

class FileReadException : public FileException{
	public:
		FileReadException(const char* caption, const char* message):FileException(caption, message) {}
};

class FileWriteException : public FileException{
	public:
		FileWriteException(const char* caption, const char* message):FileException(caption, message) {}
};

class FileParseException : public FileException{
	public:
		FileParseException(const char* caption, const char* message):FileException(caption, message) {}
};

class FileEncryptException : public FileException{
	public:
		FileEncryptException(const char* caption, const char* message):FileException(caption, message) {}
};

class FileDecryptException : public FileException{
	public:
		FileDecryptException(const char* caption, const char* message):FileException(caption, message) {}
};

#endif