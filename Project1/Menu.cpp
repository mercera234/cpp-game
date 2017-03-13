#include "Menu.h"
#include "curses.h"

/*
Menu constructor
A user can initially specify only how many menuRows/menuCols the menu can have
Everything else is given default values, but many fields can be given user-supplied values using other methods
*/
Menu::Menu(WINDOW* win, int rows, int cols)
{
	this->menuRows = rows;
	this->menuCols = cols;

	//we're assuming here for now that null windows will not be passed into this constructor!
	setWindow(win);
	capacity = rows * cols;
	
	items = new MenuItem*[capacity];
	for (int i = 0; i < capacity; i++)
	{
		items[i] = NULL;
	}

	focusable = true; //all menus accept key input
	setDefaults();	
}


void Menu::setItemHeight(int height)
{
	itemHeight = height;
	rowHeight = itemHeight + rowSepLen;
	
	totalRows = menuRows * rowHeight - rowSepLen; //subtract the separator off the last row

	visibleMenuRows = visibleRows / rowHeight;
}


void Menu::setItemWidth(int width)
{
	itemWidth = width;
	colWidth = itemWidth + mark.length() + colSepLen;

	//total columns is the width of the columns times the number of menu columns + length of the column separators between columns
	totalCols = menuCols * colWidth - 1; //subtract 1 because we don't need separator at very end of row
	
	visibleMenuCols = visibleCols / colWidth;
}


void Menu::setDefaults()
{
	itemCount = 0;
	majorOrder = ROW_MAJOR; //default

	//currentIndex = 0; //default index will be top left item
	clear();
	
	pad = ' '; //space by default

	markSide = LEFT_MARK;
	setMarkSide(markSide);

	disabledMark[0] = disabledMark[1] = 'X';
	disabledMark[2] = 0;

	colSepLen = 1;
	
	setItemWidth(16); //default is 16
	
	rowSepLen = 0; //most menus will have no gap between rows
	setItemHeight(1);

	colorPair = 0;//use default color pair
	wrapAround = true; //allow by default
}

void Menu::setColor(int colorPair)
{
	this->colorPair = colorPair;
}

void Menu::setMajorOrder(bool majorOrder)
{
	this->majorOrder = majorOrder;
}


void Menu::setMarkSide(bool markSide)
{
	this->markSide = markSide;
	mark = markSide == LEFT_MARK ? "->" : "<-";
}


void Menu::setSelectedIndex(int index)
{
	currentIndex = index;
}

void Menu::setWrapAround(bool wrap)
{
	wrapAround = wrap;
}

/*
setItem will set the item at the element specified. If element has already been set it will be overwritten.
*/
//bool Menu::setItem(string name, string itemDesc, int element, int crossRefNdx)
//{
//	setItem(name, element, crossRefNdx); //until we find a usage for the item description
//	return true;
//}

/*
setItem will set the item at the element specified. If element has already been set it will be overwritten.
*/
//bool Menu::setItem(string name, int element, int crossRefNdx)
//{
//	//items[element].index = element;
//	//items[element].itemChosen = false;
//	//items[element].crossref = crossRefNdx;
//	//items[element].name = name;
//	//items[element].selectable = true;
//
//	////next line may be deprecated
//	//crossRef[element] = crossRefNdx;//unused crossrefs will be -1 by default
//
//	return true;
//}

void Menu::setItem(MenuItem* item)
{
	items[item->index] = item;
}

void Menu::drawMenu()
{ 
	//clear the submenu
	wclear(win);

	wattron(win, COLOR_PAIR(colorPair));
	//render each item
	if (majorOrder == ROW_MAJOR)
	{
		for (int row = ulY; row < ulY + visibleMenuRows; row++)
		{
			for (int col = ulX; col < ulX + visibleMenuCols; col++)
			{
				drawItem(row, col);
			}
		}
	}
	else //majorOrder == COL_MAJOR
	{
		for (int col = 0; col < menuCols; col++)
		{
			for (int row = 0; row < menuRows; row++)
			{
				drawItem(row, col);
			}
		}
	}
}



void Menu::drawItem(int row, int col)
{
	int element = getElement(row, col);
	if (element >= capacity) //can't draw a null item
		return;

	//print mark and name in correct order
	if (markSide == LEFT_MARK)
	{
		if(element == currentIndex)
			mvwaddstr(win, row - ulY, (col - ulX) * colWidth, mark.c_str()); //print mark

		if(items[element] != NULL)
			items[element]->draw(win, row - ulY, (col - ulX) * colWidth + mark.length());
	}
	else //markSide == RIGHT_MARK
	{
		if (items[element] != NULL)
			items[element]->draw(win, row - ulY, (col - ulX) * colWidth);
			
		if (element == currentIndex)
			mvwaddstr(win, row - ulY, (col - ulX) * colWidth + itemWidth, mark.c_str()); //print mark
	}
}

