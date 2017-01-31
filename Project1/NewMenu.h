#pragma once
#include "MenuItem.h"
#include "curses.h"

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

/*

Get row from element
rowmajor: element / cols
colmajor: element / rows

Get col from element
rowmajor: element % cols
colmajor: element % rows

Get element from row and col
rowmajor: row * cols + col
colmajor: col * rows + row

*/

class NewMenu
{
private:
	int itemCount; //the current # of items the menu has
	MenuItem* items;
	WINDOW* win; //the space where the menu is rendered
	
	/*
	the maximum # of items the menu can have
	capacity should always = rows * cols
	*/
	int capacity; 
	int rows;
	int cols;
	int colWidth; //full width of column = maxNameLength + 2 (mark length) + 1(divider)
	//the maximum length to use for all names, can be used to set a menu with a uniform column width
	//if not set, then there is no limit
	int maxNameLength;  
	
	int* crossRef; //a cross reference array so that menu selections can return a more desired value

	/*row major order = true
	1 2 3
	4 5 6
	
	col major order = false
	1 3 5
	2 4 6*/
	bool majorOrder; //default is row major

	//these dimensions define how many visible rows and cols there are in a scrolling menu
	int visibleRows;
	int visibleCols;
	
	int topRow; //the top menu item that is visible
	int selectedIndex; //the currently selected item
	unsigned char pad; 
	char mark[2];
	bool markSide; //true = left; false = right
	//int colorPair;
	bool wrapAround;

	void setDefaults();

	void drawItem(int row, int col);

	int getElement(int row, int col);
public:
	NewMenu(WINDOW* win, int rows, int cols);

	void setKeypad(bool set);
	void setMajorOrder(bool majorOrder);
	bool setItem(string name, string itemDesc, int element, int crossRefNdx);
	void setSelectedIndex(int index);
	int getCrossRefIndex();
	void setMarkSide(bool markSide);
	void setWrapAround(bool wrap);
	
	void draw();
	int driver(int input);
	virtual ~NewMenu();
};

