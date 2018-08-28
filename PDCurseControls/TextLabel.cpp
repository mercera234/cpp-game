#include "TextLabel.h"

TextLabel::TextLabel()
{
	init();
}

TextLabel::TextLabel(WINDOW* win, const std::string& text)
{
	setWindow(win);
	
	this->text = text;
	init();
}

void TextLabel::init()
{
	showing = true;
	focusable = true;

	justf = Justf::LEFT;
}

void TextLabel::draw()
{	
	wclear(win);
	if (!showing) return;

	focus ? wattron(win, A_BOLD) : wattroff(win, A_BOLD);
	
	int start;
	switch (justf)
	{
	case Justf::LEFT: start = 0; break;
	case Justf::CENTER:
	{
		int textLength = text.length();
		start = (visibleCols - textLength) / 2;
	}
		break;
	}

	mvwaddstr(win, 0, start, text.c_str()); //this will truncate any characters that would fall outside of the text label box
	wnoutrefresh(win);
}

