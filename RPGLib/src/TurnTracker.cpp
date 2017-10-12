#include <iostream>
#include <ctime>
#include <algorithm>
#include "TurnTracker.h"

TurnTracker::TurnTracker()
{
	active = true;
	srand((unsigned int)time(NULL));
	players.reserve(16); //default is 16 which is probably twice the amount of turns needed
	sideline.reserve(8);
}

void TurnTracker::addPlayer(Player* player)
{
	players.push_back(player);
}

void TurnTracker::addPlayers(std::list<Player*> joiningPlayers)
{
	for each (Player* player in joiningPlayers)
	{
		players.push_back(player);
	}
}


void TurnTracker::setupRound()
{
	//copy players to a second vector that will be shuffled
	std::vector<Player*> choiceList; 
	choiceList.reserve(players.size()); 

	//set up list of choices
	for (auto it = players.begin(); it != players.end(); it++)
	{
		Player* player = (Player*)*it;
		player->setTurnCount(1);

		//only abled players are added to list of choices
		if (player->canPlay())
		{
			choiceList.push_back(player);
		}
		else
			sideline.push_back(player);
	}



	//for now this is being done randomly, but I may switch to using characters agility at some point
	std::random_shuffle(choiceList.begin(), choiceList.end());

	//assign choices to turn queue
	std::for_each(choiceList.begin(), choiceList.end(), [&](Player* player) {turnQueue.push(player); });
}

Player* TurnTracker::getNext()
{
	if (!active) return nullptr;

	refreshQueue();

	Player* next = getFromQueue();
	if (next == nullptr)
		return next;

	while (next->canPlay() == false)
	{
		turnQueue.pop(); //a disabled character cannot be used (actors can be disabled during battle)
		next = getFromQueue();
	}

	turnQueue.pop(); //remove next from queue

	next->decrementTurns();
	return next;
}


void TurnTracker::refreshQueue()
{
	//check the sideline if any players are can play and have a turnleft
	for each (Player* player in sideline)
	{
		if (player->getTurnCount() > 0)
		{
			if (player->canPlay()) //add player to queue if they still have a turn and can play
				turnQueue.push(player);
		}
	}
}


Player* TurnTracker::getFromQueue()
{
	if (turnQueue.empty())
	{
		setupRound();
	}

	if (turnQueue.empty()) //still empty after setting up round
		return nullptr;

	return turnQueue.front();
}

void TurnTracker::free()
{
	players.clear();
	
	std::queue<Player*>().swap(turnQueue);
}

TurnTracker::~TurnTracker()
{
	free();
}

