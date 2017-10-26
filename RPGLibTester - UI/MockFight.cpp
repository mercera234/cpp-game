#include "MockFight.h"
#include "MockExplore.h"
#include "GameStateManager.h"
#include "curses.h"

GameState* MockFight::instance = nullptr;

GameState* MockFight::getInstance()
{
	if (instance == nullptr)
		instance = new MockFight;

	return instance;
}

MockFight::MockFight()
{ 
	setName("MockFight"); 
}


void MockFight::processInput(GameStateManager& manager, int input)
{
	switch(input)
	{
	case 'x':
		manager.setState(MockExplore::getInstance()); 
		break;
	}
		
}


void MockFight::draw()
{
	clear();
	mvaddstr(1, 1, name.c_str());
	wnoutrefresh(stdscr);
}