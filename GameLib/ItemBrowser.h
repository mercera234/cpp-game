#pragma once
#include "GridMenu.h"
#include "GameItem.h"


class ItemBrowser : public Controllable
{
private:
	GridMenu menu;
	std::vector<Possession*>* inventoryPtr; //a pointer to an inventory
public:
	ItemBrowser(); 
	void setItems(std::vector<Possession*>& items);

	void setWindow(WINDOW* win);
	void processInput(int input);

	//override
	void setFocus(bool focusIn);
	
	GameItem* getCurrentItem();

	/*remove quantity of 1 from a particular item(remove if necessary)
	return true if item could be successfully decremented, false otherwise
	*/
	bool decrementItem(); 

	const GridMenu& getMenu() { return menu; }

	void draw();
};

