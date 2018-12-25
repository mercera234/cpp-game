#include "MainMenuState.h"
#include "GameStateManager.h"
#include "curses.h"
#include "TitleScreenState.h"
#include "ExploreState.h"
#include "GameInput.h"
#include "LineItem.h"
#include "GameApp.h"
#include "defaults.h"
#include "menu_drivers.h"

GameState* MainMenuState::instance = nullptr;

GameState* MainMenuState::getInstance()
{
	if (instance == nullptr)
		instance = new MainMenuState;

	return instance;
}

MainMenuState::MainMenuState() 
{
	
}


void MainMenuState::loadState()
{
	mm.setResourceManager(resourceManager); //TODO, this doesn't hurt, but it shouldn't need to be reset every time state loads
	if(mm.getWindow() == nullptr)
		mm.setWindow(newwin(gameScreenHeight, gameScreenWidth, 0, 0));

	mm.addPlayerParty(resourceManager->playerParty);
}



void MainMenuState::processInput(GameStateManager& manager, int input)
{
	int exitCode = mm.processInput(input);

	switch (exitCode)
	{
	case ExitCode::GO_BACK: 
		manager.setState(ExploreState::getInstance());
		break;
	case ExitCode::QUIT_TO_TITLE: 
		resourceManager->playerParty.clear(); //clear out players
		manager.setState(TitleScreenState::getInstance()); 
		break;
	}
	
}



void MainMenuState::draw()
{
	mm.draw();
}