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



