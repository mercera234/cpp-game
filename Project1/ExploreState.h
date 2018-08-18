#pragma once
#include "GameState.h"

class ExploreState : public GameState
{
private:
	ExploreState() {}; //private so class is Singleton
	static GameState* instance;

	//temporary for position in map
	int y = 0;
	int x = 0;

	void drawNullMap(int y, int x);
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
};