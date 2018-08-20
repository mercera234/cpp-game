#pragma once
#include "GameState.h"

class MockState1 : public GameState
{
private:
	static GameState* instance;
	MockState1(); 
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw() {}
	void loadState() {}
	void unloadState() {}
};