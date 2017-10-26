#include <list>
#include "turntracker_tests.h"
#include "Actor.h"
#include "TurnTracker.h"

//these top 2 methods should go in a different file
Actor* buildActorFromDef(ActorDef* def, int type)
{
	Actor* actor = new Actor();
	actor->def = def;
	actor->currHp = def->maxHp;
	actor->currMp = def->maxMp;
	actor->defIndex = 1;
	actor->prevX = -1;
	actor->prevY = -1;
	actor->x = 30;
	actor->y = 5;
	actor->type = type;
	return actor;
}

Actor* createActor(std::string filename, int type)
{
	ActorDef* def = new ActorDef();

	std::ifstream is;
	is.open("data\\" + filename, std::ios::binary);

	def->load(is);

	is.close();

	return buildActorFromDef(def, type);
}

void turnTrackerTest()
{
	std::list<Actor*> players;
	players.push_back(createActor("hero.actr", AT_HUMAN));


	std::list<Actor*> enemies;
	enemies.push_back(createActor("toadie.actr", AT_CPU));
	enemies.push_back(createActor("bigbug.actr", AT_CPU));
	enemies.push_back(createActor("Skittler.actr", AT_CPU));
	enemies.push_back(createActor("wispwing.actr", AT_CPU));

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
