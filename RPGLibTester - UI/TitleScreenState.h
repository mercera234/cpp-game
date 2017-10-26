#pragma once
#include "GameState.h"
#include "GridMenu.h"
#include "curses.h"

enum class TitleOption
{
	NEW_GAME,
	LOAD_GAME, 
	QUIT_GAME,
	EDIT_GAME
};


class TitleScreenState : public GameState
{
private:
	//TitleScreenState(WINDOW* menuWin); //private so class is Singleton
	TitleScreenState();
	static GameState* instance;

	GridMenu* titleMenu;
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
};