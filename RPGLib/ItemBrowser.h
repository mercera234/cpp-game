#pragma once
#include "GridMenu.h"
#include "GameItem.h"
#include "OwnedItemRecord.h"
#include "Inventory.h"

class ItemBrowser : public Controllable, public InputProcessor
{
private:
	GridMenu menu;
	Inventory* inventory = nullptr;
	
	/*add a gameitem in the inventory to the menu in the first blank position*/
	void addPossessionToMenu(Possession* posn); //the index in the inventory
public:
	ItemBrowser() {}
	
	void processInput();
	void setInventory(Inventory* inventoryIn);

	//override
	void setFocus(bool focusIn);
	void setWindow(WINDOW* win);
	void draw();
	
	
	GameItem* getCurrentItem();
	bool setCurrentItem(int index);

	/*Run a filter to process all the records a particular way (like only show consumable items, or only show equippable swords)*/
	template<typename T>
	void runFilter(T);

	/*remove quantity of 1 from a particular item(remove if necessary)
	return true if item could be successfully decremented, false otherwise*/
	bool decrementItem(); 

	/*Add quantity of item to inventory. return true if successfully added*/
	bool acquireItem(GameItem* item, int amount);

	//get quantity of one type of item
	int getItemQuantity(GameItem* item); 

	//get count of distinguishable items (disregards quantity)
	int getItemCount();// { return (int)inventory.size(); }
	
};

template<typename T>
void ItemBrowser::runFilter(T method)
{
	for (int i = 0; i < (int)menu.getMaxItems(); i++)
	{
		OwnedItemRecord* record = (OwnedItemRecord*)menu.AbstractMenu::getItem(i);
		method(record);
	}
}