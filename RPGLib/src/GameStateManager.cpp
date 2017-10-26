#include "GameStateManager.h"
#include "curses.h"

void GameStateManager::setState(GameState* state)
{
	this->state = state;
}


bool GameStateManager::processInput(int input)
{
	if (state == nullptr) //nothing to process
		return false;

	state->processInput(*this, input);

	return true;
}

void GameStateManager::draw()
{
	if (state) 
	{
		state->draw();
	}
	else //nothing to draw
	{
		mvprintw(1, 1, "No state set!");
	}
}