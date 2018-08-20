#pragma once
#include "GameState.h"

class MockMainMenu : public GameState
{
private:
	static GameState* instance;
	MockMainMenu();
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
	void loadState() {}
	void unloadState() {}
};