#pragma once
#include "LineFormat.h"

/*A line format that is only left justified and breaks a string of text up to display it*/
struct BookLineFormat : public LineFormat
{
	BookLineFormat() {};
	BookLineFormat(int line) : LineFormat(line, Justf::LEFT)
	{}

	void draw(WINDOW* win, const std::string& text)
	{
		int x = getPosition(justf, getmaxx(win), text.length());

		//parse text into words

		std::istringstream iss(text);
		std::vector<std::string> words(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

		int lineOffset = 0;
		int offset = 0;
		for each (std::string s in words)
		{
			int space = getmaxx(win) - offset;
			if (space < (int)s.length())
			{
				lineOffset++;
				offset = 0;
			}

			mvwaddstr(win, line + lineOffset, x + offset, s.c_str());
			offset += s.length() + 1;
			waddch(win, ' ');
		}
	}
};
