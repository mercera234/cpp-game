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
	
	
	
	battleProcessor.setWindow(win);
	
}

void BattleState::initDefaults()
{
	battleProcessor.setResourceManager(resourceManager);
}

void BattleState::loadState()
{
	std::list<Actor*> players = { resourceManager->playerParty.begin(), resourceManager->playerParty.end() };
	
	//TODO enemies should be loaded from a pool or something
	e1 = resourceManager->actors.find("Wispwing")->second; //retrieve a copy of the repository enemy
	e1.type = ActorType::CPU;

	std::list<Actor*> enemies;
	enemies.push_back(&e1);

	battleProcessor.addParticipants(players, enemies);
	battleProcessor.begin();
}

void BattleState::unloadState()
{
	battleProcessor.clearParticipants();
}

void BattleState::processInput(GameStateManager& manager, int input)
{
	if(battleProcessor.processInput(input) == ExitCode::GO_BACK)
		manager.setState(ExploreState::getInstance());
}


void BattleState::draw()
{
	werase(win);
	wnoutrefresh(win);
	battleProcessor.draw();
}