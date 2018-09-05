#pragma once
#include "GameState.h"
#include "ExplorationProcessor.h"
#include "MapRepository.h"
#include "WadDatabase.h"
#include "EncounterTracker.h"


class ExploreState : public GameState
{
private:
	ExploreState(); //private so class is Singleton
	static GameState* instance;
	ExplorationProcessor explorationProcessor;
	MapRepository mapRepo;
	Map* map;
	Actor player1;

	EncounterTracker encounterTracker;
	
	void loadResourceManagerData();

	void processDirectionalInput(int input);
	void processStepTaken(Axis stepAxis);
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void initDefaults();
	void loadState();
	void unloadState();
	void processInput(GameStateManager& manager, int input);
	void draw();
};