#include "MockStatus.h"
#include "MockMainMenu.h"
#include "GameStateManager.h"
#include "curses.h"

GameState* MockStatus::instance = nullptr;

GameState* MockStatus::getInstance()
{
	if (instance == nullptr)
		instance = new MockStatus;

	return instance;
}

MockStatus::MockStatus()
{ 
	
}


void MockStatus::processInput(GameStateManager& manager, int input)
{
	switch(input)
	{
	case 'x':
		manager.setState(MockMainMenu::getInstance()); 
		break;
	}
		
}


void MockStatus::draw()
{
	clear();
	mvaddstr(1, 1, "MockStatus");
	wnoutrefresh(stdscr);
}