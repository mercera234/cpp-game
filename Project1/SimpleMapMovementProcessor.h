#pragma once
#include "MovementProcessor.h"
#include <unordered_map>
#include <functional>
using namespace std;
#include "MapExit.h"
#include "Map.h"


int theHasher(MapOpening* key);
bool theKey(MapOpening* first, MapOpening* second);

class SimpleMapMovementProcessor : public MovementProcessor
{
private:
	Map* currMap;
	bool processStep(short* axis, int step, int dirInput);

	unordered_map<MapOpening*, MapOpening*, function<int(MapOpening*)>, function<bool(MapOpening*, MapOpening*)>> exits;// (2, theHasher, theKey);
	//need a mechanism for loading new maps
	unordered_map<unsigned short, Map*> maps;
public:
	SimpleMapMovementProcessor(short* curY, short* curX);
	void addOpening(MapOpening& source, MapOpening& dest);
	void addMap(Map& map);
	void setCurrMap(unsigned short id);
	Map* getCurrMap() { return currMap; }
};