#include <algorithm>
#include "AbstractMenu.h"

AbstractMenu::AbstractMenu()
{
	//set defaults here
	curItem = nullptr;
}

void AbstractMenu::allocateItems()
{
	/*items = new MenuItem*[capacity];
	for (int i = 0; i < capacity; i++)
	{
		items[i] = NULL;
	}
	itemCount = 0;
*/


}

void AbstractMenu::setMaxCapacity(size_t value)
{
	items.reserve(value); 
}

void AbstractMenu::setMaxItems(unsigned short value)
{
	if (value > items.size())
	{
		setMaxCapacity(value); //need enough memory to allow for max items
	}
	else if (value < items.size())
	{
		//delete vectors on the end

		for (int i = value; i < items.size(); i++) //value is new size and index of element after new size
		{
			delete items[i];
		}
	}

	items.resize(value, nullptr);	
}

/*
set item using the index already set in the input parameter
*/
bool AbstractMenu::setItem(MenuItem* item)
{
	if (item == nullptr || validateIndex(item->index) == false) //item being passed in is valid 
		return false;

	MenuItem* oldItem = items[item->index];

	if (oldItem != nullptr) //no deletion is needed
	{
		//transfer links
		item->link(Dir::UP, oldItem->getLinkedItem(Dir::UP));
		item->link(Dir::DOWN, oldItem->getLinkedItem(Dir::DOWN));
		item->link(Dir::RIGHT, oldItem->getLinkedItem(Dir::RIGHT));
		item->link(Dir::LEFT, oldItem->getLinkedItem(Dir::LEFT));

		delete oldItem;
	}

	item->setMenu(this);
	

	items[item->index] = item;
	if (curItem == nullptr) //set first added item to current item by default
		setCurrentItem(item->index);

	return true;

}

MenuItem* AbstractMenu::getItem(int index)
{
	if (validateIndex(index) == false)
		return nullptr;

	return items[index];
}

bool AbstractMenu::clearItem(int index)
{
	if (validateIndex(index) == false)
		return false;

	delete items[index];

	if (curItem == items[index])
		curItem = nullptr;

	items[index] = nullptr;
	return true;
}

bool AbstractMenu::validateIndex(int index)
{
	return index >= 0 && index < (int)items.size();
}


bool AbstractMenu::setSelected(int index, bool selected)
{
	if (validateIndex(index) == false)
		return false;

	items[index]->selected = selected;
	return true;
}


bool AbstractMenu::setCurrentItem(int index)
{
	if (validateIndex(index) == false) //can't set an out of bounds index
		return false;

	curItem = items[index];

	return true;
}

void AbstractMenu::clearItems()
{
	//TODO use a lambda and for each here
	//std::for_each(items2.begin(), items2.end, ?)

	for (int i = 0; i < (int)items.size(); i++)
	{
		delete items[i];
		items[i] = nullptr;
	}
	
	curItem = nullptr;
}



MenuItem* AbstractMenu::basicMenuDriver(int input, AbstractMenu* m) //static 
{
	MenuItem* item = nullptr;
	int retval = -1;
	switch (input)
	{
	case KEY_DOWN: retval = m->driver(REQ_DOWN_ITEM); break;
	case KEY_UP: retval = m->driver(REQ_UP_ITEM); break;
	case KEY_LEFT: retval = m->driver(REQ_LEFT_ITEM); break;
	case KEY_RIGHT: retval = m->driver(REQ_RIGHT_ITEM); break;
	/*case KEY_PPAGE: m->driver(REQ_SCR_DPAGE); break;
	case KEY_NPAGE: m->driver(REQ_SCR_UPAGE); break;*/
	case '\n':
	case '\r':
	case KEY_ENTER:
		item = m->getCurrentItem();
		break;
	default: break;
	}

	return item;
}