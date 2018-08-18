#pragma once
#include "Controllable.h"
#include "MovementProcessor.h"

/*
FreeMovementProcessor processes movement input for a cursor object across the area of a Controllable object.
A basic implementation of MovementProcessor
*/
class FreeMovementProcessor : public MovementProcessor
{
private:
	bool processMovement(Movement& move);
	void bringCursorInBounds(); //Brings cursor back within bounds based on last move taken
	void bringCursorInWindow(); //Reverse 'move' as the last move taken
	
public:
	FreeMovementProcessor() {}

	/*
	Controllable c should be something like an Image object, where it is laid out as a grid of tiles
	The cursor does not have to have any relation to the controllable itself.
	*/
	FreeMovementProcessor(Controllable* c, int* curY, int* curX);
};