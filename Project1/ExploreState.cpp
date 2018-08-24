#include <utility>
#include "ExploreState.h"
#include "GameStateManager.h"
#include "curses.h"
#include "TitleScreenState.h"
#include "GameInput.h"
#include "MainMenuState.h"
#include "BattleState.h"
#include "actor_helper.h"


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
	/*map.setControlActor(nullActor);
	map.setWindow(win);
	
	map.setId(1);
	mapRepo.add(map);
	explorationProcessor.setMapRepo(&mapRepo);
	explorationProcessor.setCursor(&(nullActor->y), &(nullActor->x));
	explorationProcessor.setCurrMap(map.getId());
	explorationProcessor.setViewMode(ViewMode::DYNAMIC);*/
}


void ExploreState::initDefaults()
{
	auto it = resourceManager->gameMaps.find(12); //id should be passed in to method
	map = it->second;

	Actor* nullActor = new Actor();
	nullActor->symbol = 'X' | COLOR_RED_BOLD << TEXTCOLOR_OFFSET;
	nullActor->x = 0;
	nullActor->y = 0;

	map.setControlActor(nullActor);
	map.setWindow(win);

	map.setId(1);
	mapRepo.add(map);
	explorationProcessor.setMapRepo(&mapRepo);
	explorationProcessor.setCursor(&(nullActor->y), &(nullActor->x));
	explorationProcessor.setCurrMap(map.getId());
	explorationProcessor.setViewMode(ViewMode::DYNAMIC);
}


void ExploreState::loadState()
{
	
}

void ExploreState::unloadState()
{
	
}


void ExploreState::processInput(GameStateManager& manager, int input)
{
	switch (input)
	{	
	case GameInput::UP_INPUT: explorationProcessor.processMovementInput(KEY_UP); break;
	case GameInput::DOWN_INPUT: explorationProcessor.processMovementInput(KEY_DOWN); break;
	case GameInput::LEFT_INPUT: explorationProcessor.processMovementInput(KEY_LEFT); break;
	case GameInput::RIGHT_INPUT: explorationProcessor.processMovementInput(KEY_RIGHT); break;
	case GameInput::FIGHT_TRIGGER: //we will remove this later, but this is for triggering a fight immediately
		manager.setState(BattleState::getInstance()); break; 
	case GameInput::OK_INPUT:
	
		break;
	case GameInput::OPEN_MENU_INPUT: manager.setState(MainMenuState::getInstance()); break;
	}

	
		
	
}

void ExploreState::draw()
{
	werase(win);
	if (map.getId() == 0)
		drawNullMap(y, x);
	else
	{
		Map* m = explorationProcessor.getCurrMap();
		m->draw();
	}
		
}


void ExploreState::drawNullMap(int y, int x)
{
	int centerY = screenHeight / 2;
	int centerX = screenWidth / 2;
	//get position of upper left corner of map
	int startY = y - centerY;
	int startX = x - centerX;

	for (int row = 0, mapY = startY; row < screenHeight; row++, mapY++)
	{
		for (int col = 0, mapX = startX; col < screenWidth; col++, mapX++)
		{
			char c;
			if (mapY % 4 == 0 && mapX % 8 == 0)
			{
				c = '!';
			}
			else {
				c = ' ';
			}
			mvwaddch(win, row, col, c);

		}
	}
}

