#pragma once
#include "Controllable.h"
#include "MenuItem.h"
#include "curses.h"
#include <iostream>

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

const std::string defaultCursor = "->";
const std::string shortCursor = ">";

class AbstractMenu : public Controllable
{
protected:
	//MenuItem** items = NULL;
	//unsigned short itemCount; //the current # of items the menu has
	//unsigned short capacity; //the max # of items a menu can have
	//
	/*
	The container for the items.
	The current size is the same as the maximum items the menu can have currently
	*/
	std::vector<MenuItem*> items; 
	
	MenuItem* curItem; //the currently selected item
	
	/*
	The menu cursor
	*/
	std::string cursor;

	chtype colorPair;


	virtual void dirDriver(int input) = 0;

	bool validateIndex(int index);
	void allocateItems(); //deprecated
public:
	AbstractMenu();

	/* The total number of items the menu can grow to without reallocating memory*/
	void setMaxCapacity(size_t value);
	
	/* The total number of items the menu is currently allowed to have. May be less than capacity.*/
	void setMaxItems(unsigned short value);
	
	/* Set item based on index of item. Returns true on success.*/
	bool setItem(MenuItem* item);

	/* Get item at index.*/
	MenuItem* getItem(int index);
	
	/* Set item at index with value of 'selected'. True on success. */
	bool setSelected(int index, bool selected);
	
	/* Delete the MenuItem pointed to at index. True on success*/
	bool clearItem(int index);

	/* Delete all MenuItems pointed to. Does not alter size or capacity.*/
	void clearItems();
	
	/* Handle menu input from user*/
	virtual int driver(int input) = 0;

	static MenuItem* basicMenuDriver(int input, AbstractMenu* m);


	//getters/setters
	std::string getCursor() { return cursor; }

	/* Set the item at index. This item will be drawn with the cursor. */
	bool setCurrentItem(int index);

	/* Get item pointed to by cursor.*/
	MenuItem* getCurrentItem() { return curItem; }

	size_t getMaxCapacity() { return items.capacity(); }
	size_t getMaxItems() { return items.size(); }

	void setColorPair(chtype colorPair) { this->colorPair = colorPair; }
};
