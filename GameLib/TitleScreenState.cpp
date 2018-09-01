#include "TitleScreenState.h"
#include "LineItem.h"
#include "GameStateManager.h"
#include "ExploreState.h"
#include "MainMenuState.h"
#include "BattleState.h"
#include "GameApp.h"
#include "GameInput.h"
#include "menu_drivers.h"

GameState* TitleScreenState::instance = nullptr;

GameState* TitleScreenState::getInstance()
{
	if (instance == nullptr)
	{
		instance = new TitleScreenState;
	}
	
	return instance;
}


//Title Menu options
TitleScreenState::TitleScreenState() 
{
	win = newwin(screenHeight, screenWidth, 0, 0);
	WINDOW* menuWin = newwin(4, 25, 1, 1); //TODO window must be deleted later!!
	titleMenu.resetItems(3, 1);
	titleMenu.setWindow(menuWin);
	
	titleMenu.setItem(new LineItem("New Game", 0, TitleMenuOptions::NEW_GAME));
	titleMenu.setItem(new LineItem("Load Game", 1, TitleMenuOptions::LOAD_GAME));
	titleMenu.setItem(new LineItem("Quit Game", 2, TitleMenuOptions::QUIT_GAME));

	titleMenu.setCurrentItem(0);
	titleMenu.setWrapAround(false);
	titleMenu.setFocus(true);
	titleMenu.post(true);
}


void TitleScreenState::loadState()
{
	
}

void TitleScreenState::unloadState()
{

}


void TitleScreenState::beginNewGame()
{
	//clear any previously loaded resources
	//initialize other needed states
	GameState* state = MainMenuState::getInstance();
	state->setResourceManager(resourceManager);

	state = BattleState::getInstance();
	state->setResourceManager(resourceManager);

	ExploreState* exploreState = (ExploreState*)ExploreState::getInstance();
	exploreState->setResourceManager(resourceManager); 
	exploreState->initDefaults(); //load default resources for new game
	
	GameStateManager* manager = getManager();
	manager->setState(exploreState);
}

void TitleScreenState::processInput(GameStateManager& manager, int input)
{
	MenuItem* item = menuDriver(input, &titleMenu);
		
	if (item) 
	{
		switch (((LineItem*)item)->getCrossRef())
		{
		case TitleMenuOptions::NEW_GAME:
			beginNewGame();
			break;
		case TitleMenuOptions::LOAD_GAME:

			break;
		case TitleMenuOptions::EDIT_GAME:
			break;
		case TitleMenuOptions::QUIT_GAME:
			manager.setState(nullptr);
			break;
		}
	}
}


void TitleScreenState::draw()
{
	werase(win);
	mvwprintw(win, 0, 1, "---KILL, IMPROVE, REPEAT---");
	wnoutrefresh(win);
	titleMenu.draw();
}

