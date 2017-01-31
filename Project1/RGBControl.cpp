#include "RGBControl.h"

RGBControl::RGBControl()
{
	colorAttrs = 0x0;
}

bool RGBControl::pullSwitch(int attr, bool position)
{
	chtype bit;
	switch (attr)
	{
	case COLOR_RED: 
	case COLOR_GREEN:
	case COLOR_BLUE: bit = COLOR_PAIR(attr); break;
	case A_BOLD: bit = attr;
		break;
	default: return false; //not an attribute we can turn on here
	}

	if(position == ON_POSITION)
		colorAttrs |= bit;
	else //position == OFF_POSITION
		colorAttrs &= (bit ^ 0xffffffff);

	return true;
}

chtype RGBControl::getColors()
{
	return colorAttrs;
}

bool RGBControl::isColorOn(int color)
{
	chtype colorBit = COLOR_PAIR(color);
	chtype val = colorBit & colorAttrs;
	bool test = val > 0;
	return test;		
}

bool RGBControl::isBoldOn()
{
	chtype val = A_BOLD & colorAttrs;
	bool test = val > 0;
	return test;
}