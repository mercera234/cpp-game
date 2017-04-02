#include "MenuItem.h"

MenuItem::MenuItem()
{
	setPosition(0, 0);
	clear();
}

MenuItem::MenuItem(unsigned short y, unsigned short x)
{
	setPosition(y, x);
	clear();
}

void MenuItem::setPosition(unsigned short y, unsigned short x)
{
	posY = y;
	posX = x;
}

void MenuItem::clear()
{
	selectable = false;
	selected = false;
	crossRef = -1;
	index = -1;
	upItem = downItem = leftItem = rightItem = NULL;
	menu = NULL;
	hidden = false;
}

void MenuItem::setHidden(bool hidden)
{
	this->hidden = hidden;
}

void MenuItem::link(int link, MenuItem* item)
{
	this->link(true, link, item);
}

void MenuItem::link(bool setLink, int link, MenuItem* item)
{
	MenuItem** linkA = NULL;
	MenuItem** linkB = NULL;

	switch (link)
	{
	case UP_LINK: 
		linkA = &upItem;
		linkB = &item->downItem;
		break;
	case DOWN_LINK: 
		linkA = &downItem;
		linkB = &item->upItem;
		break;
	case LEFT_LINK: 
		linkA = &leftItem;
		linkB = &item->rightItem;
		break;
	case RIGHT_LINK: 
		linkA = &rightItem;
		linkB = &item->leftItem;
		break;
	}
	
	if (setLink)
	{
		*linkA = item;
		*linkB = this;
	}
	else
	{
		*linkA = NULL;
		*linkB = NULL;
	}
	
}
