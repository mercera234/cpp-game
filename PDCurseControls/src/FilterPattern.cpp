#include "FilterPattern.h"

FilterPattern::FilterPattern(Controllable* c)
{
	setControl(c);
}

void FilterPattern::draw()
{
	//draw component then apply filter
	c->draw();

	if (enabled)
	{
		applyFilterPattern();
		wnoutrefresh(win);
	}
}

void FilterPattern::setControl(Controllable* c)
{
	this->c = c;
	//filter and controllable will share window
	win = c->getWindow();
}