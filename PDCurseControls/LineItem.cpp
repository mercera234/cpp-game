#include "LineItem.h"
#include "AbstractMenu.h"

LineItem::LineItem(std::string name, int element, int crossRefNdx)
{
	index = element;
	crossRef = crossRefNdx;
	this->name = name;
	selectable = true;
	selected = false;
}


void LineItem::draw()
{
	MenuItem::draw();

	if (selectable == false)
	{
		wattron(win, setTextColor(COLOR_GRAY)| A_DIM);
	}


	//draw item
	switch (field)
	{
	case Fielddraw::DRAW_ICON: 
		mvwaddch(win, topRow, itemX, icon);
		break;

	case Fielddraw::DRAW_NAME: 
		mvwaddstr(win, topRow, itemX, name.c_str());
		break;
	}
	

	if (selectable == false)
	{
		wattroff(win, setTextColor(COLOR_GRAY) | A_DIM);
	}
}

void LineItem::clear()
{
	name = "";
	MenuItem::clear();
}