#include <algorithm>
#include "ItemBrowser.h"
#include "DialogWindow.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "ExitCode.h"
#include "GameItem.h"
#include "OwnedItemRecord.h"
#include "TextLabel.h"


void ItemBrowser::setInventory(Inventory* inventoryIn)
{
	inventory = inventoryIn;
	menu.resetItems(inventory->getCapacity(), 1);

	//fill with nullitems
	for (int i = 0; i < inventory->getCapacity(); i++)
	{
		OwnedItemRecord* record = new OwnedItemRecord(nullptr, i);
		bool selectable = false;
		if (i < inventory->getItemCount())
		{
			record->setPossession(inventory->getPossession(i));
			selectable = true;
		}
	
		menu.setItem(record);
		menu.AbstractMenu::getItem(i)->selectable = selectable;
	}

	menu.setCurrentItem(0);
	menu.setWrapAround(false);
	menu.post(true);
}

int ItemBrowser::getItemCount()
{
	return inventory->getItemCount();
}

int ItemBrowser::getItemQuantity(GameItem* item)
{
	return inventory->getItemQuantity(item);
}

void ItemBrowser::setWindow(WINDOW* win)
{
	menu.setWindow(win);
}


void ItemBrowser::processInput()
{
	AbstractMenu::basicMenuDriver(input, &menu);
}


void ItemBrowser::draw()
{
	menu.draw(); 
}


bool ItemBrowser::setCurrentItem(int index)
{
	return menu.setCurrentItem(index);
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

	int startAmt = inventory->getItemCount();
	inventory->alterItemQuantity(currItem->getPossession()->item, -1);

	if (inventory->getItemCount() < startAmt)
	{
		currItem->setPossession(nullptr);
	}
	

	return true;
}


bool ItemBrowser::acquireItem(GameItem* item, int amount) //add item to inventory
{
	int startAmt = inventory->getItemCount();
	Possession* posn = inventory->alterItemQuantity(item, amount);

	if (inventory->getItemCount() > startAmt)
	{
		addPossessionToMenu(posn);
	}

	return true;
}

void ItemBrowser::addPossessionToMenu(Possession* posn)
{
	for(int i = 0; i < inventory->getCapacity(); i++)
	{
		OwnedItemRecord* item = (OwnedItemRecord*)menu.AbstractMenu::getItem(i);
		if (item->getPossession() == nullptr)
		{
			item->setPossession(posn);
			break;
		}
	}
}
