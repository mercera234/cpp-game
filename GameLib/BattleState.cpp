#include "BattleState.h"
#include "ExploreState.h"
#include "TitleScreenState.h"
#include "GameInput.h"
#include "GameStateManager.h"
#include "actor_helper.h"
#include "EnemyPool.h"
#include "defaults.h"

GameState* BattleState::instance = nullptr;

GameState* BattleState::getInstance() //since static, cannot be virtual in super class
{
	if (instance == nullptr)
		instance = new BattleState;

	return instance;
}

BattleState::BattleState()
{
	battleProcessor.setWindow(newwin(gameScreenHeight, gameScreenWidth, 0, 0));	
}

void BattleState::initDefaults()
{
	battleProcessor.setResourceManager(resourceManager);
}

void BattleState::loadState()
{
	std::list<Actor*> players = { resourceManager->playerParty.begin(), resourceManager->playerParty.end() };
	
	//TODO pool is being set manually right now, should be read in from file
	EnemyPool pool;
	EnemyGroup group1;
	group1.weight = 1;
	group1.enemyNames.push_back("Toadie");
	group1.enemyNames.push_back("Toadie");

	EnemyGroup group2;
	group2.weight = 1;
	group2.enemyNames.push_back("Toadie");
	group2.enemyNames.push_back("Skittler");

	std::vector<EnemyGroup> groups;
	groups.push_back(group1);
	groups.push_back(group2);

	pool.setGroups(groups);

	EnemyGroup randomGroup = pool.getRandomGroup();

	std::list<Actor*> enemyPtrs;
	for each (std::string name in randomGroup.enemyNames)
	{
		Actor e = resourceManager->actors.find(name)->second; //get copy of repository enemy
		e.type = ActorType::CPU;
		enemies.push_back(e);
		enemyPtrs.push_back(&enemies.back());
	}

	battleProcessor.addParticipants(players, enemyPtrs);
	battleProcessor.begin();
}

void BattleState::unloadState()
{
	battleProcessor.clearParticipants();
}

void BattleState::processInput(GameStateManager& manager, int input)
{
	int exitCode = battleProcessor.processInput(input);

	switch (exitCode)
	{
	case ExitCode::GO_BACK: manager.setState(ExploreState::getInstance()); break;
	case ExitCode::QUIT_TO_TITLE: 
		manager.setState(TitleScreenState::getInstance()); 
		break;
	}	
}


void BattleState::draw()
{
	battleProcessor.draw();
}