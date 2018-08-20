#pragma once

#include "GameState.h"
#include "curses.h"

//Main Menu options
enum class MainMenuOption
{
	INVENTORY,
	EQUIP,
	STATUS,
	SKILL,
	CONFIG,
	MAP,
	SAVE,
	MAIN_QUIT
};

class MainMenuState : public GameState
{
private:
	MainMenuState(); //private so class is Singleton
	static GameState* instance;
	WINDOW* screen;
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
};