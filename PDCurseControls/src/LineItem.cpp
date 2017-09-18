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
	int offY = menu->getUlY();
	int offX = menu->getUlX();

	int markPosX = 0;
	int itemPosX = 0;
	
	markPosX = posX - offX;
	itemPosX = markPosX + menu->getCursor().length();
	
	WINDOW* win = menu->getWindow();
	if (selected)
		wattron(win, A_REVERSE);
	else
		wattroff(win, A_REVERSE);
	
	if (this == menu->getCurrentItem()) //draw mark
	{
		mvwaddstr(win, posY - offY, markPosX, menu->getCursor().c_str()); 
	}

	//draw item
	switch (field)
	{
	case Fielddraw::DRAW_ICON: 
		mvwaddch(win, posY - offY, itemPosX, icon); 
		break;

	case Fielddraw::DRAW_NAME: 
		mvwaddstr(win, posY - offY, itemPosX, name.c_str());	
		break;
	}
	
	
}

void LineItem::clear()
{
	name = "";
	MenuItem::clear();
}