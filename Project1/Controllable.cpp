#include "Controllable.H"
#include "curses.h"

bool Controllable::isInWindow(int y, int x)
{
	int begX, begY;
	int endX, endY;
	getbegyx(win, begY, begX);
	getmaxyx(win, endY, endX);
	endY += begY;
	endX += begX;
	return (y >= begY && y < endY && x >= begX && x < endX); //clicked in text palette
}

/*
Translates window specific coordinates to terminal specific coordinates (may be a curses version of this method)
*/
void Controllable::translateCoords(int inY, int inX, int &outY, int &outX)
{
	int begX, begY;
	getbegyx(win, begY, begX);

	outY = inY - begY;
	outX = inX - begX;
}

void Controllable::setWindow(Rect* bounds)
{
	r = bounds;

	int rows = r->height;
	int cols = r->width;
	int screenRows = getmaxy(stdscr);
	int screenCols = getmaxx(stdscr);

	if (r->y < 0 || r->x < 0 || r->y + rows > screenRows || r->x + cols > screenCols) //window being created is outside terminal window
	{
		int y = r->y; 
		int x = r->x;
		
		if (r->y < 0)
		{
			rows += r->y;
			y = 0;
		}
		if (r->x < 0)
		{
			cols += r->x;
			x = 0;
		}
		if (r->y + rows > screenRows)
		{
			rows = screenRows - r->y;
		}
		if (r->x + cols > screenCols)
		{
			cols = screenCols - r->x;
		}

		//create a window that can fit on screen
		win = newwin(rows, cols, y, x); //may still fail if completely off screen, but this may be okay
	}
	else
		this->win = win;
}

void Controllable::move(int y, int x)
{
	int rows = r->height;
	int cols = r->width;
	int screenRows = getmaxy(stdscr);
	int screenCols = getmaxx(stdscr);

	if (y < 0 || x < 0 || y + rows > screenRows || x + cols > screenCols) //moved out of bounds
	{
		//onBoundary = true;
		if (y < 0) rows += y;
		if (x < 0) cols += x;
		if (y + rows > screenRows) rows = screenRows - y;
		if (x + cols > screenCols) cols = screenCols - x;
			
		//resize the window and reposition the rectangle without resizing
		wresize(win, rows, cols);
		r->y = y;
		r->x = x;
	}
	else
	{
		//check if moved back in from out of bounds (requires resizing)
		if (r->height != getmaxy(win) || r->width != getmaxx(win))
		{
			wresize(win, r->height, r->width);
		}

		mvwin(win, y, x);
		getbegyx(win, r->y, r->x);
	}
}
