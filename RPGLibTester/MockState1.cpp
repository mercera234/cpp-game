#include "MockState1.h"
#include "MockState2.h"
#include "GameStateManager.h"

GameState* MockState1::instance = nullptr;

GameState* MockState1::getInstance()
{
	if (instance == nullptr)
		instance = new MockState1;

	return instance;
}

MockState1::MockState1()
{ 
	
}


void MockState1::processInput(GameStateManager& manager, int input)
{
	if (input == 2)
	{
		manager.setState(MockState2::getInstance());
	}
		
}
