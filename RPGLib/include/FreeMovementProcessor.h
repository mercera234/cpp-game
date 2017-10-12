#pragma once
#include "Controllable.h"
#include "MovementProcessor.h"

/*
FreeMovementProcessor processes movement input for a cursor object across the area of a Controllable object
*/
class FreeMovementProcessor : public MovementProcessor
{
private:
	bool processStep(short* axis, int step, Direction dirInput);

public:
	FreeMovementProcessor(Controllable* c, short* curY, short* curX);
};