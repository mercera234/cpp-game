#pragma once
#include "Controllable.h"
#include "MenuItem.h"
#include "curses.h"
#include <iostream>
using namespace std;
#include "AbstractMenu.h"

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

/*
A grid menu will only ever be drawn in a strict grid with consistent row and col measurements
*/
class GridMenu : public AbstractMenu
{
private:
	/*
	the maximum # of items the menu can have
	capacity should always = menuRows * menuCols
	*/
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

	unsigned short ulMenuRow;
	unsigned short ulMenuCol;

	bool posted; //Grid Menus have to be posted. They should not be displayed without having their internal pointers hooked up
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
	
	//int colorPair;
	bool wrapAround;

	//unimplemented properties
	unsigned short justification;

	void setDefaults();

	void drawMenu();
	void drawItem(int row, int col);//

	int getElement(int row, int col);

	void dirDriver(int input);
	//void dirDriver3(int input);
	/*void dirDriver2(int input);
	bool wrapOccurred(int navReq, int index);*/
	void setRowHeight();
	void setColWidth();
	void selectItem(int index);
	void linkItems();
	void wrapLinks();
	//void unsetWrapLinks();
public:
	GridMenu(WINDOW* win, int rows, int cols);
	void resetItems(int rows, int cols);
	//int getCurrentIndex();
	void disableItem(int y, int x);
	void draw(); //overridden

	void setMajorOrder(bool majorOrder);
	void setItem(MenuItem* item);
	void setSelectedIndex(int index);
	void setMarkSide(bool markSide);
	void setWrapAround(bool wrap);
	//void setColor(int colorPair);
	void setItemHeight(int height);
	void setItemWidth(int width);
	unsigned short getItemWidth() { return itemWidth; }
	unsigned short getItemHeight() { return itemHeight; }
	void setRowSepLength(int length);
	void setColSepLength(int length);
	bool post(bool post);
	
	int driver(int input);
	MenuItem* getItem(int y, int x);//
	~GridMenu();
};

