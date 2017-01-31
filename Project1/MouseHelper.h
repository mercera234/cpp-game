#pragma once
#include "curses.h"

class MouseHelper
{
private:
public:

	static bool isInWindow(WINDOW* win, int y, int x);
	static void translateCoords(WINDOW* win, int inY, int inX, int &outY, int &outX);
};

