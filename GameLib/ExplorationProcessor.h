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
	Map* currMap = nullptr;
	
	bool processMovement(Movement& move);

	MapRepository* mapRepo;

	void init();
	void moveActorAcrossMapSeam(MapExit& fromMap, MapExit& toMap);
	bool bounded = true;
public:
	ExplorationProcessor();
	ExplorationProcessor(int* curY, int* curX, MapRepository* repo);
	
	void setCursor(int* curY, int* curX);
	void setMapRepo(MapRepository* repo);

	void setCurrMap(unsigned short id);
	Map* getCurrMap() { return currMap; }

	void draw();
};