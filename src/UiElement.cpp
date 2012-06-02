#include "UIElement.h"


UiElement::UiElement(char* data, chtype attrib, bool deleteWhenDone){
	secure = false;
	dataUnsecure = data;
	dataSecure = NULL;
	attributes = attrib;
	deleteData = deleteWhenDone;
}

UiElement::UiElement(SecureString* data, chtype attrib, bool deleteWhenDone){
	secure = true;
	dataUnsecure = NULL;
	dataSecure = data;
	attributes = attrib;
	deleteData = deleteWhenDone;
}

UiElement::~UiElement(){
	//do not delete data, since they come from exterior sources
	if(deleteData){
		delete dataSecure;
		delete dataUnsecure;
	}
}

char* UiElement::getString(){
	if(secure)
		return dataSecure->getUnsecureString();
	else
		return dataUnsecure;
}

void UiElement::stringDone(){
	if(secure)
		dataSecure->UnsecuredStringFinished();
}

int UiElement::getStringLength(){
	if(secure)
		return dataSecure->length();
	else
		return strlen(dataUnsecure);
}

chtype UiElement::getAttributes(){
	return attributes;
}

