#pragma once
#include "GameState.h"
#include "GridMenu.h"
#include "curses.h"

enum TitleMenuOptions
{
	NEW_GAME,
	LOAD_GAME, 
	QUIT_GAME,
	EDIT_GAME
};


class TitleScreenState : public GameState
{
private:
	TitleScreenState(); //private so class is Singleton
	
	static GameState* instance;

	GridMenu titleMenu;
public:
	//~TitleScreenState();
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void loadState();
	void unloadState();
	void draw();
};