#include "GameStateManager.h"
#include "curses.h"

void GameStateManager::setState(GameState* state)
{
	this->state = state;

	if(state)
		state->setGameStateManager(this);
}


bool GameStateManager::processInput(int input)
{
	if (state == nullptr) //nothing to process
		return false;

	state->processInput(input);

	//return stack.empty() == false; //return true if there are states left to process
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