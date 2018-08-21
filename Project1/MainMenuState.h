#pragma once

#include "GameState.h"
#include "curses.h"
#include "ControlManager.h"
#include "GridMenu.h"
#include "Frame.h"

//Main Menu options
enum MainMenuOption
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
	ControlManager cm;

	Frame* mainFrame;
	GridMenu mainMenu;

	/*Frame* playerFrame;
	GridMenu playerMenu;*/


	void processMainMenuInput(Frame* f, int input);

	void setupMainMenu();
public:
	static GameState* getInstance(); //since static, cannot be virtual in super class
	void processInput(GameStateManager& manager, int input);
	void draw();
	void loadState() {}
	void unloadState() {}

	static void mainMenuCallback(void* caller, void* ptr, int input);
};