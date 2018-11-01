#pragma once
#include "Style.h"
#include "curses.h"
#include "Direction.h"
#include <iterator>
#include <sstream>

/*A format for drawing text in a window*/
struct Format
{
	virtual void draw(WINDOW* win, const std::string& text) = 0;
};


/*A formatted line of text at a y,x position*/
struct PosFormat : public Format
{
	Pos pos;
	PosFormat() {}
	PosFormat(int yIn, int xIn) : pos(yIn,xIn) {}

	void draw(WINDOW* win, const std::string& text)
	{
		mvwaddstr(win, pos.y, pos.x, text.c_str());
	}
};

/*A positional format drawn vertically from y and x*/
struct VerticalPosFormat : public PosFormat
{
	Pos pos;
	VerticalPosFormat() {}
	VerticalPosFormat(int yIn, int xIn) : PosFormat(yIn, xIn) {}

	void draw(WINDOW* win, const std::string& text)
	{
		//add characters one at a time
		int row = pos.y;
		for each (char c in text)
		{
			mvwaddch(win, row++, pos.x, c);
		}
	}
};

