#include <list>
#include "turntracker_tests.h"
#include "Actor.h"
#include "actor_helper.h"
#include "TurnTracker.h"

void turnTrackerTest()
{
	std::list<Actor*> players;
	players.push_back(loadActor("hero.actr", ActorType::HUMAN));


	std::list<Actor*> enemies;
	enemies.push_back(loadActor("toadie.actr", ActorType::CPU));
	enemies.push_back(loadActor("bigbug.actr", ActorType::CPU));
	enemies.push_back(loadActor("Skittler.actr", ActorType::CPU));
	enemies.push_back(loadActor("wispwing.actr", ActorType::CPU));

	int turns = players.size() + enemies.size();
	TurnTracker tracker;

	//tracker.addPlayers(players);
	//tracker.addPlayers(enemies);

	//int rounds = 4;
	//for (int round = 0; round < rounds; round++)
	//{
	//	for (int i = 0; i < turns; i++)
	//	{
	//		std::cout << tracker.getNext()->def->name << std::endl;
	//	}

	//	switch (round)
	//	{
	//	case 0:
	//	{
	//		Actor* runaway = enemies.back(); //remove one entirely
	//		delete runaway;
	//		enemies.pop_back();
	//	}
	//	break;
	//	case 1:
	//	{
	//		Actor* toDie = enemies.back();
	//		toDie->currHp = 0; //kill enemy
	//	}
	//	break;
	//	}



	//	system("pause");
	//}

}

void turnTrackerTest2()
{
	//std::list<MenuItem*> players;
	//
	//players.push_back(new ActorCard(createActor("hero.actr", AT_HUMAN), 0, -1));
	//

	//std::list<MenuItem*> enemies;
	//enemies.push_back(new ActorCard(createActor("toadie.actr", AT_CPU), 1, -1));
	//enemies.push_back(new ActorCard(createActor("bigbug.actr", AT_CPU), 2, -1));
	//enemies.push_back(new ActorCard(createActor("Skittler.actr", AT_CPU), 3, -1));
	//enemies.push_back(new ActorCard(createActor("wispwing.actr", AT_CPU), 4, -1));

	//int turns = players.size() + enemies.size();
	//TurnTracker tracker;

	//tracker.addPlayers(players);
	//tracker.addPlayers(enemies);

	//int rounds = 4;
	//for (int round = 0; round < rounds; round++)
	//{
	//	for (int i = 0; i < tracker.getPlayerCount(); i++)
	//	{
	//		Actor* next = tracker.getNext()->getActor();
	//		std::cout << next->def->name << std::endl;
	//	}

	//	switch (round)
	//	{
	//	case 0:
	//	{
	//		tracker.removePlayer((ActorCard*)enemies.back());
	//		enemies.pop_back();
	//	}
	//		break;
	//	case 1:
	//	{
	//		ActorCard* toDie = (ActorCard*)enemies.back();
	//		toDie->getActor()->currHp = 0; //kill enemy
	//	}	
	//		break;
	//	}
	//	system("pause");
	//}

}
