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
	//win = mainWin;
	this->win = win;

	capacity = rows * cols;

	//by default visible menuRows/menuCols will be big enough to display entire menu without scrolling
	visibleRows = rows;
	visibleCols = cols;

	crossRef = new int[capacity];
	for (int i = 0; i < capacity; i++)
	{
		crossRef[i] = -1; //initialize all crossRefs to -1 to indicate they are linked to nothing
	}

	items = new MenuItem[capacity];

	setDefaults();
}


void Menu::setDefaults()
{
	itemCount = 0;
	majorOrder = ROW_MAJOR; //default

	topRow = 0; //the top menu item that is visible
	currentIndex = 0; //default index will be top left item

	
	pad = ' '; //space by default

	mark[0] = '-';//set default mark to ->
	mark[1] = '>';
	markSide = LEFT_MARK;

	setMaxNameLength(15); //default is 15

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

void Menu::setKeypad(bool set)
{
	keypad(win, set);
}

void Menu::setMarkSide(bool markSide)
{
	this->markSide = markSide;
	if (markSide == LEFT_MARK)
	{
		mark[0] = '-';
		mark[1] = '>';
	}
	else //markSide == RIGHT_MARK
	{
		mark[0] = '<';
		mark[1] = '-';
	}
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
bool Menu::setItem(string name, string itemDesc, int element, int crossRefNdx)
{
	crossRef[element] = crossRefNdx;//unused crossrefs will be -1 by default
	int padAmount = maxNameLength - name.length();

	if (padAmount > 0)
	{
		name.append(padAmount, pad);
		items[element].setName(name);	
	}
	//else
	//	items[element].setName("ERROR");
	
	return true;
}

//void Menu::drawOuterWin()
//{
//	wattron(win, COLOR_PAIR(colorPair));
//	//draw border
//	box(win, 0, 0);
//	wbkgd(win, ' ' | COLOR_PAIR(colorPair));
//
//	//draw title (by default, put one space to left of window)
//	mvwaddstr(win, 0, 1, title.c_str());
//}

void Menu::drawMenu()
{ 
	//clear the submenu
	wclear(win);

	wattron(win, COLOR_PAIR(colorPair));
	//render each item
	if (majorOrder == ROW_MAJOR)
	{
		for (int row = 0; row < menuRows; row++)
		{
			for (int col = 0; col < menuCols; col++)
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
	char* mark;

	//choose correct mark and attribute if item is highlighted
	if (element == currentIndex)
	{
	//	wattron(subWin, A_REVERSE);
		mark = this->mark;
	}
	else
		mark = "  ";

	//print mark and name in correct order
	if (markSide == LEFT_MARK)
	{
		mvwaddstr(win, row, col * colWidth, mark); //print blot mark
		mvwaddstr(win, row, col * colWidth + 2, items[element].getName().c_str()); //get item name
	}
	else //markSide == RIGHT_MARK
	{
		mvwaddstr(win, row, col * colWidth, items[element].getName().c_str()); //get item name
		mvwaddstr(win, row, col * colWidth + maxNameLength, mark); //print blot mark
	}

	//turn off highlight attribute if turned on earlier
	if (element == currentIndex)
	{
	//	wattroff(subWin, A_REVERSE);
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
	//drawOuterWin();
	drawMenu();
	//wnoutrefresh(win);
	wnoutrefresh(win);
}

//int Menu::driver(int input)
//{
//	int testIndex = currentIndex;
//	switch (input)
//	{
//	case REQ_DOWN_ITEM:
//		//ignore wraparound for right now and do this for one column menus to start with
//		if (majorOrder == ROW_MAJOR)
//			testIndex += menuCols;
//		else
//			testIndex++;
//		break;
//	case REQ_UP_ITEM:
//		if (majorOrder == ROW_MAJOR)
//			testIndex -= menuCols;
//		else
//			testIndex--;
//		break;
//	case REQ_LEFT_ITEM: 
//		if (majorOrder == ROW_MAJOR)
//			testIndex--;
//		else
//			testIndex -= menuRows;
//		break;
//	case REQ_RIGHT_ITEM: 
//		if (majorOrder == ROW_MAJOR)
//			testIndex++;
//		else
//			testIndex += menuRows;
//		break; 
//	default: break;
//	}
//	currentIndex = testIndex;
//	return currentIndex;
//}

void Menu::setMaxNameLength(int length)
{
	maxNameLength = length;
	colWidth = maxNameLength + 2 + 1;
}


int Menu::driver(int input)
{
	int testIndex = currentIndex;
	switch (input)
	{
	case REQ_DOWN_ITEM:
		//ignore wraparound for right now and do this for one column menus to start with
		if (majorOrder == ROW_MAJOR)
			testIndex += menuCols;
		else
			testIndex++;
		break;
	case REQ_UP_ITEM:
		if (majorOrder == ROW_MAJOR)
			testIndex -= menuCols;
		else
			testIndex--;
		break;
	case REQ_LEFT_ITEM:
		if (majorOrder == ROW_MAJOR)
			testIndex--;
		else
			testIndex -= menuRows;
		break;
	case REQ_RIGHT_ITEM:
		if (majorOrder == ROW_MAJOR)
			testIndex++;
		else
			testIndex += menuRows;
		break;
	default: break;
	}
	currentIndex = testIndex;
	return currentIndex;
}


int Menu::getCrossRefIndex()
{
	int crossRefNdx = crossRef[currentIndex];
	//-1 in the crossref means it was never set, this also implies that
	//  -1 can never be used as a value we want to return to the user unless it is cancel
	return (crossRefNdx == -1) ? currentIndex : crossRefNdx;//use crossrefndx only if set
}

int Menu::getCurrentIndex()
{
	return currentIndex;
}

Menu::~Menu()
{
	delete [] items;

	delwin(win);
}
