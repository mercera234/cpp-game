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
	bool processMovement(Movement& move);

	MapRepository* mapRepo;

	void moveActorAcrossMapSeam(MapExit& fromMap, MapExit& toMap);
	bool bounded = true;
public:
	ExplorationProcessor(int* curY, int* curX, MapRepository* repo);
	
	void setCurrMap(unsigned short id);
	Map* getCurrMap() { return currMap; }
};