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
	mm.setWindow(newwin(screenHeight, screenWidth, 0, 0));
}


void MainMenuState::loadState()
{
	mm.addPlayerParty(resourceManager->playerParty);
	mm.setData(&resourceManager->theData);
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