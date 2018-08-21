#include "CursesAttributeController.h"

CursesAttributeController::CursesAttributeController()
{
	attrs = 0x0;
}

bool CursesAttributeController::storeAttribute(chtype attr)
{
	if (attr > 0xffffff) //attributes only can be passed into this function
		return false;

	attrs |= attr;
	return true;
}

bool CursesAttributeController::freeAttribute(chtype attr)
{
	if (attr > 0xffffff) //attributes only can be passed into this function
		return false;

	attrs &= (attr ^ 0xffffffff);
	return true;
}

bool CursesAttributeController::storeColor(int color)
{
	if (color > COLOR_WHITE) //colors only can be passed into this function
		return false;

	chtype attr = COLOR_PAIR(color); //the color pair macro will give us the correct bit setting for the color
	attrs |= attr;
	return true;
}

bool CursesAttributeController::freeColor(int color)
{
	if (color > COLOR_WHITE) //colors only can be passed into this function
		return false;

	chtype attr = COLOR_PAIR(color); //the color pair macro will give us the correct bit setting for the color
	attrs &= (attr ^ 0xffffffff);
	return true;
}

bool CursesAttributeController::isColorStored(int color)
{
	chtype colorBit = COLOR_PAIR(color);
	chtype val = colorBit & attrs;
	bool test = val > 0;
	return test;
}

bool CursesAttributeController::isAttrStored(chtype attr)
{
	chtype val = attr & attrs;
	bool test = val > 0;
	return test;
}

chtype CursesAttributeController::getAllAttrs()
{
	return attrs;
}



chtype CursesAttributeController::getColors()
{
	return A_COLOR & attrs;
}


chtype CursesAttributeController::getAttrs()
{
	return A_ATTR_ONLY & attrs;
}