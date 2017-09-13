#pragma once

#include "curses.h"

#define ON_POSITION true
#define OFF_POSITION false

class RGBControl
{
private:
	chtype colorAttrs; //bitmask
	
public:
	RGBControl();
	bool pullSwitch(int color, bool position);
	chtype getColors();
	bool isColorOn(int color);
	bool isBoldOn();
};