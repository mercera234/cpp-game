#include "MockState1.h"
#include "MockState2.h"
#include "GameStateManager.h"

GameState* MockState2::instance = nullptr;

GameState* MockState2::getInstance()
{
	if (instance == nullptr)
		instance = new MockState2;

	return instance;
}

MockState2::MockState2()
{
	setName("MockState2");
}

void MockState2::processInput(GameStateManager& manager, int input)
{
	if (input == 1)
	{
		manager.setState(MockState1::getInstance());
	}
}
