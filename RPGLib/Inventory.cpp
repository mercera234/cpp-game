#include <algorithm>
#include <assert.h>
#include "Inventory.h"
#include "DialogWindow.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "ExitCode.h"
#include "GameItem.h"
#include "OwnedItemRecord.h"
#include "TextLabel.h"

Inventory::Inventory()
{
	itemList.reserve(capacity);
	//menu.resetItems(capacity, 1); //using 10 as a starter amount here

	////fill with nullitems
	//for (int i = 0; i < capacity; i++)
	//{
	//	menu.setItem(new OwnedItemRecord(nullptr, i));
	//	menu.AbstractMenu::getItem(i)->selectable = false;
	//}

	//menu.setCurrentItem(0);
	//menu.setWrapAround(false);
	//menu.post(true);
}

//void Inventory::draw()
//{
//	menu.draw();
//}
//
//void Inventory::setFocus(bool focusIn)
//{
//	Controllable::setFocus(focusIn);
//	menu.setFocus(focusIn);
//}
//
//void Inventory::setWindow(WINDOW* win)
//{
//	menu.setWindow(win);
//}
//
//
//void Inventory::processInput()
//{
//	AbstractMenu::basicMenuDriver(input, &menu);
//}



//bool Inventory::setCurrentItem(int index)
//{
//	return menu.setCurrentItem(index);
//}
//
//GameItem* Inventory::getCurrentItem()
//{
//	OwnedItemRecord* record = (OwnedItemRecord*)menu.getCurrentItem();
//	if (record == nullptr || record->getPossession() == nullptr)
//		return nullptr;
//
//	return record->getPossession()->item;
//}

std::vector<Possession*>::iterator Inventory::findItem(GameItem* item)
{
	auto it = std::find_if(itemList.begin(), itemList.end(),
		[item](Possession* posn) {
		if (posn == nullptr || posn->item == nullptr)
			return false;

		return posn->item == item;
	});

	return it;
}

//bool Inventory::decrementItem(GameItem* item, int quantity)
//{
//	//get current possession
//	//OwnedItemRecord* currItem = (OwnedItemRecord*)menu.getCurrentItem();
//	//if (currItem == nullptr)
//	//	return false;
//
//	//Possession* posn = currItem->getPossession();
//	//if (posn == nullptr || posn->quantity.getCurr() == 0)
//	//{
//	//	return false; //can't decrement
//	//}
//
//	auto it = findInventoryItem(item); //find item first so we can decrement from it
//
//	//decrement possession
//	posn->quantity.alterCurr(-1);
//
//	if (posn->quantity.getCurr() == 0)
//	{
//		currItem->setPossession(nullptr);
//
//		auto it = std::find(inventory.begin(), inventory.end(), posn);
//		inventory.erase(it);
//
//		delete posn; //this relies on all possessions being created new
//	}
//
//	return true;
//}


bool Inventory::incrementItemQuantity(BoundInt& invItemQty, unsigned int amount)
{
	if (invItemQty.getCurr() + (int)amount > invItemQty.getMax())
		return false;

	invItemQty.alterCurr(amount);
	return true;
}

template <typename iterator>
bool Inventory::decrementItemQuantity(iterator it, int quantity)
{
	Possession* p = *it;
	p->quantity.alterCurr(quantity);

	if (p->quantity.isDrained())
	{
		//remove from vector
		itemList.erase(it);

		delete p; //this relies on all possessions being created new
	}

	return true;
}

Possession* Inventory::alterItemQuantity(GameItem* item, int amount) //add item to inventory
{
	assert(amount != 0);// && amount <= 99);
	if (item == nullptr)
		return false;

	//attempt to find item in inventory first
	auto it = findItem(item);

	Possession* posnAltered = nullptr;

	if (it != itemList.end()) //item was found
	{		
		Possession* posnFound = *it;
		bool altered = amount > 0 ? 
			incrementItemQuantity(posnFound->quantity, amount) :
			decrementItemQuantity(it, amount);		

		if (altered)
			posnAltered = posnFound;
	}
	else if (amount > 0 && capacity != itemList.size()) //item not found, but we're adding, and have room to add
	{
		posnAltered = new Possession;
		posnAltered->quantity.setCurr(amount);
		posnAltered->item = item;
		itemList.push_back(posnAltered);

		//addGameItemToMenu(inventory.size() - 1);
	}
	
	return posnAltered;
}

//void Inventory::addGameItemToMenu(int index)
//{
//	for (int i = 0; i < capacity; i++)
//	{
//		OwnedItemRecord* item = (OwnedItemRecord*)menu.AbstractMenu::getItem(i);
//		if (item->getPossession() == nullptr)
//		{
//			item->setPossession(inventory[index]);
//			break;
//		}
//	}
//}

int Inventory::getItemQuantity(GameItem* item)
{
	auto it = findItem(item);
	if (it == itemList.end())
		return 0;

	return (*it)->quantity.getCurr();
}

Possession* Inventory::getPossession(int index)
{
	return itemList[index];
}