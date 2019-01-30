#include <utility>
#include "ExploreState.h"
#include "GameStateManager.h"
#include "curses.h"
#include "TitleScreenState.h"
#include "GameInput.h"
#include "MainMenuState.h"
#include "BattleState.h"
#include "actor_helper.h"
#include "defaults.h"

GameState* ExploreState::instance = nullptr;

GameState* ExploreState::getInstance()
{
	if (instance == nullptr)
		instance = new ExploreState;

	return instance;
}


ExploreState::ExploreState()
{
	
}


void ExploreState::initDefaults()
{
	//like starting a new game here
	setupDefaultDataKeys(*resourceManager);


	loadResourceManagerData();

	//setup exploration processor
	explorationProcessor.setResourceManager(resourceManager);

	Actor* contrlActor = &resourceManager->playerParty.front();
	explorationProcessor.setControlActor(contrlActor);	
	//set start position
	Pos start(startY, startX);
	explorationProcessor.setCursor(start);
	explorationProcessor.setViewMode(ViewMode::DYNAMIC);
}

void ExploreState::loadResourceManagerData()
{
	//load default megamap
	resourceManager->currMap = &resourceManager->getData().getMap("TestRegion");
	MegaMap* currMap = resourceManager->currMap;
	currMap->setUnitHeight(gameScreenHeight);
	currMap->setUnitWidth(gameScreenWidth);
	currMap->setFloor(0);

	//load default main character
	Actor player1 = resourceManager->getData().getActor(player1Name);
	player1.setType(ActorType::HUMAN);

	resourceManager->playerParty.clear(); //clear out old data first
	resourceManager->playerParty.push_back(player1);
}


void ExploreState::loadState()
{
	
}

void ExploreState::unloadState()
{
	
}

void ExploreState::processInput(GameStateManager& manager, int input)
{
	int exitCode = explorationProcessor.processInput(input);

	switch (exitCode)
	{
	case ExitCode::QUIT_TO_TITLE: 
	{
		GameState* state = TitleScreenState::getInstance();
		manager.setState(state);
	}
		break;
	}


	if (explorationProcessor.getInFight())
	{
		GameState* state = BattleState::getInstance();
		manager.setState(state);
		explorationProcessor.setInFight(false); //clear for next time
	}
	else if (explorationProcessor.getInMenu())
	{
		GameState* state = MainMenuState::getInstance();
		manager.setState(state);
		explorationProcessor.setInMenu(false);
	}

}


void ExploreState::draw()
{
	explorationProcessor.draw();
}


