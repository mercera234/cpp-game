#include "MockMovementProcessor.h"

bool MockMovementProcessor::processMovement(Movement& move)
{
	int *axis;
	switch (move.dir)
	{
	case Dir::UP:
	case Dir::DOWN: axis = curY; break;
	case Dir::LEFT:
	case Dir::RIGHT: axis = curX; break;
	}


	*axis += move.magnitude;
	return true;
}