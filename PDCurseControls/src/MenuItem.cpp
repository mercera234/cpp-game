#include "MenuItem.h"
#include <algorithm>

MenuItem::MenuItem()
{
	init(0, 0);
}

MenuItem::MenuItem(unsigned short y, unsigned short x)
{
	init(y, x);
}

void MenuItem::init(unsigned short y, unsigned short x)
{
	upItem = NULL;
	downItem = NULL;
	leftItem = NULL;
	rightItem = NULL;
	setPosition(y, x);
	clear();
}

MenuItem* MenuItem::getLinkedItem(Dir link)
{
	MenuItem* item = NULL;
	switch (link)
	{
	case Dir::UP:
		item = upItem;
		break;
	case Dir::DOWN:
		item = downItem;
		break;
	case Dir::LEFT:
		item = leftItem;
		break;
	case Dir::RIGHT:
		item = rightItem;
		break;
	}

	return item;
}

void MenuItem::setPosition(unsigned short y, unsigned short x)
{
	posY = y;
	posX = x;
}

void MenuItem::setHidden(bool hidden)
{
	this->hidden = hidden;
}

void MenuItem::link(Dir link, MenuItem* item)
{
	this->link(true, link, item);
}

void MenuItem::link(bool setLink, Dir link, MenuItem* item)
{
	MenuItem** linkA = NULL;
	MenuItem** linkB = NULL;

	switch (link)
	{
	case Dir::UP:
		linkA = &upItem;
		linkB = &item->downItem;
		break;
	case Dir::DOWN:
		linkA = &downItem;
		linkB = &item->upItem;
		break;
	case Dir::LEFT:
		linkA = &leftItem;
		linkB = &item->rightItem;
		break;
	case Dir::RIGHT:
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

void MenuItem::clear()
{
	selectable = false;
	selected = false;
	crossRef = -1;
	index = -1;
	menu = NULL;
	hidden = false;

	clearLinks();
}

void MenuItem::clearLinks()
{
	if (upItem != NULL)
		link(false, Dir::UP, upItem);

	if (downItem != NULL)
		link(false, Dir::DOWN, downItem);

	if (leftItem != NULL)
		link(false, Dir::LEFT, leftItem);

	if (rightItem != NULL)
		link(false, Dir::RIGHT, rightItem);
}

/*
Links all items in group sequentially according to link passed in
*/
void MenuItem::linkItemGroup(std::vector<MenuItem*>& group, Dir link)
{
	for (int i = 0; i < group.size() - 1; i++)
	{
		group[i]->link(link, group[i + 1]);
	}
}

void MenuItem::linkItemGroup(std::list<MenuItem*>& group, Dir link)
{
	std::list<MenuItem*>::iterator endIt = --(group.end()); //must stop at next to last element
	for (std::list<MenuItem*>::iterator it = group.begin(); it != endIt; it++)
	{
		MenuItem* item = *it;
		MenuItem* nextItem = *next(it);
		
		item->link(link, nextItem);
	}

	/*for (int i = 0; i < group.size() - 1; i++)
	{
		group[i]->link(link, group[i + 1]);
	}*/
}

/*
Links two separate groups of items in a 1 to 1 ordering for as many that can be done
ex: group1 has {A, B, C}, group2 has {D, E}, link is right
A->D
B->E
C->null
*/
void MenuItem::linkItemGroups(std::list<MenuItem*>& group1, std::list<MenuItem*>& group2, Dir link)
{
	int smallestSize = std::min(group1.size(), group2.size()); 
	
	std::list<MenuItem*>::iterator it1 = group1.begin();
	std::list<MenuItem*>::iterator it2 = group2.begin();
	for (int i = 0; i < smallestSize; i++)
	{
		(*it1++)->link(link, *it2++);
		//group1[i]->link(link, group2[i]);
	}
}

/*
For now only displays menu items sequentially vertically and assumes menuitem height of 4 (or 3 with a 1 row separator)
*/
void MenuItem::positionItemGroup(std::list<MenuItem*>& group, int posY, int posX)
{
	for each (MenuItem* item in group)
	{
		item->setPosition(posY, posX);
		posY += 4;
	}

	/*for (int i = 0; i < group.size(); i++, posY += 4)
	{
		group[i]->setPosition(posY, posX);
	}*/
}



