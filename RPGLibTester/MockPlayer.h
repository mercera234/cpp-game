#pragma once
#include "Player.h"

class MockPlayer : public Player
{
private:
public:
	int id;
	bool playStatus = true;
	bool canPlay() { return playStatus; }

};