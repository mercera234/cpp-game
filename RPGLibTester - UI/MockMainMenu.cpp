#include "MockMainMenu.h"
#include "MockExplore.h"
#include "MockTitle.h"
#include "MockStatus.h"
#include "GameStateManager.h"
#include "curses.h"

GameState* MockMainMenu::instance = nullptr;

GameState* MockMainMenu::getInstance()
{
	if (instance == nullptr)
		instance = new MockMainMenu;

	return instance;
}

MockMainMenu::MockMainMenu()
{

}

void MockMainMenu::processInput(GameStateManager& manager, int input)
{
	switch (input)
	{
	case 'c':
		manager.setState(MockStatus::getInstance());
		break;
	case 'x':
		manager.setState(MockExplore::getInstance());
		break;
	case 'q':
		manager.setState(MockTitle::getInstance());
		break;
	}
}


void MockMainMenu::draw()
{
	clear();
	mvaddstr(1, 1, "MockMainMenu");
	wnoutrefresh(stdscr);
}