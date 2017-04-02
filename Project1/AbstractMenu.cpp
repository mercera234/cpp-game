#include "AbstractMenu.h"

void AbstractMenu::allocateItems()
{
	items = new MenuItem*[capacity];
	for (int i = 0; i < capacity; i++)
	{
		items[i] = NULL;
	}
	itemCount = 0;
}

void AbstractMenu::setMarkSide(bool markSide)
{
	this->markSide = markSide;
	mark = markSide == LEFT_MARK ? "->" : "<-";
}

/*
set item using the index already set in the input parameter
*/
void AbstractMenu::setItem(MenuItem* item)
{
	MenuItem* oldItem = items[item->index];
	if (oldItem == NULL)
	{
		itemCount++;
		item->setMenu(this);
	}
	else //replace existing item
	{
		//transfer links
		item->upItem = oldItem->upItem;
		item->downItem = oldItem->downItem;
		item->rightItem = oldItem->rightItem;
		item->leftItem = oldItem->leftItem;
		item->menu = oldItem->menu;

		delete oldItem; //delete previous item
	}

	
	items[item->index] = item;
}

void AbstractMenu::selectItem(int index)
{
	items[index]->selected = true;
}

void AbstractMenu::deSelectItem(int index)
{
	items[index]->selected = false;
}


void AbstractMenu::setCurrentItem(int index)
{
	if (index < 0 || index >= capacity) //can't set an out of bounds index
		return;

	curItem = items[index];
}

MenuItem* AbstractMenu::getCurrentItem()
{
	return curItem;
}

void AbstractMenu::clearItems()
{
	for (int i = 0; i < capacity; i++)
	{
		delete items[i];
		items[i] = NULL;
	}
	
	curItem = NULL;
	setPosition(0, 0);
}
