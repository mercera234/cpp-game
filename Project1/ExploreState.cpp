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
	win = newwin(screenHeight, screenWidth, 0, 0);
}


void ExploreState::initDefaults()
{
	//load default map
	auto it = resourceManager->gameMaps.find(startingMapId); 
	if (it == resourceManager->gameMaps.end())
	{
		it = resourceManager->gameMaps.find(nullId); //load null map if default isn't found
	}
	map = &(it->second);

	//load default main character
	auto it2 = resourceManager->actors.find(player1Name);
	if (it2 == resourceManager->actors.end())
	{
		it2 = resourceManager->actors.find(nullName); //loud null Actor if default isn't found
	}
	player1 = &(it2->second);

	//set start position
	player1->x = startX;
	player1->y = startY;

	//setup exploration processor
	map->setControlActor(player1);
	map->setWindow(win);

	mapRepo.add(*map);
	explorationProcessor.setMapRepo(&mapRepo);
	explorationProcessor.setCursor(&(player1->y), &(player1->x));
	explorationProcessor.setCurrMap(map->getId());
	explorationProcessor.setViewMode(ViewMode::DYNAMIC);

	//setup trackers
	//TODO add steptracker initialization method
	encounterTracker.setMinSteps(14);
	encounterTracker.setMaxSteps(60);
	encounterTracker.setEncounterChance(5);
}


void ExploreState::loadState()
{
	
}

void ExploreState::unloadState()
{
	
}

void ExploreState::processDirectionalInput(int input)
{
	bool stepTaken = false;
	Axis stepAxis = Axis::HORIZONTAL;
	switch (input)
	{
	case GameInput::UP_INPUT: stepTaken = explorationProcessor.processMovementInput(KEY_UP); stepAxis = Axis::VERTICAL; break;
	case GameInput::DOWN_INPUT: stepTaken = explorationProcessor.processMovementInput(KEY_DOWN); stepAxis = Axis::VERTICAL; break;
	case GameInput::LEFT_INPUT: stepTaken = explorationProcessor.processMovementInput(KEY_LEFT); break;
	case GameInput::RIGHT_INPUT: stepTaken = explorationProcessor.processMovementInput(KEY_RIGHT); break;
	}
	
	if (stepTaken)
	{
		stepTracker.addStep(); //a full step was taken
		encounterTracker.takeStep(stepAxis);
		if (encounterTracker.didEncounterOccur())
		{
			//change state to battle state and load the appropriate enemy details
			GameState* state = BattleState::getInstance();
			getManager()->setState(state);
			encounterTracker.resetSteps(); //prepare for next time
		}
	}
}

void ExploreState::processInput(GameStateManager& manager, int input)
{
	bool stepTaken = false;
	Axis stepAxis = Axis::HORIZONTAL;
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
	case GameInput::OK_INPUT:
	
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
	werase(win);
	
	Map* m = explorationProcessor.getCurrMap();
	m->draw();	
}


//void ExploreState::drawNullMap(int y, int x)
//{
//	int centerY = screenHeight / 2;
//	int centerX = screenWidth / 2;
//	//get position of upper left corner of map
//	int startY = y - centerY;
//	int startX = x - centerX;
//
//	for (int row = 0, mapY = startY; row < screenHeight; row++, mapY++)
//	{
//		for (int col = 0, mapX = startX; col < screenWidth; col++, mapX++)
//		{
//			char c;
//			if (mapY % 4 == 0 && mapX % 8 == 0)
//			{
//				c = '!';
//			}
//			else {
//				c = ' ';
//			}
//			mvwaddch(win, row, col, c);
//
//		}
//	}
//}

