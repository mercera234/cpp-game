#pragma once
#include "GridMenu.h"
#include "GameItemMenuRecord.h"
#include "GameItem.h"

//this class may be useless in favor of the GridMenu
class Inventory
{
private:
	GridMenu items;

	/*the count of non-blank items (actual items).
	Quantities of individual items are not considered.*/
	size_t itemCount = 0; 
	
public:
	Inventory(unsigned int rows, unsigned int cols);


	//getters/setters
	size_t getSize() { return items.getMaxItems(); }
	void setMaxItems(unsigned int max);
	unsigned int getItemCount() { return itemCount; }
	GridMenu* getGridMenu() { GridMenu* ptr = &items; return ptr; }

	/*
	Add item to first available slot. When setting up an inventory all slots will be prefilled with blank items.
	*/
	bool addItem(GameItem* newItem);
	GameItem* getItemAtIndex(unsigned int index);
	GameItem* getItem(unsigned int row, unsigned int col);

	/*
	Remove item from inventory. It is returned in case we need to do anything with it.
	*/
	GameItem* discardItem(unsigned int index);
	void swapItems(int index1, int index2);


};

