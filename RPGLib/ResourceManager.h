#pragma once
#include <map>
#include "Actor.h"
#include "GameData.h"
#include "MegaMap.h"
#include "InputManager.h"
//#include "ItemBrowser.h"
#include "Inventory.h"

class ResourceManager
{
private:
	GameData theData;
	InputManager inputManager;
	//ItemBrowser invBrowser;
	Inventory inventory;

public:
	ResourceManager();
	
	std::vector<Actor> playerParty; //a copy of all actors that the user currently controls
	MegaMap* currMap; //the current map that the player is in
	Actor* getPlayer1();


	
	//TODO add setCurrMap(std::string)

	//setters/getters

	void setData(GameData& dataIn) { theData = dataIn; }//TODO not sure if we need this method
	GameData& getData() { return theData; }

	InputManager& getInputManager() { return inputManager; }
	//ItemBrowser& getinvBrowser() { return invBrowser; }

	Inventory& getInventory() { return inventory; }

};

