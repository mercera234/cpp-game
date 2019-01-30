#pragma once
#include "GridMenu.h"
#include "GameItem.h"
#include "OwnedItemRecord.h"

class ItemBrowser : public Controllable
{
private:
	GridMenu menu;
	std::vector<Possession*>* inventoryPtr; //a pointer to an inventory

	void refreshList(); //update inventory list if changes have taken place
public:
	ItemBrowser(); 
	void setItems(std::vector<Possession*>& items);

	void setWindow(WINDOW* win);
	void processInput(int input);

	//override
	void setFocus(bool focusIn);
	
	OwnedItemRecord* getCurrentItem();

	void draw();
};

