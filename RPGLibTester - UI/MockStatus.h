#pragma once
#include "GameState.h"

class MockStatus : public GameState
{
private:
	static GameState* instance;
	MockStatus();
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
	void loadState() {}
	void unloadState() {}
};