#include "Frame.h"

Frame::Frame(WINDOW* win, Controllable*c)
{
	this->win = win;
	this->c = c;
	showing = true;
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
	c->setFocus(focusIn);
}

void Frame::setText(string text, int y, int x)
{
	this->text = text;
	textY = y;
	textX = x;
}

Frame::~Frame()
{
	delete c;
	delwin(win);
}