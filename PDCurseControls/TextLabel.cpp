#include "TextLabel.h"

TextLabel::TextLabel()
{
	
}

TextLabel::TextLabel(WINDOW* win, const std::string& text)
{
	setWindow(win);
	
	this->text = text;
}

void TextLabel::draw()
{	
	if (format == nullptr)
		return;

	wclear(win);
	if (!showing) return;

	focus ? wattron(win, A_BOLD) : wattroff(win, A_BOLD);

	format->draw(win, text);
	wnoutrefresh(win);
}

TextLabel::~TextLabel()
{
	delete format;
}