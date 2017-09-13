#include "TextLabel.h"

TextLabel::TextLabel(WINDOW* win, std::string text)
{
	setWindow(win);
	
	this->text = text;
	showing = true;
	focusable = true;

	justf = JUST_LEFT;
}

void TextLabel::draw()
{	
	wclear(win);
	if (!showing) return;

	focus ? wattron(win, A_BOLD) : wattroff(win, A_BOLD);
	
	int start;
	switch (justf)
	{
	case JUST_LEFT: start = 0; break;
	case JUST_CENTER:  
	{
		int textLength = text.length();
		start = (visibleCols - textLength) / 2;
	}
		break;
	}

	mvwaddstr(win, 0, start, text.c_str()); //this will truncate any characters that would fall outside of the text label box
	wnoutrefresh(win);
}

