#pragma once
#include "LineFormat.h"

/*For a vertical line format, justification left and right equate to top and bottom*/
struct VerticalLineFormat : public LineFormat
{
	VerticalLineFormat() {};
	VerticalLineFormat(int column, Justf justf) : LineFormat(column, justf) {}

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