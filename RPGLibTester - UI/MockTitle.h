#pragma once
#include "GameState.h"

class MockTitle : public GameState
{
private:
	static GameState* instance;
	MockTitle();
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
};