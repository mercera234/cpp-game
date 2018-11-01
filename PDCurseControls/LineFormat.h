#pragma once
#include "Format.h"

/*A format that identifies a line and justification in a window for drawing*/
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

