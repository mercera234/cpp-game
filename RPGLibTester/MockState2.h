#pragma once
#include "GameState.h"

class MockState2 : public GameState
{
private:
	static GameState* instance;
	MockState2();
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw() {}
};