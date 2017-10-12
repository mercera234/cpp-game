#pragma once
#include <string>

/*
A participant in a game and is capable of taking turns.
*/
class Player
{
protected:
	unsigned short turnCount;
public:
	virtual bool canPlay() = 0; 

	//getters/setters
	void setTurnCount(unsigned short turns) { turnCount = turns; }
	unsigned short getTurnCount() { return turnCount; }
	void incrementTurns() { turnCount++; }
	void decrementTurns() { turnCount--; }
};
