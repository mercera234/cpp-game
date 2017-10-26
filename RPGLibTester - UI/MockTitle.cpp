#include "MockTitle.h"
#include "MockExplore.h"
#include "GameStateManager.h"
#include "curses.h"

GameState* MockTitle::instance = nullptr;

GameState* MockTitle::getInstance()
{
	if (instance == nullptr)
		instance = new MockTitle;

	return instance;
}

MockTitle::MockTitle()
{ 
	setName("MockTitle"); 
}


void MockTitle::processInput(GameStateManager& manager, int input)
{
	switch(input)
	{
	case 'c':
		manager.setState(MockExplore::getInstance()); 
		break;
	case 'q':
		manager.setState(nullptr);
		break;
	}
		
}


void MockTitle::draw()
{
	clear();
	mvaddstr(1, 1, name.c_str());
	wnoutrefresh(stdscr);
}