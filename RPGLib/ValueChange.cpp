#include "ValueChange.h"
#include <iostream>
#include <iomanip>

ValueChange::ValueChange(Format* fmtIn, int* valueIn) : fmt(fmtIn), value(valueIn)
{
}

void ValueChange::draw(WINDOW* win)
{
	if (drawZero == false && *value == 0)
		return;

	std::ostringstream oss;
	int usedColor = COLOR_GRAY;

	if (*value != 0)
	{
		oss << std::showpos;
		usedColor = (*value > 0) ? positiveColor : negativeColor;
	}

	oss << *value;

	chtype color = setTextColor(usedColor);

	wattron(win, color);
	fmt->draw(win, oss.str());
	wattroff(win, color);
}