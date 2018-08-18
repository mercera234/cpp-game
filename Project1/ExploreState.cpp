#include "ExploreState.h"
#include "GameStateManager.h"
#include "curses.h"
#include "TitleScreenState.h"
#include "GameInput.h"
#include "MainMenuState.h"

GameState* ExploreState::instance = nullptr;

GameState* ExploreState::getInstance()
{
	if (instance == nullptr)
		instance = new ExploreState;

	return instance;
}

void ExploreState::processInput(GameStateManager& manager, int input)
{
	switch (input)
	{	
	case GameInput::UP_INPUT:
		y--;
		break;
	case GameInput::DOWN_INPUT:
		y++;
		break;
	case GameInput::LEFT_INPUT:
		x--;
		break;
	case GameInput::RIGHT_INPUT:
		x++;
		break;
	case GameInput::FIGHT_TRIGGER: //we will remove this later, but this is for triggering a fight automatically
		//g->stateManager.push(fight, g);
		break;
	case GameInput::OK_INPUT:
	//	g->stateManager.push(mainMenuState, g);
		break;
	case GameInput::CANCEL_INPUT: manager.setState(TitleScreenState::getInstance()); break; //temporary
	case GameInput::OPEN_MENU_INPUT: manager.setState(MainMenuState::getInstance());  break;
	//case GameInput::FIGHT_TRIGGER: break;
	}

	
		
	
}

void ExploreState::draw()
{
	drawNullMap(y, x);

	wnoutrefresh(stdscr);
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
			mvaddch(row, col, c);

		}
	}
}