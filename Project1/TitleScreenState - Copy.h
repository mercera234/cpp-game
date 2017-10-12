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
	GridMenu titleMenu;
public:
	TitleScreenState(WINDOW* menuWin);
	void processInput(int input);
	void draw();
};