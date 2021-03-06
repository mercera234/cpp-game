#pragma once

#include "TUI.h"
#include "GridMenu.h"
#include "InputManager.h"
#include "ResourceManager.h"

class GameApp
{
private:
	TUI tui;
	ResourceManager resourceManager;

public:
	GameApp();
	
	/*The method that runs the game*/
	bool run();
};