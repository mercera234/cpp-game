#pragma once
#include <map>
#include "Actor.h"
#include "GameData.h"
#include "MegaMap.h"
#include "InputManager.h"

class ResourceManager
{
private:
	GameData theData;
	InputManager inputManager;

	std::vector<Possession*>::iterator findInventoryItem(GameItem* item);
public:
	ResourceManager();
	
	std::vector<Actor> playerParty; //a copy of all actors that the user currently controls
	MegaMap* currMap; //the current map that the player is in
	Actor* getPlayer1();


	std::vector<Possession*> inventory;
	
	bool acquireItem(GameItem* item, int quantity); //add item to inventory

	//TODO add setCurrMap(std::string)

	//setters/getters

	void setData(GameData& dataIn) { theData = dataIn; }//TODO not sure if we need this method
	GameData& getData() { return theData; }

	InputManager& getInputManager() { return inputManager; }

};

