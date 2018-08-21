#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include "Drawable.h"
#include "curses.h"
#include "direction.h"

class AbstractMenu;

template <typename T>
class MenuItem2 : public Drawable
{
protected:
	AbstractMenu* menu;
	MenuItem2* upItem;
	MenuItem2* downItem;
	MenuItem2* leftItem;
	MenuItem2* rightItem;

	void init(unsigned short y, unsigned short x);
public:
	short index; //the index is the unique identifier for the item. This cannot be removed from this object
				 //int crossRef;
	T crossRef;
	bool selected;
	bool selectable;
	bool hidden; //do not draw hidden items, they have functional purpose only

	unsigned short posY, posX;

	MenuItem2();
	MenuItem2(unsigned short y, unsigned short x);

	/*Resets all properties to default*/
	void clear();

	/*Clear all adjacent links to this item*/
	void clearLinks();

	/*Sets up a bi-directional link with another item*/
	void link(Dir linkDir, MenuItem2* item);

	/*Tears down a bi-directional link with another item. Safe to use on single linked items as well*/
	void unlink(Dir linkDir);

	//getters/setters
	MenuItem2* getLinkedItem(Dir link);
	void setLinkedItem(Dir link, MenuItem2* item); //simple 1-way link method
	void setHidden(bool hidden);
	void setPosition(unsigned short y, unsigned short x);
	void setMenu(AbstractMenu* menu) { this->menu = menu; }
	AbstractMenu* getMenu() { return menu; }

	//static methods
	static void linkItemGroup(std::list<MenuItem2*>& group, Dir link);
	static void linkItemGroup(std::vector<MenuItem2*>& group, Dir link);
	static void linkItemGroups(std::list<MenuItem2*>& group1, std::list<MenuItem2*>& group2, Dir link);
	static void positionItemGroup(std::list<MenuItem2*>& group, int posY, int posX);
};

template <typename T>
MenuItem2<T>::MenuItem2()
{
	init(0, 0);
}

template <typename T>
MenuItem2<T>::MenuItem2(unsigned short y, unsigned short x)
{
	init(y, x);
}

template <typename T>
void MenuItem2<T>::init(unsigned short y, unsigned short x)
{
	upItem = nullptr;
	downItem = nullptr;
	leftItem = nullptr;
	rightItem = nullptr;
	setPosition(y, x);
	clear();
}

template <typename T>
MenuItem2* MenuItem2<T>::getLinkedItem(Dir link)
{
	MenuItem2* item = nullptr;
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

template <typename T>
void MenuItem2<T>::setLinkedItem(Dir link, MenuItem2* item)
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

template <typename T>
void MenuItem2<T>::setPosition(unsigned short y, unsigned short x)
{
	posY = y;
	posX = x;
}

template <typename T>
void MenuItem2<T>::setHidden(bool hidden)
{
	this->hidden = hidden;
}

template <typename T>
void MenuItem2<T>::link(Dir linkDir, MenuItem2* item)
{
	Dir oppDir = getOppositeDir(linkDir);

	setLinkedItem(linkDir, item);

	if (item != nullptr)
		item->setLinkedItem(oppDir, this);
}

template <typename T>
void MenuItem2<T>::unlink(Dir linkDir)
{
	Dir oppDir = getOppositeDir(linkDir);
	MenuItem2* item = getLinkedItem(linkDir);

	setLinkedItem(linkDir, nullptr);
	if (item != nullptr)
		item->setLinkedItem(oppDir, nullptr);
}

template <typename T>
void MenuItem2<T>::clear()
{
	selectable = false;
	selected = false;
	//	crossRef = -1;
	index = -1;
	menu = nullptr;
	hidden = false;

	clearLinks();
}

template <typename T>
void MenuItem2<T>::clearLinks()
{
	for each (Dir dir in allDirs)
	{
		unlink(dir);
	}
}


/*
Links all items in group sequentially according to link passed in
*/
template <typename T>
void MenuItem2<T>::linkItemGroup(std::vector<MenuItem2*>& group, Dir link)
{
	for (int i = 0; i < (int)(group.size() - 1); i++)
	{
		group[i]->link(link, group[i + 1]);
	}
}

template <typename T>
void MenuItem2::linkItemGroup(std::list<MenuItem2*>& group, Dir link)
{
	std::list<MenuItem2*>::iterator endIt = --(group.end()); //must stop at next to last element
	for (std::list<MenuItem2*>::iterator it = group.begin(); it != endIt; it++)
	{
		MenuItem2* item = *it;
		MenuItem2* nextItem = *next(it);

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
template <typename T>
void MenuItem2<T>::linkItemGroups(std::list<MenuItem2*>& group1, std::list<MenuItem2*>& group2, Dir link)
{
	int smallestSize = std::min(group1.size(), group2.size());

	std::list<MenuItem2*>::iterator it1 = group1.begin();
	std::list<MenuItem2*>::iterator it2 = group2.begin();
	for (int i = 0; i < smallestSize; i++)
	{
		(*it1++)->link(link, *it2++);
		//group1[i]->link(link, group2[i]);
	}
}

/*
For now only displays menu items sequentially vertically and assumes MenuItem2 height of 4 (or 3 with a 1 row separator)
*/
template <typename T>
void MenuItem2<T>::positionItemGroup(std::list<MenuItem2*>& group, int posY, int posX)
{
	for each (MenuItem2* item in group)
	{
		item->setPosition(posY, posX);
		posY += 4;
	}

	/*for (int i = 0; i < group.size(); i++, posY += 4)
	{
	group[i]->setPosition(posY, posX);
	}*/
}



