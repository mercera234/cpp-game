#pragma once

#include "curses.h"

#define A_ATTR_ONLY (A_COLOR ^ A_ATTRIBUTES)

class CursesAttributeController
{
private:
	chtype attrs; //the attributes

public:
	CursesAttributeController();
	bool storeAttribute(chtype attr);
	bool freeAttribute(chtype attr);

	//we handle colors separately because of how the attron function works
	bool storeColor(int color);
	bool freeColor(int color);
	
	bool isColorStored(int color);
	bool isAttrStored(chtype attr);

	chtype getAllAttrs();
	chtype getColors();
	chtype getAttrs();
	
};