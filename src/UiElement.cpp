#include "UIElement.h"
#include <string.h>


UiElement::UiElement(char* data, chtype attrib, int attribOffset, bool deleteWhenDone){
	secure = false;
	dataUnsecure = data;
	dataSecure = NULL;
	attributes = attrib;
	deleteData = deleteWhenDone;
	attributeOffset = attribOffset;
}

UiElement::UiElement(SecureString* data, chtype attrib, int attribOffset, bool deleteWhenDone){
	secure = true;
	dataUnsecure = NULL;
	dataSecure = data;
	attributes = attrib;
	deleteData = deleteWhenDone;
	attributeOffset = attribOffset;
}

UiElement::UiElement(const UiElement& obj){
	this->attributes = obj.attributes;
	this->attributeOffset = obj.attributeOffset;
	if(obj.secure){
		this->dataSecure = new SecureString(*obj.dataSecure);
		this->dataUnsecure = NULL;
		this->secure = true;
	} else {
		this->dataSecure = NULL;
		this->dataUnsecure = new char[obj.getStringLength()+1];
		strcpy(this->dataUnsecure, obj.dataUnsecure);
		this->secure = false;
	}
	this->deleteData = true; //make sure to delete the copy of the data
}

UiElement::~UiElement(){
	//do not delete data, since they come from exterior sources
	if(deleteData){
		delete dataSecure;
		delete dataUnsecure;
		dataSecure = NULL;
		dataUnsecure = NULL;
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

int UiElement::getStringLength() const{
	if(secure)
		return dataSecure->length();
	else
		return strlen(dataUnsecure);
}

chtype UiElement::getAttributes(){
	return attributes;
}

int UiElement::getAttributesOffset(){
	return attributeOffset;
}