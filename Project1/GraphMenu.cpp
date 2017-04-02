#include "GraphMenu.h"

GraphMenu::GraphMenu(WINDOW* win, int capacity)
{
	setWindow(win);

	this->capacity = capacity; //will grow as items are added

	allocateItems();

	setMarkSide(LEFT_MARK);
}


void GraphMenu::draw() 
{
	wclear(win);

	//render each item
	for (int i = 0; i < capacity; i++)
	{
		if (items[i] == NULL || items[i]->hidden) //only draw non-null items //hidden items are not drawn
			continue;
			
		items[i]->draw(win);

		//draw mark if 
		//if (curItem == items[i] || items[i]->selected == true) //draw mark
		//{
		//	//only doing left mark for right now!
		//	mvwaddstr(win, (items[i]->posY - ulY), (items[i]->posX - ulX - mark.length()), mark.c_str()); //print mark
		//}

	}
			
	wnoutrefresh(win);
}

/*
handle directional requests in a Graph Menu
the way this works, we cannot move our cursor to null items
This may be okay, because if we wanted a null item we could replace it with a blank item
*/
void GraphMenu::dirDriver(int input)
{
	MenuItem* destination = NULL;
	switch (input)
	{
	case REQ_DOWN_ITEM: destination = curItem->downItem; break;
	case REQ_UP_ITEM: destination = curItem->upItem; break;
	case REQ_RIGHT_ITEM: destination = curItem->rightItem; break;
	case REQ_LEFT_ITEM: destination = curItem->leftItem; break;
	}

	if (destination != NULL)
		curItem = destination;

}

int GraphMenu::driver(int input)
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
		/*if (items[currentIndex]->selectable == false)
			break;*/
		break;

	default: break;
	}

	return 0; //not sure what else to return here
}