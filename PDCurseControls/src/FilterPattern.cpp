#include "FilterPattern.h"

FilterPattern::FilterPattern(Controllable* c)
{
	this->c = c;
	//filter and controllable will share window
	win = c->getWindow();
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

