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

	bool invEmpty = items.size() == 0;

	int totalItems = invEmpty ? 1 : items.size();
	menu.resetItems(totalItems, 1);

	int element = 0;
	for each (Possession* item in items)
	{
		menu.setItem(new OwnedItemRecord(item, element++));
		/*
		
		
		
		if (item->item->type != GameItemType::USABLE)
		{
			menu.AbstractMenu::getItem(element - 1)->selectable = false;
		}
	
	
	*/
	}

	if (invEmpty)
	{
		menu.setItem(new OwnedItemRecord(nullptr, 0)); //set null item 
		menu.AbstractMenu::getItem(0)->selectable = false;
	}
	
	menu.setCurrentItem(0);
	menu.post(true);
}

void ItemBrowser::draw()
{
	menu.draw(); 
}

void ItemBrowser::runFilter(void(*filter)(OwnedItemRecord* record))
{
	for (int i = 0; i < (int)menu.getMaxItems(); i++)
	{
		OwnedItemRecord* record = (OwnedItemRecord*)menu.AbstractMenu::getItem(i);
		filter(record);
	}
}


GameItem* ItemBrowser::getCurrentItem()
{
	OwnedItemRecord* record = (OwnedItemRecord*)menu.getCurrentItem();
	if (record == nullptr || record->getPossession() == nullptr)
		return nullptr;

	return record->getPossession()->item;
}

void ItemBrowser::setFocus(bool focusIn)
{
	Controllable::setFocus(focusIn);
	menu.setFocus(focusIn);
}


bool ItemBrowser::decrementItem()
{
	//get current possession
	OwnedItemRecord* currItem = (OwnedItemRecord*)menu.getCurrentItem();
	if (currItem == nullptr)
		return false;

	Possession* posn = currItem->getPossession();
	if (posn == nullptr || posn->quantity.getCurr() == 0)
	{
		return false; //can't decrement
	}

	//decrement possession
	posn->quantity.alterCurr(-1);

	if (posn->quantity.getCurr() == 0)
	{
		currItem->setPossession(nullptr);

		auto it = std::find(inventoryPtr->begin(), inventoryPtr->end(), posn);
		inventoryPtr->erase(it);

		delete posn; //this relies on all possessions being created new
	}

	return true;
}