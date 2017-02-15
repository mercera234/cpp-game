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
	this->win = win;

	capacity = rows * cols;
	
	//this array may be deprecated! crossref is now in menuitem
	crossRef = new int[capacity];
	for (int i = 0; i < capacity; i++)
	{
		crossRef[i] = -1; //initialize all crossRefs to -1 to indicate they are linked to nothing
	}

	items = new MenuItem[capacity];

	focusable = true; //all menus accept key input
	setDefaults();

	visibleRows = getmaxy(win); //as many rows that can fit in the window being used
	visibleCols = getmaxx(win) / colWidth;

	totalRows = menuRows;
	totalCols = menuCols * colWidth;
}


void Menu::setDefaults()
{
	itemCount = 0;
	majorOrder = ROW_MAJOR; //default

	topRow = 0; //the top menu item that is visible
	leftCol = 0;
	currentIndex = 0; //default index will be top left item

	
	pad = ' '; //space by default

	mark[0] = '-';//set default mark to ->
	mark[1] = '>';
	mark[2] = 0;
	markSide = LEFT_MARK;

	disabledMark[0] = disabledMark[1] = 'X';
	disabledMark[2] = 0;

	dividerLen = 1;
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
	items[element].index = element;
	items[element].itemChosen = false;
	items[element].crossref = crossRefNdx;
	items[element].name = name;
	items[element].selectable = true;

	//next line may be deprecated
	crossRef[element] = crossRefNdx;//unused crossrefs will be -1 by default
	
	return true;
}

