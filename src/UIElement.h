#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "SecureString.h"
#include <vector>

#ifdef _WIN32
#define PDC_WIDE
#include <pdcurses/curses.h>
#else
#include <curses.h>
#endif

//forward declaration
class UiElement;

typedef std::vector<UiElement> UiElementList;

class UiElement{
	private:
	/* VARIBLES */
		bool secure;
		bool deleteData;
		SecureString* dataSecure;
		char* dataUnsecure;
		chtype attributes;
		int attributeOffset;
	public:
	/* VARABLES */
	private:
	/* METHODS */
	public:
	/* METHODS */
		
		UiElement(char* data, chtype attrib=A_BOLD, int attribOffset=0, bool deleteWhenDone=false);
		UiElement(SecureString* data, chtype attrib=A_BOLD, int attribOffset=0, bool deleteWhenDone=false);
		UiElement(const UiElement& obj);
		~UiElement();

		/* Call dataDone when finished with the data! */
		char* getString();
		void stringDone();

		int getStringLength() const;
		
		chtype getAttributes();
		int getAttributesOffset();
};

#endif