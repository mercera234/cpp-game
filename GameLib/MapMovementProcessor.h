#pragma once
#include "MovementProcessor.h"
#include "MegaMap.h"

class MapMovementProcessor : public MovementProcessor
{
private:
	bool processStep2(short* axis, int step, int dirInput);
	MegaMap* megaMap;
public:
	MapMovementProcessor(MegaMap* megaMap, short* curY, short* curX);
};

