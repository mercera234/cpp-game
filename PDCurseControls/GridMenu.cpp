#include <algorithm>
#include <assert.h>
#include "GridMenu.h"
#include "curses.h"

GridMenu::GridMenu()
{
	init();
}

GridMenu::GridMenu(WINDOW* win, int rows, int cols)
{
	init();
	//we're assuming here for now that null windows will not be passed into this constructor!
	setWindow(win);

	resetItems(rows, cols);
}


void GridMenu::init()
{
	cursor = defaultCursor;

	colSepLen = 1;

	setItemWidth(16); //TODO it would be better if this could change dynamically 

	rowSepLen = 0; //most menus will have no gap between rows
	setItemHeight(1);
}


/*
Set the capacity using the dimensions passed in. 
Any defaults already set will not be changed.
*/
void GridMenu::resetItems(int rows, int cols)
{
	posted = false;
	clearItems();

	menuRows = rows;
	menuCols = cols;

	setMaxItems(menuRows * menuCols);

	setDimensions(menuRows * rowHeight, menuCols * colWidth);
}



void GridMenu::setItemHeight(unsigned short height)
{
	itemHeight = height;
	setRowHeight(); //recalculate when item height changes
}

void GridMenu::setRowSepLength(unsigned short length)
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
	for (int i = 0; i < (int)items.size(); i++)
	{
		if (items[i] == nullptr)
			continue;

		items[i]->posY *= rowHeight;
	}
}

void GridMenu::setItemWidth(unsigned short width)
{
	itemWidth = width;
	setColWidth(); //recalculate when item width changes
}

void GridMenu::setColSepLength(unsigned short length)
{
	colSepLen = length;
	setColWidth(); //recalculate when separator length changes
}

void GridMenu::setColWidth()
{
	colWidth = itemWidth + (unsigned short)cursor.length() + colSepLen;

	//total columns is the width of the columns times the number of menu columns + length of the column separators between columns
	totalCols = menuCols * colWidth - 1; //subtract 1 because we don't need separator at very end of row

	visibleMenuCols = visibleCols / colWidth;
}


void GridMenu::setWrapAround(bool wrap)
{
	wrapAround = wrap;
	if(posted)
		wrapLinks(); 
}


bool GridMenu::post(bool post)
{
	if (post) //post menu
	{
		posted = validateForPost();
	}
	else
		posted = false;

	if (posted)
	{
		linkItems();
	}
	return posted;
}

bool GridMenu::validateForPost()
{
	if (items.size() == 0) //don't post an empty menu
		return false;

	//std::for_each(items.begin(), items.end(), )
	for each (MenuItem* item in items)
	{
		if (item == nullptr)
			return false;
	}
	return true;
}

void GridMenu::linkItems()
{
	//link inner links

	//iterate through rows, linking all horizontal links
	for (int row = 0; row < menuRows; row++)
	{
		for (int col = 0; col < menuCols - 1; col++) //the last column will already be linked, hence - 1
		{
			MenuItem* item = getItem(row, col);
			item->link(Dir::RIGHT, getItem(row, col + 1));
		}
	}

	//iterate through cols
	for (int col = 0; col < menuCols; col++)
	{
		for (int row = 0; row < menuRows - 1; row++) //the last row will already be linked, hence - 1
		{
			MenuItem* item = getItem(row, col);
			item->link(Dir::DOWN, getItem(row + 1, col));
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
		MenuItem* item = getItem(0, col);  
		if(wrapAround)
			item->link(Dir::UP, getItem(menuRows - 1, col));
		else
			item->unlink(Dir::UP);
	}

	//setup horizontal outer pointers
	for (int row = 0; row < menuRows; row++)
	{
		MenuItem* item = getItem(row, 0);
		if (wrapAround)
			item->link(Dir::LEFT, getItem(row, menuCols - 1));
		else
			item->unlink(Dir::LEFT);
	}
}


/*Get element from row and col
rowmajor : row * menuCols + col
colmajor : col * menuRows + row*/
int GridMenu::getElement(int row, int col)
{
	return row * menuCols + col;
}


void GridMenu::draw()
{
	//clear the menu
	wclear(win);
	assert(posted);

	if (!showing) return;

	
	wbkgd(win, ' ' | colorPair);

	wattron(win, colorPair);
	if (focus)
		wattron(win, A_BOLD);
	else
		wattroff(win, A_BOLD);


	//render each item
	for (int i = 0; i < (int)items.size(); i++)
	{
		if (items[i] == NULL || items[i]->hidden) //don't draw null or hidden items
			continue;

		int posX = items[i]->posX;
		int posY = items[i]->posY;

		if (posX < ulX || posY < ulY || posX >= ulX + visibleCols || posY >= ulY + visibleRows)
		{
			continue; //don't draw item that is out of bounds of the view
		}

		items[i]->draw();
	}
	wnoutrefresh(win);
}


void GridMenu::dirDriver(int input)
{
	MenuItem* destination = NULL;
	int posX;
	int posY;
	switch (input)
	{
		case REQ_DOWN_ITEM:
			destination = curItem->getLinkedItem(Dir::DOWN);
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
			destination = curItem->getLinkedItem(Dir::UP);
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
			destination = curItem->getLinkedItem(Dir::RIGHT);
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
			destination = curItem->getLinkedItem(Dir::LEFT);
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
				setInput(REQ_DOWN_ITEM);
				processInput();
			}
			break;
		case REQ_SCR_UPAGE:
			for (int i = 0; i < visibleRows; i++)
			{
				setInput(REQ_UP_ITEM);
				processInput();
			}
			break;
		case REQ_FIRST_ITEM:
			curItem = items[0];
			setPosition(0, 0);
			break;
		case REQ_LAST_ITEM:
			curItem = items[items.size() - 1];
			//setPosition((menuRows - 1) * rowHeight, (menuCols - 1) * colWidth);
			posY = totalRows - visibleRows;
			if (posY < 0)
				posY = 0;

			posX = totalCols - visibleCols;
			if (posX < 0)
				posX = 0;

			setPosition(posY, posX);
			break;
	}

}


