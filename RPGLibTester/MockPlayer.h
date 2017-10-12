#pragma once
#include "Player.h"

class MockPlayer : public Player
{
private:
public:
	int id;
	bool mockOn = true;
	bool canPlay() { return mockOn; }

};