#include "BattleState.h"
#include "ExploreState.h"
#include "GameInput.h"
#include "GameStateManager.h"
#include "actor_helper.h"

GameState* BattleState::instance = nullptr;

GameState* BattleState::getInstance() //since static, cannot be virtual in super class
{
	if (instance == nullptr)
		instance = new BattleState;

	return instance;
}

BattleState::BattleState()
{
	win = newwin(screenHeight, screenWidth, 0, 0);
	
	std::list<Actor*> players;
	std::list<Actor*> enemies;
	
	battleProcessor.setWindow(win);
	battleProcessor.addParticipants(players, enemies);
}

void BattleState::processInput(GameStateManager& manager, int input)
{
	//temporary fix for how battle processor handles input
	/*switch (input)
	{
	case GameInput::DOWN_INPUT: input = KEY_DOWN; break;
	case GameInput::UP_INPUT: input = KEY_UP; break;
	case GameInput::LEFT_INPUT: input = KEY_LEFT; break;
	case GameInput::RIGHT_INPUT: input = KEY_RIGHT; break;
	case GameInput::OK_INPUT: input = 'c'; break;
	case GameInput::CANCEL_INPUT: input = 'x'; break;
	default: break;
	}*/

	if(battleProcessor.processInput(input) == ExitCode::GO_BACK)
		manager.setState(ExploreState::getInstance());
}


void BattleState::draw()
{
	werase(win);
	wnoutrefresh(win);
	battleProcessor.draw();
}