void GridMenu::mouseDriver()
{
	MEVENT event;
	nc_getmouse(&event);

	int pY = event.y;
	int	pX = event.x;
	wmouse_trafo(win, &pY, &pX, false);

	//translate to gridmenu coordinates
	int row = pY;
	int col = pX / colWidth;

	setCurrentItem(getElement(row, col));
}

void GridMenu::processInput()
{
	if (!posted) //only accept input to a posted menu
	{
		exitCode = ExitCode::NOT_HANDLED;
		return;
	}
		

	switch (input)
	{
	case REQ_DOWN_ITEM:
	case REQ_UP_ITEM:
	case REQ_RIGHT_ITEM:
	case REQ_LEFT_ITEM:
	case REQ_SCR_DPAGE:
	case REQ_SCR_UPAGE:
	case REQ_FIRST_ITEM:
	case REQ_LAST_ITEM:
		dirDriver(input);
		break;
	case KEY_MOUSE:
		mouseDriver();
		break;
		//case REQ_TOGGLE_ITEM:
		//	if (items[currentIndex]->selectable == false)
		//		break;

	default: break;
	}
}


void GridMenu::setItem(MenuItem* item)
{
	int element = item->index;
	int menuRow = (element / menuCols); 
	int menuCol = (element % menuCols);
	
	setItem(item, menuRow, menuCol);
}

//here we favor the menu row and col over the items index
void GridMenu::setItem(MenuItem* item, unsigned short menuRow, unsigned short menuCol)
{
	int y = menuRow * rowHeight;
	int x = menuCol * colWidth;

	item->setPosition(y, x);
	item->index = getElement(menuRow, menuCol); //change index to match menu row and col
	AbstractMenu::setItem(item);
}

MenuItem* GridMenu::getItem(int menuRow, int menuCol)
{
	return AbstractMenu::getItem(getElement(menuRow, menuCol));
}


GridMenu::~GridMenu()
{
	clearItems();
}

void GridMenu::saveLinkIndices(MenuItem* item, std::vector<short>& indices)
{
	indices.resize(4);

	int i = 0;
	for each (Dir dir in allDirs)
	{
		MenuItem* linkedItem = item->getLinkedItem(dir);
		indices[i++] = linkedItem == nullptr ? -1 : linkedItem->index;
	}
}

void GridMenu::restoreLinkIndices(MenuItem* item, const std::vector<short>& indices)
{
	int i = 0;
	for each (Dir dir in allDirs)
	{
		item->link(dir, AbstractMenu::getItem(indices[i++]));
	}
}


bool GridMenu::swapItems(int index1, int index2)
{
	if ((validateIndex(index1) && validateIndex(index2)) == false)
		return false;

	MenuItem* item1 = items[index1];
	MenuItem* item2 = items[index2];

	//save old links
	std::vector<short> oneItems;
	saveLinkIndices(item1, oneItems);

	std::vector<short> twoItems;
	saveLinkIndices(item2, twoItems);

	//break links
	item1->clearLinks();
	item2->clearLinks();

	//perform swap actions
	std::swap(items[index1], items[index2]); //swap the bulk of the data (only swaps within the items vector)

											 //retrieve items again after swap
	item1 = items[index1];
	item2 = items[index2];

	//swap indices and positions
	std::swap(item1->index, item2->index); //swap the indices
	std::swap(item1->posY, item2->posY); //swap the positions
	std::swap(item1->posX, item2->posX); //swap the positions

	//restore links
	restoreLinkIndices(item1, oneItems);
	restoreLinkIndices(item2, twoItems);

	return true;
}

