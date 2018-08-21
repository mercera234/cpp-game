#include "GameStateManager.h"
#include "curses.h"

void GameStateManager::setState(GameState* newState)
{
	//unload old state
	if(state != nullptr)
		state->unloadState();

	state = newState;
	//load new state
	if (state != nullptr)
	{
		state->loadState();
		state->setManager(this);
	}
		
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