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

	bool wrapAround; //true if a user can navigate off boundary of menu to go from one edge to its opposite.

	bool posted; //Grid Menus have to be posted. They should not be displayed without having their internal pointers hooked up

	//private methods
	void init();

	//void drawMenu();
	
	int getElement(int row, int col);

	void dirDriver(int input);//handle directional input
	void mouseDriver();
	
	void setRowHeight();
	void setColWidth();
	
	bool validateForPost();
	void linkItems();
	void wrapLinks();

	//these methods should maybe use an associative container instead of a vector
	void saveLinkIndices(MenuItem* item, std::vector<short>& indices);
	void restoreLinkIndices(MenuItem* item, const std::vector<short>& indices);
	
public:
	GridMenu();
	/* Menu constructor
	A user can initially specify how many menuRows/menuCols the menu can have */
	GridMenu(WINDOW* win, int rows, int cols);
	
	/*Resets the entire grid menu with new dimensions*/
	void resetItems(int rows, int cols);

	/*Post the menu. This method is important because it links up the items internally after all items have been filled.
	This cannot be done when menu is first declared because it only allocates room for pointers to menu items, not the items themselves.*/
	bool post(bool post);

	void draw(); //overridden

	//overridden
	//handle key input
	int driver(int input); 
	

	/*Exchange two items in the same menu*/
	bool swapItems(int index1, int index2);

	//overridden
	//Set item based on item index
	void setItem(MenuItem* item); 

	//Set item located at menuRow, menuCol in grid
	void setItem(MenuItem* item, unsigned short menuRow, unsigned short menuCol);
	
	//Get item located at menuRow, menuCol in grid
	virtual MenuItem* getItem(int menuRow, int menuCol);

	//Sets if menu can be wrapped around. Will link or unlink outer items as necessary.
	void setWrapAround(bool wrap);

	//getters/setters
	void setItemHeight(unsigned short height);
	void setItemWidth(unsigned short width);
	unsigned short getItemWidth() { return itemWidth; }
	unsigned short getItemHeight() { return itemHeight; }
	void setRowSepLength(unsigned short length);
	void setColSepLength(unsigned short length);
	bool isPosted() { return posted; }
	
	/* Remove all items from menu, but do not destroy menu itself. Capacity is unaltered */
	~GridMenu();
};

