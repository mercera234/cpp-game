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
	explorationProcessor.setControlActor(&player1);	
	//set start position
	Pos start(startY, startX);
	explorationProcessor.setCursor(start);
	explorationProcessor.setViewMode(ViewMode::DYNAMIC);

	
	
	//setup trackers
	encounterTracker.setMinSteps(minPeaceSteps);
	encounterTracker.setMaxSteps(maxPeaceSteps);
	encounterTracker.setEncounterChance(encounterChance);
}

void ExploreState::loadResourceManagerData()
{
	//load default megamap
	auto it = resourceManager->gameMaps.find("TestRegion");
	if (it == resourceManager->gameMaps.end())
	{
		it = resourceManager->gameMaps.find(nullName);
	}
	MegaMap& mm = explorationProcessor.getMap();
	mm = it->second;
	mm.setUnitHeight(screenHeight);
	mm.setUnitWidth(screenWidth);
	mm.setFloor(0);

	//load default main character
	auto it2 = resourceManager->actors.find(player1Name);
	if (it2 == resourceManager->actors.end())
	{
		it2 = resourceManager->actors.find(nullName); //loud null Actor if default isn't found
	}
	player1 = (it2->second); //get copy of player1
	player1.type = ActorType::HUMAN;

	resourceManager->playerParty.clear(); //clear out old data first
	resourceManager->playerParty.push_back(&player1);

}


void ExploreState::loadState()
{
	
}

void ExploreState::unloadState()
{
	
}

void ExploreState::processStepTaken(Movement& stepTaken)
{
	resourceManager->theData.alterIntData(STEPS, 1);
	
	encounterTracker.takeStep(getAxis(stepTaken.dir));
	if (encounterTracker.didEncounterOccur())
	{
		//change state to battle state and load the appropriate enemy details
		GameState* state = BattleState::getInstance();
		getManager()->setState(state);
		encounterTracker.resetSteps(); //prepare for next time

		//get enemy pool from current map

		//get specific enemy group

		//pass enemies into BattleState
	}
}

void ExploreState::processDirectionalInput(int input)
{
	//bool stepTaken = false;
	//Axis stepAxis = Axis::HORIZONTAL;

	MovementChain moves;
	switch (input)
	{
		//These should all be only one move at a time
	case GameInput::UP_INPUT: moves = explorationProcessor.processMovementInput(KEY_UP); break;
	case GameInput::DOWN_INPUT: moves = explorationProcessor.processMovementInput(KEY_DOWN); break;
	case GameInput::LEFT_INPUT: moves = explorationProcessor.processMovementInput(KEY_LEFT); break;
	case GameInput::RIGHT_INPUT: moves = explorationProcessor.processMovementInput(KEY_RIGHT); break;
	}

	//verify that actor is still alive
	if (isAlive(player1) == false)
	{
		player1.symbol = nullSymbol;
		return;
	}

	Movement stepTaken = moves.moves.back();
	if (stepTaken.magnitude > 0)
	{
		processStepTaken(stepTaken);
	}
}

void ExploreState::processInput(GameStateManager& manager, int input)
{
	if (isAlive(player1) == false && input != GameInput::OK_INPUT) //only accept ok input if player has died
		return;

	switch (input)
	{	
	case GameInput::UP_INPUT: 
	case GameInput::DOWN_INPUT: 
	case GameInput::LEFT_INPUT: 
	case GameInput::RIGHT_INPUT: 
		processDirectionalInput(input);
		break;
	case GameInput::FIGHT_TRIGGER: //we will remove this later, but this is for triggering a fight immediately
	{
		GameState* state = BattleState::getInstance();
		manager.setState(state);
	}
		break;
	case GameInput::TOGGLE_ENCOUNTERS:
	{
		encounterTracker.setTracking(!encounterTracker.getTracking());
	}
		
		break;
	case GameInput::OK_INPUT:
		if (isAlive(player1) == false)
		{
			GameState* state = TitleScreenState::getInstance();
			manager.setState(state);
		}
		break;
	case GameInput::OPEN_MENU_INPUT: 
	{
		GameState* state = MainMenuState::getInstance();
		manager.setState(state);
	}
		
		break;
	}

	
}


void ExploreState::draw()
{
	explorationProcessor.draw();
}


