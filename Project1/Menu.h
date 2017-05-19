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

struct MenuSelector
{
	int type; 
	int rowCursor;
	int colCursor;
	short* cursor; //equal to the current Index selected in the menu
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
	MenuItem** items;
	MenuSelector* selector;

	unsigned short itemCount; //the current # of items the menu has
	/*
	the maximum # of items the menu can have
	capacity should always = menuRows * menuCols
	*/
	unsigned short capacity; 
	unsigned short menuRows; //the selectable rows in the menu
	unsigned short menuCols; //selectable cols in menu

	unsigned short colSepLen; //how large of a gap between columns
	unsigned short rowSepLen; //how large of a gap between rows

	unsigned short itemWidth; //width of the column portion that holds the item (not the mark or separator)
	unsigned short colWidth; //item width + mark and separator
	//the maximum length to use for all names, can be used to set a menu with a uniform column width
	//if not set, then there is no limit
	unsigned short itemHeight;
	unsigned short rowHeight; //full height of row = itemHeight + separator
	
	//these dimensions define how many visible menuRows and menuCols there are in a scrolling menu
	unsigned short visibleMenuCols;
	unsigned short visibleMenuRows;

	//int* crossRef; //a cross reference array so that menu selections can return a more desired value
	
	/*row major order = true
	1 2 3
	4 5 6
	
	col major order = false
	1 3 5
	2 4 6*/
	bool majorOrder; //default is row major

	
	short currentIndex; //the currently selected item
	unsigned char pad; 
	string mark;
	
	char disabledMark[3];
	bool markSide; //true = left; false = right
	int colorPair;
	bool wrapAround;

	bool spread; //if selection is spread
	bool spreadable; //allows for spreading selection to an entire column on edge of menu

	//unimplemented properties
	unsigned short justification;

	void setDefaults();

	void drawMenu();
	void drawItem(int row, int col);//

	int getElement(int row, int col);

	void dirDriver(int input);
	bool wrapOccurred(int navReq, int index);
	void setRowHeight();
	void setColWidth();
	void selectItem(int index);
public:
	Menu(WINDOW* win, int rows, int cols);

	int getCurrentIndex();
	void disableItem(int y, int x);
	void draw(); //overridden

	void setMajorOrder(bool majorOrder);
	void setItem(MenuItem* item);
	void setSelectedIndex(int index);
	void setMarkSide(bool markSide);
	void setWrapAround(bool wrap);
	void setSpreadable(bool spread);
	void setColor(int colorPair);
	void setItemHeight(int height);
	void setItemWidth(int width); //should be set wide enough to hold item label data, mark, and separator
	void setRowSepLength(int length);
	void setColSepLength(int length);
	
	void clear();
	
	int driver(int input);
	MenuItem* getItem(int y, int x);//
	MenuItem* getCurrentItem();//
	~Menu();

	//deprecated methods
	bool setItem(string name, string itemDesc, int element, int crossRefNdx);//deprecated
	bool setItem(string name, int element, int crossRefNdx);//deprecated

};

