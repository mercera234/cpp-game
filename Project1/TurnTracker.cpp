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

//
//bool TurnTracker::init()
//{
//	free();
//	
//	turnStyle = ROUND_BASED;
//	srand(time(NULL));
//	return true;
//}
//
//bool TurnTracker::free()
//{
//	turnHolder = -1;
//	//playerCount = 0; //total of all teams
//	//teamCount = 0;//?
//	//exhausted = true;
//	allPlayers.clear();
//	crossRef.clear();
//	
//	/*for (int i = 0; i < TT_CAPACITY; i++) 
//	{
//		memset(&players[i], 0, sizeof(Player));
//	}*/
//	return true;
//}

//bool TurnTracker::addPlayer(int id, int teamId, bool enabled)
//{
//	Player p;
//	p.id = id;
//	p.teamId = teamId;
//	p.turns = 1; //always 1 to start
//	p.enabled = enabled;
//	allPlayers.push_back(p);
//	
//	//playerCount++;
//	return true;
//}
//
//bool TurnTracker::enablePlayer(int id, bool enabled)
//{
//	int count = (int)allPlayers.size();
//	for (int i = 0; i < count; i++) 
//	{
//		if(allPlayers[i].id == id)
//		{
//			allPlayers[i].enabled = enabled;
//		}
//	}
//}



//int TurnTracker::getNext()
//{
//	switch (turnStyle) {
//		case ROUND_BASED:
//			
//			//check to see if any players are enabled and can take a turn
//			bool noEnabledPlayers = true;
//			int count = (int)allPlayers.size();
//			for (int i = 0; i < count; i++) 
//			{
//				if(allPlayers[i].enabled)
//				{
//					noEnabledPlayers = false;
//					break;
//				}
//			}
//			
//			if (noEnabledPlayers) 
//				return -1;
//			
//			//rebuild crossRef if necessary
//			if(crossRef.empty())//if empty, populate with all enabled players
//			{
//				populateCrossRef();
//			}
//			
//			//randomly choose from current round
//			long selection;
//			int playerNdx;
//			bool playerChosen = false;
//			while (playerChosen == false)//a player may have been disabled between turns so make sure to choose an enabled player
//			{
//				selection = rand() % crossRef.size();
//				playerNdx = crossRef[selection];
//				
//				if (allPlayers[playerNdx].enabled == false) 
//				{
//					crossRef.erase(crossRef.begin() + selection);//cross ref points to disabled player so erase it
//					if(crossRef.empty())
//					{
//						populateCrossRef();
//					}
//				}
//				else
//					playerChosen = true;
//			}
//			
//			
//			//get player id and alter stats
//			Player* p = &allPlayers[playerNdx];
//			p->turns--;
//			
//			if(p->turns <= 0)//turns should never actually dip below 0
//			{
//				//remove player from crossref
//				crossRef.erase(crossRef.begin() + selection);
//			}
//			int turnId = p->id;
//			
//			return turnId;
//		break;
//			
//	}
//}

//bool TurnTracker::populateCrossRef()
//{
//	bool populated = false;
//	int count = allPlayers.size();
//	for (int i = 0; i < count; i++) 
//	{
//		if(allPlayers[i].enabled)
//		{
//			populated = true;
//			crossRef.push_back(i);
//		}
//	}
//	return populated;
//}




