#pragma once
#include "GridMenu.h"
#include "GameItem.h"
#include "OwnedItemRecord.h"

class ItemBrowser : public Controllable
{
private:
	GridMenu menu;
	std::vector<OwnedItem*>* inventoryPtr; //a pointer to an inventory

	//void processItemInput();
public:
	ItemBrowser(); 
	void setItems(std::vector<OwnedItem*>& items);

	void setWindow(WINDOW* win);
	void processInput(int input);

	//override
	void setFocus(bool focusIn);
	
	OwnedItemRecord* getCurrentItem();

	void draw();
};

