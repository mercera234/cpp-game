#include "TextLabel.h"

TextLabel::TextLabel(WINDOW* win, string text)
{
	this->win = win;
	this->text = text;
}

void TextLabel::draw()
{
	mvwaddstr(win, 0, 0, text.c_str()); //this will truncate any characters that would fall outside of the text label box
	wnoutrefresh(win);
}