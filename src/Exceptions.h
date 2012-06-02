#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <cstdlib>
#include "Ui.h"

class BaseException : std::exception{
	protected:
		char* msg;
		char* capt;
	
	public:
		BaseException(char* caption, char* message){
			msg = message;
			capt = caption;
		}

		char* getMessage(){
			return msg;
		}

		char* getCaption(){
			return capt;
		}

		virtual void displayOnScreen(){
			Ui::getInstance()->showDialog(UiElement(msg), UiElement(capt));
		}
};

class UnresolvableException: public BaseException{
	public:
		UnresolvableException(char* msg) : BaseException(ERROR_CAPTION, msg){};
};

class FileException : public BaseException{
	public:
		FileException(char* caption, char* message):BaseException(caption, message) {}
};

class FileReadException : public FileException{
	public:
		FileReadException(char* caption, char* message):FileException(caption, message) {}
};

class FileWriteException : public FileException{
	public:
		FileWriteException(char* caption, char* message):FileException(caption, message) {}
};

class FileParseException : public FileException{
	public:
		FileParseException(char* caption, char* message):FileException(caption, message) {}
};

#endif