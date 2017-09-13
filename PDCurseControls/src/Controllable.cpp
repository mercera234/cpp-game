#include "Controllable.h"
#include "curses.h"

/*
Sets window for Controllable with count of visible rows/cols and upper left corner of view
*/
void Controllable::setWindow(WINDOW* win)
{
	this->win = win;
	getmaxyx(win, visibleRows, visibleCols);

	ulY = 0;
	ulX = 0;
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

void Controllable::setPosition(int y, int x)
{
	ulY = y;
	ulX = x;
}

void Controllable::setDimensions(unsigned int rows, unsigned int cols)
{
	totalRows = rows;
	totalCols = cols;
	totalTiles = rows * cols;
}


void Controllable::shift(int y, int x)
{
	setPosition(ulY + y, ulX + x);
}