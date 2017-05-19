#include <iostream>
#include <ctime>
#include "TurnTracker2.h"

TurnTracker::TurnTracker()
{
	active = true;
	srand(time(NULL));
}

void TurnTracker::addPlayers(list<MenuItem*> joiningPlayers)
{
	for each (MenuItem* actor in joiningPlayers)
	{
		players.push_back(actor);
	}
}

void TurnTracker::removePlayer(ActorCard* player)
{
	players.remove(player);
}

void TurnTracker::setupRound()
{
	//set up list of choices
	for (list<MenuItem*>::iterator it = players.begin(); it != players.end(); it++)
	{
		ActorCard* player = (ActorCard*)*it;

		//only abled players are added to list of choices
		if (IS_ALIVE(player->getActor())) //only if player is alive (later change to can fight, not immobilized or anything like that)
		{
			choiceList.push_back(player);
		}
	}

	//assign choices to turn queue
	//for now this is being done randomly, but I may switch to using characters agility at some point
	int initialSize = choiceList.size();
	for (int i = 0; i < initialSize; i++)
	{
		int selection = rand() % choiceList.size();

		turnQueue.push(choiceList[selection]);

		choiceList.erase(choiceList.begin() + selection); //remove selection from list of choices
	}
}

ActorCard* TurnTracker::getNext()
{
	if (!active) return NULL;


	ActorCard* next = getFromQueue();

	while (IS_DEAD(next->getActor()))
	{
		turnQueue.pop(); //a disabled character cannot be used (actors can be disabled during battle)
		next = getFromQueue();
	}

	turnQueue.pop(); //remove next from queue

	return next;
}

ActorCard* TurnTracker::getFromQueue()
{
	if (turnQueue.empty())
	{
		setupRound();
	}

	return turnQueue.front();
}

void TurnTracker::free()
{
	players.clear();
}

TurnTracker::~TurnTracker()
{
	free();
}

