#include "TitleScreenState.h"
#include "LineItem.h"
#include "GameStateManager.h"
#include "ExploreState.h"
#include "MainMenuState.h"
#include "BattleState.h"
#include "GameApp.h"
#include "GameInput.h"
#include "menu_drivers.h"
#include "defaults.h"

GameState* TitleScreenState::instance = nullptr;

GameState* TitleScreenState::getInstance()
{
	if (instance == nullptr)
	{
		instance = new TitleScreenState;
	}
	
	return instance;
}


void TitleScreenState::beginNewGame()
{
	//clear any previously loaded resources
	//initialize other needed states
	GameState* state = MainMenuState::getInstance();
	state->setResourceManager(resourceManager);

	state = BattleState::getInstance();
	state->setResourceManager(resourceManager);
	state->initDefaults();

	state = ExploreState::getInstance();
	state->setResourceManager(resourceManager);
	state->initDefaults(); //load default resources for new game
	
	GameStateManager* manager = getManager();
	manager->setState(state); //should be explore state by this point
}

void TitleScreenState::processInput(GameStateManager& manager, int input)
{
	switch (titleScreen.processInput(input))
	{
	case START_NEW:
		beginNewGame();
		break;
	case TERMINATE:
		manager.setState(nullptr);
		break;
	}
}


void TitleScreenState::draw()
{
	titleScreen.draw();
}

