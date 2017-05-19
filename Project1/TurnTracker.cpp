#include <iostream>
#include <ctime>
#include "TurnTracker.h"

TurnTracker::TurnTracker(int capacity)
{
	active = true;
	playerCount = 0;
	this->capacity = capacity;
	players = new Actor*[capacity];
	srand(time(NULL));
}

void TurnTracker::addPlayers(list<Actor*> joiningPlayers)
{
	for each (Actor* actor in joiningPlayers)
	{
		players[playerCount++] = actor;
	}
}

void TurnTracker::setupRound()
{
	//set up list of choices
	for (int i = 0; i < capacity; i++)
	{
		Actor* player = players[i];

		//only abled players are added to list of choices
		if (IS_ALIVE(player)) //only if player is alive (later change to can fight, not immobilized or anything like that)
		{
			choiceList.push_back(players[i]);
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

Actor* TurnTracker::getNext()
{
	if (!active) return NULL;


	Actor* next = getFromQueue();

	while (IS_DEAD(next))
	{
		turnQueue.pop(); //a disabled character cannot be used (actors can be disabled during battle)
		next = getFromQueue();
	}

	turnQueue.pop(); //remove next from queue

	return next;
}

Actor* TurnTracker::getFromQueue()
{
	if (turnQueue.empty())
	{
		setupRound();
	}

	return turnQueue.front();
}

void TurnTracker::free()
{
	delete [] players;
}

TurnTracker::~TurnTracker()
{
	free();
}

