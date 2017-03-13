#include "LineItem.h"

LineItem::LineItem(string name, int element, int crossRefNdx)
{
	index = element;
//	selected = false;
	crossref = crossRefNdx;
	this->name = name;
	selectable = true;
}

void LineItem::draw(WINDOW* win, int y, int x)
{
	mvwaddstr(win, y, x, name.c_str()); //get item name
}

void LineItem::clear()
{
	name = "";
	MenuItem::clear();
}