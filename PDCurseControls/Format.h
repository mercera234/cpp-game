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
	Justf justf = Justf::RIGHT;
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