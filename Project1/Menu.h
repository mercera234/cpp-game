#pragma once
#include "Controllable.h"
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



struct MenuResponse
{
	short index;
	short crossref;
	bool itemChosen;
};


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

class Menu : public Controllable
{
private:
	unsigned short itemCount; //the current # of items the menu has
	MenuItem* items;
	//WINDOW* win; //the outer frame window
	WINDOW* win; //the containing window of the menu
	//string title;
	/*
	the maximum # of items the menu can have
	capacity should always = menuRows * menuCols
	*/
	unsigned short capacity; 
	unsigned short menuRows;
	unsigned short menuCols;
	unsigned short colWidth; //full width of column = maxNameLength + 2 (mark length) + 1(divider)
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
	
	unsigned short topRow; //the top menu item that is visible
	unsigned short currentIndex; //the currently selected item
	unsigned char pad; 
	char mark[2];
	bool markSide; //true = left; false = right
	int colorPair;

	//unimplemented properties
	bool wrapAround;
	unsigned short justification;

	void setDefaults();

	//void drawOuterWin();
	void drawMenu();
	void drawItem(int row, int col);

	int getElement(int row, int col);
public:
	/*Menu(int rows, int cols, 
		int winRows, int winCols, int winY, int winX,
		int subWinRows, int subWinCols, int subWinY, int subWinX);*/
	
	Menu(WINDOW* win, int rows, int cols);

	//void setTitle(string title);
	void setMaxNameLength(int length);
	void setKeypad(bool set);
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
	virtual ~Menu();
};

