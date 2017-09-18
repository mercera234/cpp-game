#include "ScrollBar.h"

ScrollBar::ScrollBar()
{
}

ScrollBar::ScrollBar(Controllable* c)
{
	setTargetControl(c);
}

void ScrollBar::setTargetControl(Controllable* c)
{
	//window will be positioned one tile to right of c's window
	this->c = c;
	WINDOW* w = c->getWindow();

	int rows;
	int cols;
	getmaxyx(w, rows, cols);

	int begX;
	int begY;
	getbegyx(w, begY, begX);

	setWindow(newwin(rows, 1, begY, begX + cols + 1));
}

void ScrollBar::draw()
{
	for (int row = 0; row < visibleRows; row++)
	{
		mvwaddch(win, row, 0, ACS_VLINE);
	}
	
	vPos = c->getUlY();
	int cTotalRows = c->getTotalRows();
	if (cTotalRows > visibleRows)
	{
		if(vPos < cTotalRows - visibleRows)
			drawDownArrow();
	
		if (vPos > 0)
			drawUpArrow();
	}
	wnoutrefresh(win);
	c->draw();
}

void ScrollBar::drawDownArrow()
{
	mvwaddch(win, visibleRows - 2, 0, '|');
	mvwaddch(win, visibleRows - 1, 0, 'v');
}

void ScrollBar::drawUpArrow()
{
	mvwaddch(win, 0, 0, '^');
	mvwaddch(win, 1, 0, '|');
}