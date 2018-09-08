#include "Inventory.h"

const std::string blankName = "               "; //15 spaces
GameItem* blankItem;

Inventory::Inventory(unsigned int rows, unsigned int cols)
{
	items.resetItems(rows, cols);

	//create one blank item
	blankItem = new GameItem();
	blankItem->name = blankName;
	//blankItem->x = -1;
	//blankItem->y = -1;
	blankItem->description = "";
	blankItem->cost = 0;
	blankItem->type = GameItemType::BLANK;

	//point all items in menu to point to the same blank item
	for (unsigned int i = 0; i < items.getMaxCapacity(); i++)
	{
		GameItemMenuRecord* gimr = new GameItemMenuRecord();
		gimr->index = i;
		gimr->setItem(blankItem);
		items.setItem(gimr);
	}
	
	items.setWrapAround(false); //disabling wraparounmd 
}


void Inventory::setMaxItems(unsigned int max)
{
	items.setMaxItems(max);
}

bool Inventory::addItem(GameItem* newItem)
{
	bool found = false;
	//search through inventory from beginning 0,0 and search from left right, top to bottom for next blank item
	for (unsigned int i = 0; i < items.getMaxCapacity(); i++)
	{
		GameItemMenuRecord* gimr = (GameItemMenuRecord*)items.AbstractMenu::getItem(i);
		GameItem* item = gimr->getItem();
		if (item == blankItem)
		{
			//assign new Item to this item
			gimr->setItem(newItem);
			itemCount++;
			found = true;
			break;
		}
	}


	return found;
}

GameItem* Inventory::getItemAtIndex(unsigned int index)
{
	GameItemMenuRecord* gimr = (GameItemMenuRecord*)items.AbstractMenu::getItem(index);
	return gimr->getItem();
}

GameItem* Inventory::getItem(unsigned int row, unsigned int col)
{
	GameItemMenuRecord* gimr = (GameItemMenuRecord*)items.getItem(row, col);
	return gimr->getItem();
}

GameItem* Inventory::discardItem(unsigned int index)
{
	//save off the item for returning
	GameItemMenuRecord* gimr = (GameItemMenuRecord*)items.AbstractMenu::getItem(index);

	GameItem* item = gimr->getItem();

	//replace slot in item with blankItem
	gimr->setItem(blankItem);
	itemCount--;

	return item; 
}

void Inventory::swapItems(int index1, int index2)
{
	items.swapItems(index1, index2);
	return;
}
