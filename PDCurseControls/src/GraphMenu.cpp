#include "GraphMenu.h"

GraphMenu::GraphMenu(WINDOW* win, int capacity)
{
	setWindow(win);

	setMaxItems(capacity);
	//this->capacity = capacity; //will grow as items are added

	//allocateItems();

	cursor = defaultCursor;
//	setMarkSide(LEFT_MARK);
}


void GraphMenu::draw() 
{
	wclear(win);

	if (focus)
		wattron(win, A_BOLD);
	else
		wattroff(win, A_BOLD);

	//render each item
	for (int i = 0; i < (int)items.size(); i++)
	{
		if (items[i] == NULL || items[i]->hidden) //only draw non-null items //hidden items are not drawn
			continue;
			
		items[i]->draw();

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
	case REQ_DOWN_ITEM: destination = curItem->getLinkedItem(Dir::DOWN); break;
	case REQ_UP_ITEM: destination = curItem->getLinkedItem(Dir::UP); break;
	case REQ_RIGHT_ITEM: destination = curItem->getLinkedItem(Dir::RIGHT); break;
	case REQ_LEFT_ITEM: destination = curItem->getLinkedItem(Dir::LEFT); break;
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