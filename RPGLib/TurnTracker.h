#pragma once

#include <list>
#include <vector>
#include <queue>
#include "Player.h"

/*
A class that manages turn-taking for different players in a game.
*/
class TurnTracker
{
private:
	bool active; //whether turns are being tracked or not
	std::vector<Player*> players; //all players participating
	std::vector<Player*> sideline; //all players that can't participate in round
	std::queue<Player*> turnQueue; //order of turns
	Player* getFromQueue();
	void setupRound();
	void refreshQueue();
public:
	TurnTracker();
	void addPlayer(Player* player);
	void addPlayers(std::list<Player*> players);
	Player* getNext(); 
	void free();
	~TurnTracker();

	//getters/setters
	int getPlayerCount() { return players.size(); }
	void setActive(bool status) { active = status; }
	bool getActive() { return active; }
	int getRoundTurns() { return turnQueue.size(); }
};



