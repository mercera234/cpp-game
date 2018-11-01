#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	//TODO bad idea
	OwnedItem* blankItem = new OwnedItem;
	blankItem->item = nullptr;
	blankItem->quantity = 0;

	inventory.push_back(blankItem);
}

Actor* ResourceManager::getPlayer1()
{
	if (playerParty.empty())
		return nullptr;

	return &playerParty.front();
}

