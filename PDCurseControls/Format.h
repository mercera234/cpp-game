#pragma once
#include "Style.h"
#include "curses.h"
#include "Direction.h"

struct Format
{
	virtual void draw(WINDOW* win, const std::string& text) = 0;
};

struct LineFormat : public Format
{
	int line = 0;
	Justf justf = Justf::LEFT;
	LineFormat() {};
	LineFormat(int line, Justf justf)
	{
		this->line = line;
		this->justf = justf;
	}
	 
	void draw(WINDOW* win, const std::string& text)
	{
		int x = getPosition(justf, getmaxx(win), text.length());
		mvwaddstr(win, line, x, text.c_str());
	}
};

/*For a vertical line format, justification left and right equate to top and bottom*/
struct VerticalLineFormat : public LineFormat
{
	VerticalLineFormat() {};
	VerticalLineFormat(int column, Justf justf) : LineFormat(column, justf)
	{
	}

	void draw(WINDOW* win, const std::string& text)
	{
		int y = getPosition(justf, getmaxy(win), text.length());

		//add characters one at a time
		for each (char c in text)
		{
			mvwaddch(win, y++, line, c);
		}
	}
};

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