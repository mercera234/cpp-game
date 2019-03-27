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


//void ItemBrowser::setItems(std::vector<Possession*>& items)
//{
//	//inventory = &items; //save reference to items
//
//	bool invEmpty = items.size() == 0;
//
//	int totalItems = invEmpty ? 1 : items.size();
//	menu.resetItems(totalItems, 1);
//
//	int element = 0;
//	for each (Possession* item in items)
//	{
//		menu.setItem(new OwnedItemRecord(item, element++));
//	}
//
//	if (invEmpty)
//	{
//		menu.setItem(new OwnedItemRecord(nullptr, 0)); //set null item 
//		menu.AbstractMenu::getItem(0)->selectable = false;
//	}
//	
//	menu.setCurrentItem(0);
//	menu.post(true);
//}

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
	//decrement possession
	//posn->quantity.alterCurr(-1);

	//if (posn->quantity.getCurr() == 0)
	//{
	//	currItem->setPossession(nullptr);

	//	auto it = std::find(inventory.begin(), inventory.end(), posn);
	//	inventory.erase(it);

	//	delete posn; //this relies on all possessions being created new
	//}

	return true;
}

//std::vector<Possession*>::iterator ItemBrowser::findInventoryItem(GameItem* item)
//{
//	auto it = std::find_if(inventory.begin(), inventory.end(),
//		[item](Possession* posn) {
//		if (posn == nullptr || posn->item == nullptr)
//			return false;
//
//		return posn->item == item;
//	});
//
//	return it;
//}


bool ItemBrowser::acquireItem(GameItem* item, int amount) //add item to inventory
{
	//attempt to find item in inventory first and add to it
	//auto it = findInventoryItem(item);

	//if (it != inventory.end())
	//{
	//	(*it)->quantity.alterCurr(quantity);
	//}
	//else //attempt to add to inventory
	//{
	//	if (capacity == inventory.size())
	//	{
	//		return false; //inventory is full
	//	}

	//	Possession* posn = new Possession;
	//	posn->quantity.setCurr(quantity);
	//	posn->item = item;
	//	inventory.push_back(posn);

	//	addGameItemToMenu(inventory.size() - 1);
	//}
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

//int ItemBrowser::getItemQuantity(GameItem* item)
//{
//	auto it = findInventoryItem(item);
//	if (it == inventory.end())
//		return 0;
//
//	return (*it)->quantity.getCurr();
//}