#include "MenuItem.h"
#include "AbstractMenu.h"
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
	upItem = nullptr;
	downItem = nullptr;
	leftItem = nullptr;
	rightItem = nullptr;
	setPosition(y, x);
	clear();
}

void MenuItem::calcDrawingVars()
{
	if (menu == NULL) //item is not in a menu. This is ok for testing
	{
		offY = 0;
		offX = 0;
		win = stdscr;
		topRow = posY;
		cursorX = posX;
		cursorLen = 0;
		itemX = cursorX;
	}
	else //item is in menu
	{
		offY = menu->getUlY();
		offX = menu->getUlX();
		win = menu->getWindow();
		topRow = posY - offY;
		cursorX = posX - offX;
		cursorLen = menu->getCursor().length();
		itemX = cursorX + cursorLen;
	}
}

void MenuItem::draw()
{
	calcDrawingVars();

	if (menu != NULL &&
		(this == menu->getCurrentItem() || selected))
	{
		drawCursor();
	}
}

void MenuItem::drawCursor()
{
	if (menu == NULL)
		return;

	mvwaddstr(win, topRow, cursorX,
	menu->getCursor().c_str());
}

MenuItem* MenuItem::getLinkedItem(Dir link)
{
	MenuItem* item = nullptr;
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

void MenuItem::setLinkedItem(Dir link, MenuItem* item)
{
	switch (link)
	{
	case Dir::UP:
		upItem = item;
		break;
	case Dir::DOWN:
		downItem = item;
		break;
	case Dir::LEFT:
		leftItem = item;
		break;
	case Dir::RIGHT:
		rightItem = item;
		break;
	}
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

void MenuItem::link(Dir linkDir, MenuItem* item)
{
	Dir oppDir = getOppositeDir(linkDir);

	setLinkedItem(linkDir, item);

	if(item != nullptr)
		item->setLinkedItem(oppDir, this);
}

void MenuItem::unlink(Dir linkDir)
{
	Dir oppDir = getOppositeDir(linkDir);
	MenuItem* item = getLinkedItem(linkDir);

	setLinkedItem(linkDir, nullptr);
	if(item != nullptr)
		item->setLinkedItem(oppDir, nullptr);
}


void MenuItem::clear()
{
	selectable = false;
	selected = false;
//	crossRef = -1;
	index = -1;
	menu = nullptr;
	hidden = false;

	clearLinks();
}

void MenuItem::clearLinks()
{
	for each (Dir dir in allDirs)
	{
		unlink(dir);
	}
}


/*
Links all items in group sequentially according to link passed in
*/
void MenuItem::linkItemGroup(std::vector<MenuItem*>& group, Dir link)
{
	for (int i = 0; i < (int)(group.size() - 1); i++)
	{
		group[i]->link(link, group[i + 1]);
	}
}

void MenuItem::linkItemGroup(std::list<MenuItem*>& group, Dir link)
{
	if (group.size() <= 0)
		return;

	std::list<MenuItem*>::iterator endIt = --(group.end()); //must stop at next to last element
	for (std::list<MenuItem*>::iterator it = group.begin(); it != endIt; it++)
	{
		MenuItem* item = *it;
		MenuItem* nextItem = *next(it);
		
		item->link(link, nextItem);
	}
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
	if (group1.size() <= 0 || group2.size() <= 0)
		return;

	int smallestSize = std::min(group1.size(), group2.size()); 
	
	std::list<MenuItem*>::iterator it1 = group1.begin();
	std::list<MenuItem*>::iterator it2 = group2.begin();
	for (int i = 0; i < smallestSize; i++)
	{
		(*it1++)->link(link, *it2++);
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
}



