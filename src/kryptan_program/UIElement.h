#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "..\kryptan_core\SecureString.h"
#include <vector>

#ifdef _WIN32
#define PDC_WIDE
#include <pdcurses/curses.h>
//Removes compiler warning
#undef MOUSE_MOVED
#else
#include <curses.h>
#endif

//forward declaration
class UiElement;

typedef std::vector<UiElement> UiElementList;

class UiElement{
	private:
	/* VARIBLES */
		bool deleteData;
		SecureString* dataSecure;
		char* dataUnsecure;
		const char* dataConstant;
		chtype attributes;
		int attributeOffset;
	public:
	/* VARABLES */
	private:
	/* METHODS */
	public:
	/* METHODS */
		
		UiElement(char* data, bool deleteWhenDone, chtype attrib=A_BOLD, int attribOffset=0);
                UiElement(const char* data, chtype attrib=A_BOLD, int attribOffset=0);
		UiElement(SecureString* data, bool deleteWhenDone, chtype attrib=A_BOLD, int attribOffset=0);
		UiElement(const UiElement& obj);
		~UiElement();

		/* Call dataDone when finished with the data! */
		const char* getString();
		void stringDone();

		unsigned int getStringLength() const;
		
		chtype getAttributes();
		int getAttributesOffset();
};

#endif