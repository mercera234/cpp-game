#pragma once
#include "GameState.h"

class MockFight : public GameState
{
private:
	static GameState* instance;
	MockFight();
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
	void loadState() {}
	void unloadState() {}
};