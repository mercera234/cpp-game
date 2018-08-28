#pragma once

#include "TUI.h"
//#include "stateManager.h"
#include "GridMenu.h"
#include "InputManager.h"
#include "ResourceManager.h"
//#include "WadDatabase.h"

class GameApp
{
private:
	TUI tui;
	InputManager inputManager;
	ResourceManager resourceManager;

	int getInput();
public:
	GameApp();
	
	/*The method that runs the game*/
	bool run();


	static MenuItem* menuDriver(int input, AbstractMenu* m);
};