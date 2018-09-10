#pragma once
#include "MovementProcessor.h"
#include <map>
#include <functional>
#include "MapExit.h"
#include "MapRoom.h"
#include "MapRepository.h"
#include "MegaMap.h"
#include "ResourceManager.h"
#include "Sprite.h"

class ExplorationProcessor : public MovementProcessor
{
private:
	MapRoom* currMap = nullptr;
	
	//The sprite that is seen as the character we are controlling.
	Sprite controlSprite;
	Pos cursor; //position of exploration in map

	//return true if movement was possible
	bool processMovement(Movement& move);

	WINDOW* screen = nullptr;

	MegaMap map;
	ResourceManager* resourceManager;

	bool clipMode = false;
	bool findMapExit(Boundary& edge, Movement& move);
	int jumpGap = 0; //testing the jumpable tiles

	void moveActorAcrossMapSeam(MapExit& fromMap, MapExit& toMap);

	bool processThingCollisions(Movement& move);
	bool processTileEffect(Movement& move);

	//returns true if actor lived. False if died
	void alterActorHP(int amount);
	
	int damage = 0; //damage taken by actor while traversing the map
public:
	ExplorationProcessor();
	
	void setCursor(Pos cursorIn);

	
	Pos	getCursor() { return cursor; }



	void setCurrMap(int id);

	//getters/setters
	void setClipMode(bool clipModeIn) { clipMode = clipModeIn; }
	bool getClipMode() { return clipMode; }

	void setResourceManager(ResourceManager* resourceManagerIn) { resourceManager = resourceManagerIn; }
	ResourceManager* getResourceManager() { return resourceManager; }


	void setMap(MegaMap& mapIn) { map = mapIn; }
	MegaMap& getMap() { return map; }


	WINDOW* getScreen() { return screen; }


	void setControlActor(Actor* controlActorIn);
	
	MapRoom* getCurrMap() { return currMap; }

	void draw();
};