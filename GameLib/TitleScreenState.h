#pragma once
#include "GameState.h"
#include "GridMenu.h"
#include "curses.h"
#include "TitleScreen.h"


class TitleScreenState : public GameState
{
private:
	TitleScreenState(); //private so class is Singleton
	
	static GameState* instance;

	TitleScreen titleScreen;
	GridMenu titleMenu;

	void beginNewGame();

public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void loadState();
	void unloadState();
	void draw();
};