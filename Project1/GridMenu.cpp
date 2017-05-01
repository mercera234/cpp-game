#include "GridMenu.h"
#include "curses.h"

/*
Menu constructor
A user can initially specify only how many menuRows/menuCols the menu can have
Everything else is given default values, but many fields can be given user-supplied values using other methods
*/
GridMenu::GridMenu(WINDOW* win, int rows, int cols)
{
	//we're assuming here for now that null windows will not be passed into this constructor!
	setWindow(win);

	resetItems(rows, cols);
	
	setDefaults();	
}


/*
Set the capacity using the dimensions passed in. 
Any defaults already set will not be changed.
*/
void GridMenu::resetItems(int rows, int cols)
{
	posted = false;
	clearItems();

	if(items != NULL)
		delete[] items;

	menuRows = rows;
	menuCols = cols;

	capacity = menuRows * menuCols;

	allocateItems();

	clearItems(); //clear out any garbage data
}



void GridMenu::setItemHeight(int height)
{
	itemHeight = height;
	setRowHeight(); //recalculate when item height changes
}

void GridMenu::setRowSepLength(int length)
{
	rowSepLen = length;
	setRowHeight(); //recalculate when separator length changes
}

void GridMenu::setRowHeight()
{
	rowHeight = itemHeight + rowSepLen;

	totalRows = menuRows * rowHeight - rowSepLen; //subtract the separator off the last row

	visibleMenuRows = visibleRows / rowHeight;

	//update the y coordinate of each item
	for (int i = 0; i < capacity; i++)
	{
		if (items[i] == NULL)
			continue;

		items[i]->posY *= rowHeight;
	}
}

void GridMenu::setItemWidth(int width)
{
	itemWidth = width;
	setColWidth(); //recalculate when item width changes
}

void GridMenu::setColSepLength(int length)
{
	colSepLen = length;
	setColWidth(); //recalculate when separator length changes
}

void GridMenu::setColWidth()
{
	colWidth = itemWidth + mark.length() + colSepLen;

	//total columns is the width of the columns times the number of menu columns + length of the column separators between columns
	totalCols = menuCols * colWidth - 1; //subtract 1 because we don't need separator at very end of row

	visibleMenuCols = visibleCols / colWidth;
}


