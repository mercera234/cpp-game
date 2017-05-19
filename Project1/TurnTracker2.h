#pragma once

#include <list>
#include <vector>
#include <queue>
using namespace std;
#include "ActorCard.h"

#define TT_CAPACITY 30

class TurnTracker
{
private:
	int active; //whether turns are being tracked or not
	list<MenuItem*> players; //all players participating
	vector<ActorCard*> choiceList; //all players that can take a turn for the next round
	queue<ActorCard*> turnQueue; //order of turns
	ActorCard* getFromQueue();
public:
	TurnTracker();
	void addPlayers(list<MenuItem*> players);
	void removePlayer(ActorCard* player);
	void setupRound();
	ActorCard* getNext();
	void free();
	~TurnTracker();

	int getPlayerCount() { return players.size(); }
	void setActive(bool status) { active = status; }
	//bool getActive() { return active; }
	int getRoundTurns() { return turnQueue.size(); }
};



