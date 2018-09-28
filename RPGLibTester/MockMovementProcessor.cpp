#include "MockMovementProcessor.h"

bool MockMovementProcessor::processMovement()
{
	int *axis;

	axis = currMove.axis == Axis::HORIZONTAL ? curX : curY;

	*axis += currMove.magnitude;
	return true;
}