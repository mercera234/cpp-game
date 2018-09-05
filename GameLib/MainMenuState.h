#pragma once

#include "GameState.h"
#include "curses.h"
#include "ControlManager.h"
#include "GridMenu.h"
#include "Frame.h"
#include "MainMenu.h"

class MainMenuState : public GameState
{
private:
	MainMenuState(); //private so class is Singleton
	static GameState* instance;
	
	MainMenu mm;
	
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
	void loadState();
	void unloadState() {}
};