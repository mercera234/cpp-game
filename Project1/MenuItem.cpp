#include "MenuItem.h"
#include <algorithm>
using namespace std;

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

/*
Links all items in group sequentially according to link passed in
*/
void MenuItem::linkItemGroup(vector<MenuItem*>& group, int link)
{
	for (int i = 0; i < group.size() - 1; i++)
	{
		group[i]->link(link, group[i + 1]);
	}
}

void MenuItem::linkItemGroup(list<MenuItem*>& group, int link)
{
	list<MenuItem*>::iterator endIt = --(group.end()); //must stop at next to last element
	for (list<MenuItem*>::iterator it = group.begin(); it != endIt; it++)
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
void MenuItem::linkItemGroups(list<MenuItem*>& group1, list<MenuItem*>& group2, int link)
{
	int smallestSize = min(group1.size(), group2.size()); 
	
	list<MenuItem*>::iterator it1 = group1.begin();
	list<MenuItem*>::iterator it2 = group2.begin();
	for (int i = 0; i < smallestSize; i++)
	{
		(*it1++)->link(link, *it2++);
		//group1[i]->link(link, group2[i]);
	}
}

/*
For now only displays menu items sequentially vertically and assumes menuitem height of 4 (or 3 with a 1 row separator)
*/
void MenuItem::positionItemGroup(list<MenuItem*>& group, int posY, int posX)
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
