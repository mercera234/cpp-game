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
	
	//for now we are adding stub characters, later we should add the potential for a null fight
	Actor* p1;
	Actor* e1;

	std::list<Actor*> players;

	p1 = buildActor(ActorType::HUMAN);
	p1->stats.strength.setCurr(10);
	p1->stats.agility.setCurr(10);
	p1->name = "Hero";

	players.push_back(p1);

	std::list<Actor*> enemies;
	e1 = buildActor(ActorType::CPU);
	e1->stats.strength.setCurr(1);
	e1->stats.agility.setCurr(40);
	e1->stats.exp.setCurr(1);
	e1->money.setCurr(1);
	e1->name = "Toadie";
	enemies.push_back(e1);

	
	battleProcessor.setWindow(win);
	battleProcessor.addParticipants(players, enemies);
}

void BattleState::processInput(GameStateManager& manager, int input)
{
	//temporary fix for how battle processor handles input
	switch (input)
	{
	case GameInput::DOWN_INPUT: input = KEY_DOWN; break;
	case GameInput::UP_INPUT: input = KEY_UP; break;
	case GameInput::LEFT_INPUT: input = KEY_LEFT; break;
	case GameInput::RIGHT_INPUT: input = KEY_RIGHT; break;
	case GameInput::OK_INPUT: input = 'c'; break;
	case GameInput::CANCEL_INPUT: input = 'x'; break;
	default: break;
	}

	if(battleProcessor.processInput(input) == false)
		manager.setState(ExploreState::getInstance());
}


void BattleState::draw()
{
	werase(win);
	//mvwaddstr(win, 0, 0, "BATTLE STATE");
	wnoutrefresh(win);
	battleProcessor.draw();
}