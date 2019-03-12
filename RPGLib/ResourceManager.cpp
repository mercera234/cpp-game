#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	//setup map with null value to start
	currMap = &theData.getMap(nullName);
}

Actor* ResourceManager::getPlayer1()
{
	if (playerParty.empty())
		return nullptr;

	return &playerParty.front();
}

std::vector<Possession*>::iterator ResourceManager::findInventoryItem(GameItem* item)
{
	auto it = std::find_if(inventory.begin(), inventory.end(),
		[item](Possession* posn) {
		if (posn == nullptr || posn->item == nullptr)
			return false;

		return posn->item == item;
	});

	return it;
}


bool ResourceManager::acquireItem(GameItem* item, int quantity) //add item to inventory
{
	//attempt to find item in inventory first and add to it
	auto it = findInventoryItem(item);

	if (it != inventory.end())
	{
		(*it)->quantity.alterCurr(quantity);
	}
	else
	{
		Possession* posn = new Possession;
		posn->quantity.setCurr(quantity);
		posn->item = item;
		inventory.push_back(posn);
	}

	return true;
}

