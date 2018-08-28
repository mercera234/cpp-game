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
	int line = -1;
	Justf justf;
	void draw(WINDOW* win, const std::string& text)
	{
		int x = getPosition(justf, getmaxx(win), text.length());
		mvwaddstr(win, line, x, text.c_str());
	}
};

struct PosFormat : public Format
{
	Pos pos;
	void draw(WINDOW* win, const std::string& text)
	{
		mvwaddstr(win, pos.y, pos.x, text.c_str());
	}
};