/*Get element from row and col
rowmajor : row * menuCols + col
colmajor : col * menuRows + row*/
int Menu::getElement(int row, int col)
{
	if (majorOrder == ROW_MAJOR)
	{
		return row * menuCols + col;
	}
	else //majorOrder == COL_MAJOR
	{
		return col * menuRows + row;
	}
}


void Menu::draw()
{
	drawMenu();
	wnoutrefresh(win);
}

//return true if wrap condition is met for the specified direction
//the nav req is the last movement that occurred that brought the user to the current index
bool Menu::wrapOccurred(int navReq)
{
	bool wrapOccurred = false;
	switch (navReq)
	{
	case REQ_DOWN_ITEM: // down at bottom of menu it should wrap around to top of same column
		wrapOccurred = currentIndex >= capacity; break;
	case REQ_UP_ITEM: // up at top of menu it should wrap around to bottom of same column
		wrapOccurred = currentIndex < 0; break;
	case REQ_RIGHT_ITEM: //right at right edge of menu should wrap to first item of same row
		wrapOccurred = currentIndex % menuCols == 0; break;
	case REQ_LEFT_ITEM: //left at left edge of menu should wrap to last item of same row
		wrapOccurred = (currentIndex + 1) % menuCols == 0; break;
	}

	return wrapOccurred;
}

/*
process all directional navigation requests
(only handles requests in row major format currently!)
*/
int Menu::dirDriver(int input)
{
	int navIncr = 0;
	int wrapOffset = 0;
	int* axis = 0;
	int axisPos = 0;
	int visualOffset = 0;
	int visibleDimension;
	switch (input)
	{
	case REQ_DOWN_ITEM:
		navIncr = menuCols;
		axis = &ulY;
		wrapOffset = -capacity;
		axisPos = 0;
		visibleDimension = visibleMenuRows;
		visualOffset = 1 - visibleDimension;
		break;
	case REQ_UP_ITEM:
		navIncr = -menuCols;
		axis = &ulY;
		wrapOffset = capacity;
		visibleDimension = visibleMenuRows;
		axisPos = menuRows - visibleDimension;
		visualOffset = 0;
		break;
	case REQ_RIGHT_ITEM:
		navIncr = 1;
		axis = &ulX;
		wrapOffset = -menuCols;
		axisPos = 0;
		visibleDimension = visibleMenuCols;
		visualOffset = 1 - visibleDimension;
		break;
	case REQ_LEFT_ITEM:
		navIncr = -1;
		axis = &ulX;
		wrapOffset = menuCols;
		visibleDimension = visibleMenuCols;
		axisPos = menuCols - visibleDimension;
		visualOffset = 0;
		break;
	
	}

	//perform movement
	currentIndex += navIncr;
	if(wrapOccurred(input)) //hit edge of menu
	{
		if(wrapAround)
		{
			currentIndex += wrapOffset;
			*axis = axisPos;
		}
		else //no wrapping, reverse the movement
			currentIndex -= navIncr; 
	}

	//get current row or col
	int currAxis = axis == &ulY ? currentIndex / menuCols : currentIndex % menuCols;

	/*if current row or col is either less than the upper left corner of the control 
	or beyond the visual portion of the control*/
	if(currAxis < *axis || currAxis >= *axis + visibleDimension)
	{
		*axis = currAxis + visualOffset;
	}

	return currentIndex;
}



int Menu::driver(int input)
{
	switch (input)
	{
	case REQ_DOWN_ITEM:
	case REQ_UP_ITEM:
	case REQ_RIGHT_ITEM:
	case REQ_LEFT_ITEM: 
		dirDriver(input);
		break;
	case REQ_TOGGLE_ITEM:
		if (items[currentIndex]->selectable == false)
			break;

		//items[currentIndex]->selected = true;
		break;

	default: break;
	}

	return currentIndex;
}


int Menu::getCurrentIndex()
{
	return currentIndex;
}


MenuItem* Menu::getItem(int y, int x)
{
	int row = y;
	int col = x / 3;
	int element = row * menuCols + col;
	return items[element];
}

MenuItem* Menu::getCurrentItem()
{
	return items[currentIndex];
}

void Menu::disableItem(int y, int x)
{
	MenuItem* mi = getItem(y, x);
	mi->selectable = false;
}

/*
Remove all items from menu, but do not destroy menu itself. Capacity is unaltered
*/
void Menu::clear()
{
	for (int i = 0; i < capacity; i++)
	{
		delete items[i];
		items[i] = 0;
	}	
	currentIndex = 0;
	setPosition(0, 0);
}

Menu::~Menu()
{
	clear();
	delete [] items;

	delwin(win);
}
