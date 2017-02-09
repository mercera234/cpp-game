#pragma once
#include "Controllable.h"
//#include "MenuItem.h"
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






/*

Get row from element
rowmajor: element / menuCols
colmajor: element / menuRows

Get col from element
rowmajor: element % menuCols
colmajor: element % menuRows

Get element from row and col
rowmajor: row * menuCols + col
colmajor: col * menuRows + row

*/
struct MenuResponse
{
	short index;
	short crossref;
	bool itemChosen;
};

struct MenuItem
{
	unsigned short index;
	short crossref;
	bool itemChosen;
	string name;
	int selectable;
};


class Menu : public Controllable
{
private:
	unsigned short itemCount; //the current # of items the menu has
	MenuItem* items;

	/*
	the maximum # of items the menu can have
	capacity should always = menuRows * menuCols
	*/
	unsigned short capacity; 
	unsigned short menuRows;
	unsigned short menuCols;
	unsigned short dividerLen;
	unsigned short colWidth; //full width of column = maxNameLength + 2 (mark length) + dividerLen
	//the maximum length to use for all names, can be used to set a menu with a uniform column width
	//if not set, then there is no limit
	unsigned short maxNameLength;
	
	int* crossRef; //a cross reference array so that menu selections can return a more desired value
	
	/*row major order = true
	1 2 3
	4 5 6
	
	col major order = false
	1 3 5
	2 4 6*/
	bool majorOrder; //default is row major

	//these dimensions define how many visible menuRows and menuCols there are in a scrolling menu
	unsigned short visibleRows;
	unsigned short visibleCols;
	
	unsigned short topRow; //the top menu row that is visible
	unsigned short leftCol; //the left most col that is visible
	short currentIndex; //the currently selected item
	unsigned char pad; 
	char mark[2];
	bool markSide; //true = left; false = right
	int colorPair;
	bool wrapAround;

	//unimplemented properties
	unsigned short justification;

	void setDefaults();

	void drawMenu();
	void drawItem(int row, int col);

	int getElement(int row, int col);

	int rowMajorDriver(int input);
public:
	Menu(WINDOW* win, int rows, int cols);

	void setMaxNameLength(int length);
	//void setKeypad(bool set);
	void setMajorOrder(bool majorOrder);
	bool setItem(string name, string itemDesc, int element, int crossRefNdx);
	void setSelectedIndex(int index);
	int getCrossRefIndex();
	int getCurrentIndex();
	void setMarkSide(bool markSide);
	void setWrapAround(bool wrap);
	void setColor(int colorPair);
	void draw(); //overridden
	
	int driver(int input);
	MenuItem* getSelectedItem();
	MenuItem* getItem(int y, int x);
	virtual ~Menu();
};

