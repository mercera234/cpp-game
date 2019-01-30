#pragma once
#include "Controllable.h"
#include "InputProcessor.h"
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

const short NO_CUR_ITEM = -1;
const std::string defaultCursor = "->";
const std::string shortCursor = ">";

class AbstractMenu : public Controllable, public InputProcessor
{
protected:
	/* The container for the items.
	The current size is the same as the maximum items the menu can have currently. 
	All menu items being added should be pointers to heap allocated objects, not references.*/
	std::vector<MenuItem*> items; 
	
	short curIndex; //index of currently selected item
	MenuItem* curItem; //the currently selected item
	
	/*The menu cursor */
	std::string cursor;

	bool allowSwap;
	int swapState;


	chtype colorPair; //this should be moved to controllable


	virtual void dirDriver(int input) = 0;

	bool validateIndex(int index);
	void resetCurrentItem();


public:
	AbstractMenu();

	/* The total number of items the menu can grow to without reallocating memory*/
	void setMaxCapacity(size_t value);
	
	/* The total number of items the menu is currently allowed to have. May be less than capacity.*/
	void setMaxItems(unsigned short value);
	
	/* Set item based on index of item. Returns true on success.*/
	bool setItem(MenuItem* item);

	/* Get item at index.*/
	virtual MenuItem* getItem(int index);
	
	/* Delete the MenuItem pointed to at index. True on success*/
	bool clearItem(int index);

	/* Delete all MenuItems pointed to. Does not alter size or capacity.*/
	void clearItems();
	
	//TODO replace this by having this class inherit InputProcessor interface
	/* Handle menu input from user*/
	virtual int driver(int input) = 0;

	virtual void processInput() = 0;

	//convenience methods(we might not need these next two)
	/* Set item at index with value of 'selected'. True on success. */
	bool setSelected(int index, bool selected);

	/* Grant/revoke ability for item at index to be 'selected'. True on success. */
	bool setSelectable(int index, bool selectable);

	static MenuItem* basicMenuDriver(int input, AbstractMenu* m);


	//getters/setters
	std::string getCursor() { return cursor; }
	void setCursor(const std::string& cursor) { this->cursor = cursor; }

	/* Set the item at index. This item will be drawn with the cursor. */
	bool setCurrentItem(int index);

	/* Get item pointed to by cursor.*/
	MenuItem* getCurrentItem() { return curItem; }

	size_t getMaxCapacity() { return items.capacity(); }
	size_t getMaxItems() { return items.size(); }

	void setColorPair(chtype colorPair) { this->colorPair = colorPair; }
};
