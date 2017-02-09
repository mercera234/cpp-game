#include "Frame.h"

Frame::Frame(WINDOW* win, Controllable*c)
{
	this->win = win;
	this->c = c;
	border = BORDER_NORMAL;
}

void Frame::draw()
{
	//draw frame
	wclear(win);
	switch (border)
	{
	case BORDER_NORMAL: box(win, 0, 0); break;
	case BORDER_BOLD: 
		wattron(win, A_BOLD);
		box(win, 0, 0);
		wattroff(win, A_BOLD);
	default: break; //just break, no border
	}
	//draw text if there is any
	if (text.empty() == false)
	{
		mvwaddstr(win, textY, textX, text.c_str());
	}

	wnoutrefresh(win);

	//draw control
	if(c != NULL)
		c->draw();
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