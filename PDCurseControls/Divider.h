#pragma once
#include "Direction.h"
#include "TUI.h"

struct Divider
{
	Pos pos;
	int length;
	Axis axis;

	Divider(Pos posIn, Axis orientation, int lengthIn = -1) : pos(posIn), length(lengthIn), axis(orientation) {}


	void draw(WINDOW* win)
	{
		chtype tile;
		int drawLength = length;

		if (axis == Axis::HORIZONTAL)
		{
			tile = ACS_HLINE;
			if (drawLength == -1)
				drawLength = getmaxx(win);

			for (int i = 0; i < drawLength; i++)
			{
				mvwaddch(win, pos.y, pos.x + i, tile);
			}
		}
		else if (axis == Axis::VERTICAL)
		{
			tile = ACS_VLINE;
			if (drawLength == -1)
				drawLength = getmaxy(win);

			for (int i = 0; i < drawLength; i++)
			{
				mvwaddch(win, pos.y + i, pos.x, tile);
			}
		}
	}
};