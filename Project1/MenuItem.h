#pragma once

#include <iostream>
#include <vector>
#include <list>
using namespace std;
#include "curses.h"

#define UP_LINK 0
#define DOWN_LINK 1
#define LEFT_LINK 2
#define RIGHT_LINK 3

class AbstractMenu;
struct MenuItem
{
private:
public:
	AbstractMenu* menu;

	short index;
	int crossRef;
	bool selected;
	bool selectable;
	bool hidden; //do not draw hidden items, they have functional purpose only

	MenuItem* upItem;
	MenuItem* downItem;
	MenuItem* leftItem;
	MenuItem* rightItem;

	unsigned short posY, posX;
	
	virtual void draw(WINDOW* win) = 0;
	MenuItem();
	MenuItem(unsigned short y, unsigned short x);
	void setPosition(unsigned short y, unsigned short x);
	void setMenu(AbstractMenu* menu) { this->menu = menu; }
	void clear();
	void link(bool setLink, int link, MenuItem* item);
	void link(int link, MenuItem* item);
	void setHidden(bool hidden);

	static void linkItemGroup(list<MenuItem*>& group, int link);
	static void linkItemGroup(vector<MenuItem*>& group, int link);
	static void linkItemGroups(list<MenuItem*>& group1, list<MenuItem*>& group2, int link);
	static void positionItemGroup(list<MenuItem*>& group, int posY, int posX);
};
