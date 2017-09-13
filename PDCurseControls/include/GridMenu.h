#pragma once
#include "Controllable.h"
#include "MenuItem.h"
#include "curses.h"
#include <iostream>
#include "AbstractMenu.h"

/*
A grid menu will only ever be drawn in a strict grid with consistent row and col measurements. 
It will always be in row major format.
1 2 3
4 5 6
*/
class GridMenu : public AbstractMenu
{
private:
	unsigned short menuRows; //navigatable rows in the menu
	unsigned short menuCols; //navigatable cols in menu

	unsigned short rowSepLen; //how large of a gap between rows
	unsigned short colSepLen; //how large of a gap between columns
	
	unsigned short rowHeight; //itemHeight + row separator length
	unsigned short itemHeight; //total height of the rows used to hold the item

	unsigned short colWidth; //item width + cursor length + col separator length
	unsigned short itemWidth; //width of the column portion that holds the item (not the mark or separator)
		
	unsigned short visibleMenuRows; //The visible rows in menu. Could <= menuRows.
	unsigned short visibleMenuCols; //The visible cols in menu. Could <= menuCols

	unsigned short ulMenuRow; //the top most visible row in the menu
	unsigned short ulMenuCol; //the left most visible col in the menu

	//not sure if I need this
	bool posted; //Grid Menus have to be posted. They should not be displayed without having their internal pointers hooked up

	bool wrapAround; //true if a user can navigate off boundary of menu to go from one edge to its opposite.



	void setDefaults();

	void drawMenu();
	void drawItem(int row, int col);

	int getElement(int row, int col);

	void dirDriver(int input);
	
	void setRowHeight();
	void setColWidth();
	
	void linkItems();
	void wrapLinks();

	
	//void dirDriver3(int input);
	/*void dirDriver2(int input);
	bool wrapOccurred(int navReq, int index);*/
	//void unsetWrapLinks();
public:
	GridMenu(WINDOW* win, int rows, int cols);
	void resetItems(int rows, int cols);
	
	void disableItem(int y, int x);
	void draw(); //overridden

	
	void setItem(MenuItem* item);
	//void setSelectedIndex(int index);
	
	//getters/setters
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

