#pragma once
#include "MovementProcessor.h"

class MockMovementProcessor : public MovementProcessor
{
private:
public:
	bool processMovement(Movement& move);
};