#include "MainMenuState.h"
#include "GameStateManager.h"
#include "curses.h"
#include "TitleScreenState.h"
#include "ExploreState.h"
#include "GameInput.h"

GameState* MainMenuState::instance = nullptr;

GameState* MainMenuState::getInstance()
{
	if (instance == nullptr)
		instance = new MainMenuState;

	return instance;
}

MainMenuState::MainMenuState()
{
	screen = newwin(screenHeight, screenWidth, 0, 0);
}


void MainMenuState::processInput(GameStateManager& manager, int input)
{
	switch (input)
	{
	case GameInput::CANCEL_INPUT: manager.setState(ExploreState::getInstance()); break;
	case GameInput::QUIT_INPUT: manager.setState(TitleScreenState::getInstance()); break;
		
	}




}

void MainMenuState::draw()
{
	werase(screen);
	mvwprintw(screen, 0, 1, "MAIN MENU STATE");
	wnoutrefresh(screen);
}