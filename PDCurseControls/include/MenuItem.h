#pragma once

#include <iostream>
#include <vector>
#include <list>
#include "Drawable.h"
#include "curses.h"
#include "direction.h"

class AbstractMenu;
class MenuItem : public Drawable
{
protected:
	AbstractMenu* menu;
	MenuItem* upItem;
	MenuItem* downItem;
	MenuItem* leftItem;
	MenuItem* rightItem;

	void init(unsigned short y, unsigned short x);

public:
	short index;
	int crossRef;
	bool selected;
	bool selectable;
	bool hidden; //do not draw hidden items, they have functional purpose only

	unsigned short posY, posX;
	
	MenuItem();
	MenuItem(unsigned short y, unsigned short x);
	
	/*
	Resets all properties to default
	*/
	void clear();

	/*
	Clear all adjacent links to this item
	*/
	void clearLinks();

	void link(bool setLink, Dir link, MenuItem* item);
	void link(Dir link, MenuItem* item);
	
	//getters/setters
	MenuItem* getLinkedItem(Dir link);
	void setHidden(bool hidden);
	void setPosition(unsigned short y, unsigned short x);
	void setMenu(AbstractMenu* menu) { this->menu = menu; }
	AbstractMenu* getMenu() { return menu; }

	//static methods
	static void linkItemGroup(std::list<MenuItem*>& group, Dir link);
	static void linkItemGroup(std::vector<MenuItem*>& group, Dir link);
	static void linkItemGroups(std::list<MenuItem*>& group1, std::list<MenuItem*>& group2, Dir link);
	static void positionItemGroup(std::list<MenuItem*>& group, int posY, int posX);
};
