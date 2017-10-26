#include "TestState.h"
#include "GameStateManager.h"
#include "curses.h"
#include "TitleScreenState.h"

GameState* TestState::instance = nullptr;

GameState* TestState::getInstance()
{
	if (instance == nullptr)
		instance = new TestState;

	return instance;
}

void TestState::processInput(GameStateManager& manager, int input)
{
	if (input == 'q')
	{
		manager.setState(TitleScreenState::getInstance());
		//manager->setState(nullptr);
	}
}

void TestState::draw()
{
	mvwprintw(stdscr, 0, 1, "TEST STATE");
	wnoutrefresh(stdscr);
}