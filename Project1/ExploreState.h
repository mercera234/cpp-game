#pragma once
#include "GameState.h"
#include "ExplorationProcessor.h"
#include "MapRepository.h"

class ExploreState : public GameState
{
private:
	ExploreState(); //private so class is Singleton
	static GameState* instance;
	ExplorationProcessor explorationProcessor;
	MapRepository mapRepo;
	Map map;
	
	//temporary for position in map
	int y = 0;
	int x = 0;

	void drawNullMap(int y, int x);
public:
	//~ExploreState();
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void loadState();
	void unloadState();
	void processInput(GameStateManager& manager, int input);
	void draw();
};