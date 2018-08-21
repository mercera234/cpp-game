#include "MainMenuState.h"
#include "GameStateManager.h"
#include "curses.h"
#include "TitleScreenState.h"
#include "ExploreState.h"
#include "GameInput.h"
#include "LineItem.h"
#include "GameApp.h"

GameState* MainMenuState::instance = nullptr;

GameState* MainMenuState::getInstance()
{
	if (instance == nullptr)
		instance = new MainMenuState;

	return instance;
}

MainMenuState::MainMenuState()
{
	win = newwin(screenHeight, screenWidth, 0, 0);
	cm.setCaller(this);

	setupMainMenu();

	/*playerFrame;
	playerMenu.resetItems(4, 1);
	playerMenu.setWindow*/

	cm.registerControl(mainFrame, KEY_LISTENER, mainMenuCallback);
	cm.setFocus(mainFrame);
}

void MainMenuState::setupMainMenu()
{
	mainMenu.resetItems(4, 2);
	mainMenu.setWindow(newwin(4, 18, 1, 1)); //more windows to be deleted later

	mainMenu.setItemWidth(6);
	mainMenu.setColSepLength(0);

	mainMenu.setItem(new LineItem("Item", 0, MainMenuOption::INVENTORY), 0, 0);
	mainMenu.setItem(new LineItem("Equip", 1, MainMenuOption::EQUIP), 1, 0);
	mainMenu.setItem(new LineItem("Status", 2, MainMenuOption::STATUS), 2, 0);
	mainMenu.setItem(new LineItem("Skill", 3, MainMenuOption::SKILL), 3, 0);
	mainMenu.setItem(new LineItem("Config", 4, MainMenuOption::CONFIG), 0, 1);
	mainMenu.setItem(new LineItem("Map", 5, MainMenuOption::MAP), 1, 1);
	mainMenu.setItem(new LineItem("Save", 6, MainMenuOption::SAVE), 2, 1);
	mainMenu.setItem(new LineItem("Quit", 7, MainMenuOption::MAIN_QUIT), 3, 1);

	mainMenu.setCurrentItem(0);
	mainMenu.setWrapAround(false);
	mainMenu.setFocus(true);
	mainMenu.post(true);

	mainFrame = new Frame(newwin(6, 20, 0, 0), &mainMenu);
}


void MainMenuState::processInput(GameStateManager& manager, int input)
{
	cm.handleInput(input);
}


void MainMenuState::mainMenuCallback(void* caller, void* ptr, int input) //static
{
	MainMenuState* state = (MainMenuState*)caller;
	state->processMainMenuInput((Frame*)ptr, input);
}

void MainMenuState::processMainMenuInput(Frame* f, int input)
{
	GameStateManager* manager = getManager();
	if(input == GameInput::CANCEL_INPUT)
	{
		manager->setState(ExploreState::getInstance());
		return;
	}


	MenuItem* item = GameApp::menuDriver(input, &mainMenu);

	if (item)
	{
		switch (((LineItem*)item)->getCrossRef())
		{
		case MainMenuOption::MAIN_QUIT: manager->setState(TitleScreenState::getInstance()); break;
		}
	}
}

void MainMenuState::draw()
{
	werase(win);
	wnoutrefresh(win);

	cm.draw();
}