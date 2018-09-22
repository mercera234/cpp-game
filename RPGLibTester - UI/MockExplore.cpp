#include "MockExplore.h"
#include "MockTitle.h"
#include "MockMainMenu.h"
#include "MockFight.h"
#include "GameStateManager.h"
#include "curses.h"

GameState* MockExplore::instance = nullptr;

GameState* MockExplore::getInstance()
{
	if (instance == nullptr)
		instance = new MockExplore;

	return instance;
}

MockExplore::MockExplore()
{
}

void MockExplore::processInput(GameStateManager& manager, int input)
{
	switch (input)
	{
	case 'q':
		manager.setState(MockTitle::getInstance());
		break;
	case 'c':
		manager.setState(MockMainMenu::getInstance());
		break;
	case 'f':
		manager.setState(MockFight::getInstance());
		break;
	}
}


void MockExplore::draw()
{
	clear();
	mvaddstr(1, 1, "MockExplore");
	wnoutrefresh(stdscr);
}