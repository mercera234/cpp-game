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
#include "ControlManager.h"
#include "ControlHandle.h"
#include "SimpleCommand.h"
#include "EncounterTracker.h"

class ExplorationProcessor : public MovementProcessor
{
private:
	ControlManager cm;
	SimpleCommand<ExplorationProcessor> exploreCmd;
	SimpleCommand<ExplorationProcessor> dialogCmd;

	ControlHandle room; //a wrapper for the current room being explored(not sure if I like this idea, but it works in a pinch)
	Sprite controlSprite; //The sprite that is seen as the character we are controlling.
	Sprite* objectSprite; //A sprite that the control Sprite has collided with
	
	Pos cursor; //position of exploration in map
	WINDOW* screen = nullptr;

	MegaMap* map; //the larger map being explored. The current room is just one room in this map
	ResourceManager* resourceManager; 

	EncounterTracker encounterTracker;

	bool inFight = false;
	bool inMenu = false;

	bool clipMode = false; //if true then character can walk anywhere without triggering anything or being obstructed
	int jumpGap = 0; //testing the jumpable tiles
	int damage = 0; //damage taken by actor while traversing the map


	void setCurrRoomId(int id);

	//return true if movement was possible
	void processDirectionalInput(int input);
	bool processMovement();
	bool processThingCollisions();
	bool processTileEffect();
	void processStepTaken(Movement& stepTaken);

	void changeFloor(int amount);
	

	void processExploreCmd();
	void processDialogCmd();

	bool findMapExit(Boundary& edge, Movement& move);
	void moveActorAcrossMapSeam(MapExit& fromMap, MapExit& toMap);

	//returns true if actor lived. False if died
	void alterActorHP(int amount);
	
public:
	ExplorationProcessor();
	~ExplorationProcessor();
	
	void setResourceManager(ResourceManager* resourceManagerIn);
	void setControlActor(Actor* controlActorIn);
	void setCursor(Pos& cursorIn);
	void draw();
	

	int processInput(int input);
	
	void barrierRoutine();
	void signRoutine();

	//getters/setters

	Actor* getControlActor() { return (Actor*)controlSprite.thing; }

	Pos	getCursor() { return cursor; }
	ResourceManager* getResourceManager() { return resourceManager; }
	WINDOW* getScreen() { return screen; }
	
	void setClipMode(bool clipModeIn) { clipMode = clipModeIn; }
	bool getClipMode() { return clipMode; }
	

	void setInFight(bool inFightIn) { inFight = inFightIn; }
	bool getInFight() { return inFight; }


	void setInMenu(bool inMenuIn) { inMenu = inMenuIn; }
	bool getInMenu() { return inMenu; }


	void setMap(MegaMap* mapIn) { map = mapIn; }
	MegaMap* getMap() { return map; }
};