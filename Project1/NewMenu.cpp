#include "NewMenu.h"
#include "curses.h"

/*
NewMenu constructor
A user can initially specify only how many rows/cols the menu can have
Everything else is given default values, but many fields can be given user-supplied values using other methods
*/

NewMenu::NewMenu(WINDOW* win, int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;

	//we're assuming here for now that null windows will not be passed into this constructor!
	this->win = win;
	
	capacity = rows * cols;

	//by default visible rows/cols will be big enough to display entire menu without scrolling
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


void NewMenu::setDefaults()
{
	itemCount = 0;
	majorOrder = ROW_MAJOR; //default

	topRow = 0; //the top menu item that is visible
	selectedIndex = 0; //default index will be top left item

	maxNameLength = 15; //default is 15

	pad = ' '; //space by default

	mark[0] = '-';//set default mark to ->
	mark[1] = '>';
	markSide = LEFT_MARK;

	colWidth = maxNameLength + 2 + 1; //default is 19 = maxNameLength + 2 (mark length) + 1(divider)
	wrapAround = true; //allow by default
}

void NewMenu::setMajorOrder(bool majorOrder)
{
	this->majorOrder = majorOrder;
}

void NewMenu::setKeypad(bool set)
{
	keypad(win, set);
}

void NewMenu::setMarkSide(bool markSide)
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


void NewMenu::setSelectedIndex(int index)
{
	selectedIndex = index;
}

void NewMenu::setWrapAround(bool wrap)
{
	wrapAround = wrap;
}

/*
setItem will set the item at the element specified. If element has already been set it will be overwritten.
*/
bool NewMenu::setItem(string name, string itemDesc, int element, int crossRefNdx)
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


void NewMenu::draw()
{ 
	//clear the submenu
	//wclear(win);

	//render each item
	if (majorOrder == ROW_MAJOR)
	{
		for (int row = 0; row < rows; row++)
		{
			for (int col = 0; col < cols; col++)
			{
				drawItem(row, col);
			}
		}
	}
	else //majorOrder == COL_MAJOR
	{
		for (int col = 0; col < cols; col++)
		{
			for (int row = 0; row < rows; row++)
			{
				drawItem(row, col);
			}
		}
	}
}



void NewMenu::drawItem(int row, int col)
{
	int element = getElement(row, col);
	char* mark;

	//choose correct mark and attribute if item is highlighted
	if (element == selectedIndex)
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
	if (element == selectedIndex)
	{
	//	wattroff(subWin, A_REVERSE);
	}
}

/*Get element from row and col
rowmajor : row * cols + col
colmajor : col * rows + row*/
int NewMenu::getElement(int row, int col)
{
	if (majorOrder == ROW_MAJOR)
	{
		return row * cols + col;
	}
	else //majorOrder == COL_MAJOR
	{
		return col * rows + row;
	}
}


int NewMenu::driver(int input)
{
	int testIndex = selectedIndex;
	switch (input)
	{
	case REQ_DOWN_ITEM:
		//ignore wraparound for right now and do this for one column menus to start with
		if (majorOrder == ROW_MAJOR)
			testIndex += cols;
		else
			testIndex++;
		break;
	case REQ_UP_ITEM:
		if (majorOrder == ROW_MAJOR)
			testIndex -= cols;
		else
			testIndex--;
		break;
	case REQ_LEFT_ITEM: 
		if (majorOrder == ROW_MAJOR)
			testIndex--;
		else
			testIndex -= rows;
		break;
	case REQ_RIGHT_ITEM: 
		if (majorOrder == ROW_MAJOR)
			testIndex++;
		else
			testIndex += rows;
		break; 
	default: break;
	}
	selectedIndex = testIndex;
	return selectedIndex;
}

int NewMenu::getCrossRefIndex()
{
	int crossRefNdx = crossRef[selectedIndex];
	//-1 in the crossref means it was never set, this also implies that
	//  -1 can never be used as a value we want to return to the user unless it is cancel
	return (crossRefNdx == -1) ? selectedIndex : crossRefNdx;//use crossrefndx only if set
}

NewMenu::~NewMenu()
{
	delete [] items;
	delwin(win);
}
