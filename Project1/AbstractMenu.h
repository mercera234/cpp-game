#pragma once
#include "Controllable.h"
#include "MenuItem.h"
#include "curses.h"
#include <iostream>
using namespace std;

#define CANCEL -1
#define NOTHING -2


/* Define keys - ripped straight from ncurses menu.h file */

#define REQ_LEFT_ITEM           (KEY_MAX + 1)
#define REQ_RIGHT_ITEM          (KEY_MAX + 2)
#define REQ_UP_ITEM             (KEY_MAX + 3)
#define REQ_DOWN_ITEM           (KEY_MAX + 4)
#define REQ_SCR_ULINE           (KEY_MAX + 5)
#define REQ_SCR_DLINE           (KEY_MAX + 6)
#define REQ_SCR_DPAGE           (KEY_MAX + 7)
#define REQ_SCR_UPAGE           (KEY_MAX + 8)
#define REQ_FIRST_ITEM          (KEY_MAX + 9)
#define REQ_LAST_ITEM           (KEY_MAX + 10)
#define REQ_NEXT_ITEM           (KEY_MAX + 11)
#define REQ_PREV_ITEM           (KEY_MAX + 12)
#define REQ_TOGGLE_ITEM         (KEY_MAX + 13)
#define REQ_CLEAR_PATTERN       (KEY_MAX + 14)
#define REQ_BACK_PATTERN        (KEY_MAX + 15)
#define REQ_NEXT_MATCH          (KEY_MAX + 16)
#define REQ_PREV_MATCH          (KEY_MAX + 17)

#define MIN_MENU_COMMAND        (KEY_MAX + 1)
#define MAX_MENU_COMMAND        (KEY_MAX + 17)


#define LEFT_MARK true
#define RIGHT_MARK false

#define ROW_MAJOR true
#define COL_MAJOR false


#define MS_ELEMENT 0
#define MS_ROW 1
#define MS_COL 2


class AbstractMenu : public Controllable
{
protected:
	MenuItem** items = NULL;
	
	unsigned short itemCount; //the current # of items the menu has
	unsigned short capacity; //the max # of items a menu can have

	MenuItem* curItem; //the currently selected item
	unsigned char pad;
	string mark;

	char disabledMark[3];
	bool markSide; //true = left; false = right
	chtype colorPair;

	virtual void dirDriver(int input) = 0;
	void allocateItems();
public:
	void selectItem(int index);// = 0;
	void deSelectItem(int index);// = 0;
	void setMarkSide(bool markSide);
	bool getMarkSide() { return markSide; }
	string getMark() { return mark; }
	void setItem(MenuItem* item);
	void setColorPair(chtype colorPair) { this->colorPair = colorPair; }
	void clearItems();
	//void setColor(int colorPair);
	void setCurrentItem(int index);
	virtual void draw() = 0;
	
	virtual int driver(int input) = 0;
	MenuItem* getCurrentItem();//
};
