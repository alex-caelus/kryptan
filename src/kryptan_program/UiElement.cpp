#include "UIElement.h"
#include <string.h>

using namespace Kryptan::Core;
using namespace Kryptan::Program;

UiElement::UiElement(char* data, bool deleteWhenDone, chtype attrib, int attribOffset) {
    dataSecure = NULL;
    dataUnsecure = data;
    dataConstant = NULL;
    attributes = attrib;
    deleteData = deleteWhenDone;
    attributeOffset = attribOffset;
}

UiElement::UiElement(const char* data, chtype attrib, int attribOffset) {
    dataSecure = NULL;
    dataUnsecure = NULL;
    dataConstant = data;
    attributes = attrib;
    deleteData = false;
    attributeOffset = attribOffset;
}

UiElement::UiElement(SecureString* data, bool deleteWhenDone, chtype attrib, int attribOffset) {
    dataSecure = data;
    dataUnsecure = NULL;
    dataConstant = NULL;
    attributes = attrib;
    deleteData = deleteWhenDone;
    attributeOffset = attribOffset;
}

UiElement::UiElement(const UiElement& obj) {
    this->attributes = obj.attributes;
    this->attributeOffset = obj.attributeOffset;
    if (obj.dataSecure) {
        this->dataSecure = new SecureString(*obj.dataSecure);
        this->dataUnsecure = NULL;
    } else {
		int length = obj.getStringLength() + 1;
        this->dataSecure = NULL;
        this->dataUnsecure = new char[length];
        this->dataConstant = NULL;
        if (obj.dataUnsecure) {
			memcpy(this->dataUnsecure, obj.dataUnsecure, length);
        } else if (obj.dataConstant) {
            memcpy(this->dataUnsecure, obj.dataConstant, length);
        }
    }
    this->deleteData = true; //make sure to delete the copy of the data
}

UiElement::~UiElement() {
    //do not delete data, since they come from exterior sources
    if (deleteData) {
        delete dataSecure;
        delete dataUnsecure;
        //delete dataConstant; //do NOT delete dataConstant
        dataSecure = NULL;
        dataUnsecure = NULL;
        dataConstant = NULL;
    }
}

const char* UiElement::getString() {
    if (dataSecure)
        return dataSecure->getUnsecureString();
    else if (dataUnsecure)
        return dataUnsecure;
    else 
        return dataConstant;
}

void UiElement::stringDone() {
    if (dataSecure)
        dataSecure->UnsecuredStringFinished();
}

unsigned int UiElement::getStringLength() const {
    if (dataSecure)
        return dataSecure->length();
    else if(dataUnsecure)
        return strlen(dataUnsecure);
    else 
        return strlen(dataConstant);
}

chtype UiElement::getAttributes() {
    return attributes;
}

int UiElement::getAttributesOffset() {
    return attributeOffset;
}