void Menu::drawMenu()
{ 
	//clear the submenu
	wclear(win);

	wattron(win, COLOR_PAIR(colorPair));
	//render each item
	if (majorOrder == ROW_MAJOR)
	{
		for (int row = topRow; row < topRow + visibleRows; row++)
		{
			for (int col = leftCol; col < leftCol + visibleCols; col++)
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

	char* usedMark;

	usedMark = mark;
	if (items[element].selectable == false && items[element].name.compare("") != 0)
	{
		usedMark = disabledMark;
	}

	//print mark and name in correct order
	if (markSide == LEFT_MARK)
	{
		if(element == currentIndex)
			mvwaddstr(win, row - topRow, (col - leftCol) * colWidth, usedMark); //print blot mark
		mvwaddnstr(win, row - topRow, (col - leftCol) * colWidth + 2, items[element].name.c_str(), maxNameLength); //get item name
	}
	else //markSide == RIGHT_MARK
	{
		mvwaddnstr(win, row - topRow, (col - leftCol) * colWidth, items[element].name.c_str(), maxNameLength); //get item name

		if (element == currentIndex)
			mvwaddstr(win, row - topRow, (col - leftCol) * colWidth + maxNameLength, usedMark); //print blot mark
	}

	wattroff(win, A_BLINK);
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


void Menu::setMaxNameLength(int length)
{
	maxNameLength = length;
	colWidth = maxNameLength + 2 + dividerLen; //the 2 is the mark length
}

int Menu::rowMajorDriver(int input)
{
	switch (input)
	{
	case REQ_DOWN_ITEM:
		currentIndex += menuCols;
		//handle wraparound
		if (currentIndex >= capacity)
		{
			if (wrapAround)
			{
				currentIndex %= menuCols;
				topRow = 0;
			}
			else
				currentIndex -= menuCols;
		}

		{
			//reset toprow if necessary
			int currRow = currentIndex / menuCols;
			if (currRow >= topRow + visibleRows)
			{
				topRow = currRow + 1 - visibleRows;
			}
		}
		
		break;
	case REQ_UP_ITEM:
		//calculate next position
		currentIndex -= menuCols;
		//handle wraparound
		if (currentIndex < 0)
		{
			if (wrapAround)
			{
				currentIndex += capacity; //yay for this working!
				topRow = menuRows - visibleRows;
			}
			else
				currentIndex += menuCols;
		}
		//reset toprow if necessary
		{
			int currRow = currentIndex / menuCols;
			if (currRow < topRow)
			{
				topRow = currRow;
			}
		}
		
		break;
	case REQ_LEFT_ITEM:
		//calculate next position
		currentIndex--;
		//handle wraparound
		if ((currentIndex + 1) % menuCols == 0)
		{
			if (wrapAround)
			{
				currentIndex += menuCols;
				leftCol = (currentIndex + 1 - visibleCols) % menuCols;
			}
			else
				currentIndex++;
		}
		//reset leftCol if necessary
		{
			int currCol = currentIndex % menuCols;
			if (currCol < leftCol)
			{
				leftCol = currCol;
			}
		}
		break;
	case REQ_RIGHT_ITEM:
		//calculate next position
		currentIndex++;
		//handle wraparound
		if (currentIndex % menuCols == 0)
		{
			if (wrapAround)
			{
				currentIndex -= menuCols;
				leftCol = currentIndex % menuCols;
			}
			else
				currentIndex--;
		}
		//reset leftCol if necessary
		{
			int currCol = currentIndex % menuCols;
			if (currCol >= visibleCols)
			{
				leftCol = currCol + 1 - visibleCols;
			}
		}
		break;
	}

	return currentIndex;
}

/*
colMajorDriver(int input)
{
//switch (input)
//{
//case REQ_DOWN_ITEM:
//	//calculate next position
//	
//		currentIndex++;
//		//handle wraparound
//		if (currentIndex % menuRows == 0) //pressing down shouldn't lead to these values
//		{
//			if (wrapAround)
//			{
//				currentIndex -= menuRows;
//			}
//			else
//				currentIndex--;
//		}

//	break;
//case REQ_UP_ITEM:
//	//calculate next position
//	
//		currentIndex--;
//		//handle wraparound
//		if ((currentIndex + 1) % menuRows == 0) //pressing down shouldn't lead to these values
//		{
//			if (wrapAround)
//			{
//				currentIndex += menuRows;
//			}
//			else
//				currentIndex++;
//		}
//	break;
//case REQ_LEFT_ITEM:
//	//calculate next position
//		currentIndex -= menuRows;
//		//handle wraparound
//		if (currentIndex < 0) //pressing down shouldn't lead to these values
//		{
//			if (wrapAround)
//			{
//				currentIndex += capacity;
//			}
//			else
//				currentIndex += menuRows;
//		}
//	break;
//case REQ_RIGHT_ITEM:
//	//calculate next position
//		currentIndex += menuRows;
//		//handle wraparound
//		if (currentIndex >= capacity) //pressing down shouldn't lead to these values
//		{
//			if (wrapAround)
//			{
//				currentIndex -= capacity;
//			}
//			else
//				currentIndex -= menuRows;
//		}
//	break;
	}
}
*/

int Menu::driver(int input)
{
	if (majorOrder == ROW_MAJOR)
	{
		rowMajorDriver(input); //not returning a value right now
	}
	//else colMajorDriver(input)

	
	switch(input)
	{
	case REQ_TOGGLE_ITEM:
		if (items[currentIndex].selectable == false)
			break;

		items[currentIndex].itemChosen = true;
		break;

	default: break;
	}

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

MenuItem* Menu::getSelectedItem()
{
	MenuItem* item = &items[currentIndex];

	//currentIndex = row * menuCols + col;
	return item->itemChosen ? item : NULL;
}

MenuItem* Menu::getItem(int y, int x)
{
	int row = y;
	int col = x / 3;
	int element = row * menuCols + col;
	return &items[element];
}

MenuItem* Menu::getCurrentItem()
{
	return &items[currentIndex];
}

void Menu::disableItem(int y, int x)
{
	MenuItem* mi = getItem(y, x);
	mi->selectable = false;
}

void Menu::clear()
{
	for (int i = 0; i < capacity; i++)
	{
		MenuItem* item = &items[i];
		item->clear();
	}	
	currentIndex = 0;
	topRow = 0;
	leftCol = 0;
}

Menu::~Menu()
{
	delete [] items;

	delwin(win);
}
