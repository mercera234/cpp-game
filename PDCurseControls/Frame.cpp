#include "Frame.h"

Frame::Frame(WINDOW* win, Controllable*c)
{
	setWindow(win);
	setControl(c);
}

void Frame::draw()
{
	//draw frame
	wclear(win);

	if (showing)
	{
		focus ? wattron(win, A_BOLD) : wattroff(win, A_BOLD);

		box(win, 0, 0);

		if (text.empty() == false)
		{
			mvwaddstr(win, textY, textX, text.c_str());
		}

		wnoutrefresh(win);
	}
	
	//draw control
	if(c != NULL)
		c->draw();
}

void Frame::setFocus(bool focusIn)
{
	Controllable::setFocus(focusIn);
	if(c != nullptr)
		c->setFocus(focusIn);
}

void Frame::setText(const std::string& text, int y, int x)
{
	this->text = text;
	textY = y;
	textX = x;
}
