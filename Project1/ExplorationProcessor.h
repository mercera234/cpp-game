#pragma once
#include "MovementProcessor.h"
#include <map>
#include <functional>
#include "MapExit.h"
#include "Map.h"
#include "MapRepository.h"

class ExplorationProcessor : public MovementProcessor
{
private:
	Map* currMap;
	bool processStep(short* axis, int step, int dirInput);
	bool processStep2(short* axis, int step, int dirInput);

	MapRepository* mapRepo;

	void moveActorAcrossMapSeam(MapExit& fromMap, MapExit& toMap);

public:
	ExplorationProcessor(short* curY, short* curX, MapRepository* repo);
	
	void setCurrMap(unsigned short id);
	Map* getCurrMap() { return currMap; }
};