void GridMenu::setDefaults()
{
	majorOrder = ROW_MAJOR; //default
	focusable = true; //all menus accept key input
	showing = true; //shows by default

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

void GridMenu::setMajorOrder(bool majorOrder)
{
	this->majorOrder = majorOrder;
}


void GridMenu::setMarkSide(bool markSide)
{
	this->markSide = markSide;
	mark = markSide == LEFT_MARK ? "->" : "<-";
}


void GridMenu::setSelectedIndex(int index)
{
	if (index < 0 || index >= capacity) //can't set an out of bounds index
		return;

	items[currentIndex]->selected = false; //unselect current item
	currentIndex = index;
	items[currentIndex]->selected = true;
}


void GridMenu::setWrapAround(bool wrap)
{
	wrapAround = wrap;
	if(posted)
		wrapLinks(); 
}

/*
Post the menu. This method is important because it links up the items internally after all items have been filled.
*/
bool GridMenu::post(bool post)
{
	if (itemCount != capacity)
		return false; //cannot post an unfilled grid menu

	posted = post;
	if (posted)
	{
		linkItems();
		setCurrentItem(0); //default will always be the first item
	}
	return true;
}

void GridMenu::linkItems()
{
	//link inner links

	//iterate through rows
	for (int row = 0; row < menuRows; row++)
	{
		for (int col = 0; col < menuCols - 1; col++) //the last column will already be linked, hence - 1
		{
			int element = row * menuCols + col;
			MenuItem* item = items[element];
			item->link(true, RIGHT_LINK, items[element + 1]);
		}
	}

	//iterate through cols
	for (int col = 0; col < menuCols; col++)
	{
		for (int row = 0; row < menuRows - 1; row++) //the last row will already be linked, hence - 1
		{
			int element = row * menuCols + col;
			MenuItem* item = items[element];
			item->link(true, DOWN_LINK, items[element + menuCols]);
		}
	}

	//setup outer pointers
	wrapLinks();
}

void GridMenu::wrapLinks()
{
	//setup vertical outer pointers
	for (int col = 0; col < menuCols; col++) 
	{
		MenuItem* item = items[col];
		int bottomElement = col + capacity - menuCols;
		item->link(wrapAround, UP_LINK, items[bottomElement]);
	}

	//setup horizontal outer pointers
	for (int row = 0; row < menuRows; row++)
	{
		int element = row * menuCols; //col is always 0 here
		MenuItem* item = items[element];
		int rightElement = element + menuCols - 1;
		item->link(wrapAround, LEFT_LINK, items[rightElement]);
	}
}


void GridMenu::drawMenu()
{ 
	//clear the submenu
	wclear(win);
	wbkgd(win, ' ' | colorPair);

	wattron(win, colorPair);
	//render each item

	for (int i = 0; i < capacity; i++)
	{
		if (items[i] != NULL && items[i]->hidden) //hidden items are not drawn
			continue;
		
		items[i]->draw(win);
		//draw mark if 
		//if (curItem == items[i] || items[i]->selected == true) //draw mark
		//{
		//	//only doing left mark for right now!
		//	//mvwaddstr(win, (items[i]->yPos - ulY), (items[i]->xPos - ulX - mark.length()), mark.c_str()); //print mark
		//	mvwaddstr(win, (items[i]->posY - ulY), (items[i]->posX - ulX), mark.c_str()); //print mark
		//}

		//if (items[i] != NULL) //only draw non-null items
		//{
		//	if (markSide == LEFT_MARK)
		//	{
		//		items[i]->draw(win, ulY, ulX + mark.length());
		//	}
		//	
		//}
			

	}

	//if (majorOrder == ROW_MAJOR)
	//{
	//	for (int row = ulY; row < ulY + visibleMenuRows; row++)
	//	{
	//		for (int col = ulX; col < ulX + visibleMenuCols; col++)
	//		{
	//			/*int menuRow = row * rowHeight;
	//			int menuCol = col * colWidth;*/

	//		//	drawItem(menuRow, menuCol);
	//			drawItem(row, col);
	//		}
	//	}
	//}
	//else //majorOrder == COL_MAJOR
	//{
	//	for (int col = 0; col < menuCols; col++)
	//	{
	//		for (int row = 0; row < menuRows; row++)
	//		{
	//			drawItem(row, col);
	//		}
	//	}
	//}
}


void GridMenu::drawItem(int row, int col)
{
	//int element = getElement(row, col);
	//if (element >= capacity) //can't draw a null item
	//	return;

	//bool selected = true;
	//if (items[element] == NULL || items[element]->selected == false)
	//	selected = false;

	////print mark and name in correct order
	//if (markSide == LEFT_MARK)
	//{
	//	if(curItem == items[element] || selected)
	//		mvwaddstr(win, (row - ulY) * rowHeight, (col - ulX) * colWidth, mark.c_str()); //print mark

	//	if(items[element] != NULL)
	//		items[element]->draw(win, ulY * rowHeight, ulX - mark.length());
	//}
	//else //markSide == RIGHT_MARK
	//{
	//	if (items[element] != NULL)
	//		items[element]->draw(win, ulY, ulX);
	//		
	//	if (curItem == items[element] || selected)
	//		mvwaddstr(win, (row - ulY) * rowHeight, (col - ulX) * colWidth + itemWidth, mark.c_str()); //print mark
	//}
}

/*Get element from row and col
rowmajor : row * menuCols + col
colmajor : col * menuRows + row*/
int GridMenu::getElement(int row, int col)
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


void GridMenu::draw()
{
	//clear the submenu
	wclear(win);

	if (!showing) return;

	
	wbkgd(win, ' ' | colorPair);

	wattron(win, colorPair);
	if (focus)
		wattron(win, A_BOLD);
	else
		wattroff(win, A_BOLD);


	//render each item
	for (int i = 0; i < capacity; i++)
	{
		if (items[i] == NULL || items[i]->hidden) //don't draw null or hidden items
			continue;

		int posX = items[i]->posX;
		int posY = items[i]->posY;

		if (posX < ulX || posY < ulY || posX >= ulX + visibleCols || posY >= ulY + visibleRows)
		{
			continue; //don't draw item that is out of bounds of the view
		}

		items[i]->draw(win);
	}
	wnoutrefresh(win);
}

//return true if wrap condition is met for the specified direction
//the nav req is the last movement that occurred that brought the user to the current index
//bool GridMenu::wrapOccurred(int navReq, int index)
//{
//	bool wrapOccurred = false;
//	switch (navReq)
//	{
//	case REQ_DOWN_ITEM: // down at bottom of menu it should wrap around to top of same column
//		wrapOccurred = index >= capacity; break;
//	case REQ_UP_ITEM: // up at top of menu it should wrap around to bottom of same column
//		wrapOccurred = index < 0; break;
//	case REQ_RIGHT_ITEM: //right at right edge of menu should wrap to first item of same row
//		wrapOccurred = index % menuCols == 0; break;
//	case REQ_LEFT_ITEM: //left at left edge of menu should wrap to last item of same row
//		wrapOccurred = (index + 1) % menuCols == 0; break;
//	}
//
//	return wrapOccurred;
//}
//
///*
//process all directional navigation requests
//(only handles requests in row major format currently!)
//*/
//void GridMenu::dirDriver2(int input)
//{
//	int index = currentIndex;
//	int navIncr = 0;
//	int wrapOffset = 0;
//	int* axis = 0;
//	int axisPos = 0;
//	int visualOffset = 0;
//	int visibleDimension;
//	switch (input)
//	{
//	case REQ_DOWN_ITEM:
//		navIncr = menuCols;
//		axis = &ulY;
//		wrapOffset = -capacity;
//		axisPos = 0;
//		visibleDimension = visibleMenuRows;
//		visualOffset = 1 - visibleDimension;
//		break;
//	case REQ_UP_ITEM:
//		navIncr = -menuCols;
//		axis = &ulY;
//		wrapOffset = capacity;
//		visibleDimension = visibleMenuRows;
//		axisPos = menuRows - visibleDimension;
//		visualOffset = 0;
//		break;
//	case REQ_RIGHT_ITEM:
//		navIncr = 1;
//		axis = &ulX;
//		wrapOffset = -menuCols;
//		axisPos = 0;
//		visibleDimension = visibleMenuCols;
//		visualOffset = 1 - visibleDimension;
//		break;
//	case REQ_LEFT_ITEM:
//		navIncr = -1;
//		axis = &ulX;
//		wrapOffset = menuCols;
//		visibleDimension = visibleMenuCols;
//		axisPos = menuCols - visibleDimension;
//		visualOffset = 0;
//		break;
//	
//	}
//
//	//perform movement
//	index += navIncr;
//	if(wrapOccurred(input, index)) //hit edge of menu
//	{
//		if(wrapAround)
//		{
//			index += wrapOffset;
//			*axis = axisPos;
//		}
//		else //no wrapping, reverse the movement
//		{
//			index -= navIncr;
//		}
//	}
//
//	//get current row or col
//	int currAxis = axis == &ulY ? index / menuCols : index % menuCols;
//
//	/*if current row or col is either less than the upper left corner of the control 
//	or beyond the visual portion of the control*/
//	if(currAxis < *axis || currAxis >= *axis + visibleDimension)
//	{
//		*axis = currAxis + visualOffset;
//	}
//
//	setCurrentItem(index);
//	setSelectedIndex(index);
//}


//void GridMenu::dirDriver3(int input)
//{
//	MenuItem* destination = NULL;
//	int visibleDimension;
//	int* axis = 0;
//	int visualOffset = 0;
//	int oldIndex = curItem->index;
//	bool wrapDownRight = false;
//	switch (input)
//	{
//	case REQ_DOWN_ITEM: 
//		destination = curItem->downItem; 
//		axis = &ulY;
//		visibleDimension = visibleMenuRows;
//		visualOffset = 1 - visibleDimension;
//		wrapDownRight = true;
//		break;
//	case REQ_UP_ITEM: 
//		destination = curItem->upItem; 
//		axis = &ulY;
//		visibleDimension = visibleMenuRows;
//		visualOffset = 0;
//		break;
//	case REQ_RIGHT_ITEM: 
//		destination = curItem->rightItem; 
//		axis = &ulX;
//		visibleDimension = visibleMenuCols;
//		visualOffset = 1 - visibleDimension;
//		wrapDownRight = true;
//		break;
//	case REQ_LEFT_ITEM: 
//		destination = curItem->leftItem; 
//		axis = &ulX;
//		visibleDimension = visibleMenuCols;
//		visualOffset = 0;
//		break;
//	}
//
//	if (destination != NULL)
//		curItem = destination;
//
//	if ((curItem->index < oldIndex && wrapDownRight) || (curItem->index > oldIndex && !wrapDownRight ))
//	{
//
//	}
//
//	int currAxis = axis == &ulY ? curItem->index / menuCols : curItem->index % menuCols;
//
//	/*if current row or col is either less than the upper left corner of the control
//	//	or beyond the visual portion of the control*/
//	if (currAxis < *axis || currAxis >= *axis + visibleDimension)
//	{
//		*axis = currAxis + visualOffset;
//	}
//}

void GridMenu::dirDriver(int input)
{
	MenuItem* destination = NULL;
	int posX;
	int posY;
	switch (input)
	{
		case REQ_DOWN_ITEM:
			destination = curItem->downItem;
			if (destination != NULL)
				curItem = destination;

			posY = curItem->posY;

			//if (posX < ulX || posY < ulY || posX >= ulX + visibleCols || posY >= ulY + visibleRows)
			if (posY >= ulY + visibleRows)
			{
				shift(rowHeight, 0); //slight bug when wrapping around for row heights greater than 1!
			}
			else if (posY < ulY) //wrap around occurred
			{
				setPosition(0, ulX);
			}

			break;
		case REQ_UP_ITEM:
			destination = curItem->upItem;
			if (destination != NULL)
				curItem = destination;

			posY = curItem->posY;
			if (posY < ulY)
			{
				shift(-rowHeight, 0);
			}
			else if (posY >= ulY + visibleRows)
			{
				setPosition(totalRows - visibleRows, ulX);
			}

			break;
		case REQ_RIGHT_ITEM:
			destination = curItem->rightItem;
			if (destination != NULL)
				curItem = destination;

			posX = curItem->posX;
		
			if (posX >= ulX + visibleCols)
			{
				shift(0, colWidth); //slight bug when wrapping around for row heights greater than 1!
			}
			else if (posX < ulX) //wrap around occurred
			{
				setPosition(ulY, 0);
			}

			break;
		case REQ_LEFT_ITEM:
			destination = curItem->leftItem;
			if (destination != NULL)
				curItem = destination;

			posX = curItem->posX;
			if (posX < ulX)
			{
				shift(0, -colWidth);
			}
			else if (posX >= ulX + visibleCols)
			{
				setPosition(ulY, totalCols - visibleCols);
			}

			break;
		case REQ_SCR_DPAGE: //not sure if this is the most efficient way to page up and down but it was quick to implement
			for (int i = 0; i < visibleRows; i++)
			{
				driver(REQ_DOWN_ITEM);
			}
			break;
		case REQ_SCR_UPAGE:
			for (int i = 0; i < visibleRows; i++)
			{
				driver(REQ_UP_ITEM);
			}
			break;
	}

}

void GridMenu::selectItem(int index)
{
	items[index]->selected = true;
}

int GridMenu::driver(int input)
{
	if (!posted) //only accept input to a posted menu
		return -1;

	switch (input)
	{
	case REQ_DOWN_ITEM:
	case REQ_UP_ITEM:
	case REQ_RIGHT_ITEM:
	case REQ_LEFT_ITEM: 
	case REQ_SCR_DPAGE:
	case REQ_SCR_UPAGE:
		dirDriver(input);
		break;
	case REQ_TOGGLE_ITEM:
		if (items[currentIndex]->selectable == false)
			break;
		break;

	default: break;
	}

	return currentIndex;
}

void GridMenu::setItem(MenuItem* item)
{
	AbstractMenu::setItem(item);

	int element = item->index;
	int menuRow = (element / menuCols); 
	int menuCol = (element % menuCols);
	
	int y = menuRow * rowHeight;
	int x = menuCol * colWidth;
	
	item->setPosition(y, x);
}


MenuItem* GridMenu::getItem(int y, int x)
{
	int row = y;
	int col = x / 3;
	int element = row * menuCols + col;
	return items[element];
}


void GridMenu::disableItem(int y, int x)
{
	MenuItem* mi = getItem(y, x);
	mi->selectable = false;
}

/*
Remove all items from menu, but do not destroy menu itself. Capacity is unaltered
*/

GridMenu::~GridMenu()
{
	clearItems();
	delete[] items;
	delwin(win);
}
