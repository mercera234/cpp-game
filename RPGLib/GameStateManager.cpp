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

	return state != nullptr; //return if state was invalidated during processing
}

void GameStateManager::draw()
{
	if (state) 
	{
		state->draw();
	}
}