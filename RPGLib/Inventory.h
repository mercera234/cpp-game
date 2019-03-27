#pragma once
#include "GameItem.h"

class Inventory
{
private:
	std::vector<Possession*> itemList; //a pointer to an inventory
	int capacity = 10; //the limit to how many items we can have in the inventory

	std::vector<Possession*>::iterator findItem(GameItem* item);

	bool incrementItemQuantity(BoundInt& invItemQty, unsigned int amount);

	template <typename iterator>
	bool decrementItemQuantity(iterator, int quantity);
public:
	Inventory();

	/*Alter quantity of item to inventory. return possession of item*/
	Possession* alterItemQuantity(GameItem* item, int amount);
	
	//get quantity of one type of item
	int getItemQuantity(GameItem* item); 

	//get count of distinguishable items (disregards quantity)
	int getItemCount() { return (int)itemList.size(); }

	int getCapacity() { return capacity; }

	Possession* getPossession(int index);
};
