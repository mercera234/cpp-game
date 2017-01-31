#include "MouseHelper.h"

bool MouseHelper::isInWindow(WINDOW* win, int y, int x)
{
	int begX, begY;
	int endX, endY;
	getbegyx(win, begY, begX);
	getmaxyx(win, endY, endX);
	endY += begY;
	endX += begX;
	return (y >= begY && y < endY && x >= begX && x < endX); //clicked in text palette
}


void MouseHelper::translateCoords(WINDOW* win, int inY, int inX, int &outY, int &outX)
{
	int begX, begY;
	getbegyx(win, begY, begX);

	outY = inY - begY;
	outX = inX - begX;
}