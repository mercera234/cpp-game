#pragma once
#include "curses.h"

//singleton pattern used since all methods will be static
class ChtypeFilter
{
private:
public:
	static chtype applyColorFilter(chtype ch, int color);
	static chtype applyBWFilter(chtype ch);
	static chtype applyMarkFilter(chtype ch, int color, char mark);
};