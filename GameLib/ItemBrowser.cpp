#include "ItemBrowser.h"
#include "DialogWindow.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "GameInput.h"
#include "ExitCode.h"
#include "menu_drivers.h"
#include "GameItem.h"
#include "OwnedItemRecord.h"
#include "TextLabel.h"
#include <algorithm>


ItemBrowser::ItemBrowser()
{ 
	menu.setWrapAround(false); 
}

void ItemBrowser::setWindow(WINDOW* win)
{
	menu.setWindow(win);
}


void ItemBrowser::processInput(int input)
{
	menuDriver(input, &menu);
}


void ItemBrowser::setItems(std::vector<Possession*>& items)
{
	inventoryPtr = &items; //save reference to items

	menu.resetItems(items.size(), 1);

	int element = 0;
	for each (Possession* item in items)
	{
		menu.setItem(new OwnedItemRecord(item, element++));
		if (item->item->type != GameItemType::USABLE)
		{
			menu.AbstractMenu::getItem(element - 1)->selectable = false;
		}
	}
	
	menu.setCurrentItem(0);
	menu.post(true);
}

void ItemBrowser::draw()
{
	refreshList();

	menu.draw();
}

void ItemBrowser::refreshList()
{
	/*int removeCount = 0;
	std::for_each(inventoryPtr->begin(), inventoryPtr->end(), 
		[&removeCount](Possession* posn) {

		if (posn->quantity.getCurr() <= 0)
		{
			posn->item = 
		}
		
	});*/



}

OwnedItemRecord* ItemBrowser::getCurrentItem()
{
	return (OwnedItemRecord*)menu.getCurrentItem();
}

void ItemBrowser::setFocus(bool focusIn)
{
	Controllable::setFocus(focusIn);
	menu.setFocus(focusIn);
}