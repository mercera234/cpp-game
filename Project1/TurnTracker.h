#pragma once

#include <list>
#include <vector>
#include <queue>
using namespace std;
#include "Actor.h"

//struct Player
//{
////	int id;
//	Actor* actor;
//	int teamId;
//	int turns;
//	bool enabled;
//};

#define TT_CAPACITY 30

//enum 
//{
//	TEAM_BASED = 1,
//	ROUND_BASED
//};

class TurnTracker
{
private:
	int capacity;
	int playerCount; //total of all teams
	int active; //whether turns are being tracked or not
	Actor** players;
	vector<Actor*> choiceList;
	queue<Actor*> turnQueue;
	Actor* getFromQueue();
public:
	TurnTracker(int capacity);
	void addPlayers(list<Actor*> players);
	void setupRound();
	Actor* getNext();
	void free();
	~TurnTracker();

	void setActive(bool status) { active = status; }
	//bool getActive() { return active; }
	int getRoundTurns() { return turnQueue.size(); }
};



