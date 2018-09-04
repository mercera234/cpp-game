#include "MapRepository.h"
#include "MovementProcessor.h"
#include "mapKeyComparators.h"

MapRepository::MapRepository() : exits(mapExitKeyCompare)
{
	this->unitMapHeight = 64;
	this->unitMapWidth = 64;
}


MapRepository::MapRepository(unsigned short unitMapHeight, unsigned short unitMapWidth)
	: exits(mapExitKeyCompare)
{
	this->unitMapHeight = unitMapHeight;
	this->unitMapWidth = unitMapWidth;
}


void MapRepository::addExit(MapExit* source, MapExit* dest)
{
	exits.insert(std::make_pair(source, dest));
}

/*
Add 2 exits between maps effectively joining the two
*/
void MapRepository::addMapSeam(Map& negMap, Map& posMap, //negative map is either N or W, positive is S or E
	Axis axis,
	unsigned short negUnit, unsigned short posUnit, //the unit portion of the map to join together
	unsigned short length) //not used just yet
{
	MapExit* negExit = new MapExit();
	MapExit* posExit = new MapExit();

	negExit->mapId = negMap.getId();
	negExit->edge = axis == Axis::HORIZONTAL ? Boundary::EAST : Boundary::SOUTH;
	negExit->unit = negUnit;

	posExit->mapId = posMap.getId();
	posExit->edge = axis == Axis::HORIZONTAL ? Boundary::WEST : Boundary::NORTH;
	posExit->unit = posUnit;

	addExit(negExit, posExit);
	addExit(posExit, negExit);
}


MapExit* MapRepository::getExit(MapExit* sourceExit)
{
	std::map<MapExit*, MapExit*>::iterator it;
	it = exits.find(sourceExit);

	return it == exits.end() ? nullptr : &(*it->second);
}

void MapRepository::add(Map& obj) //add one object of type T
{
	std::pair<unsigned short, Map*> aMap(obj.getId(), &obj);
	maps.insert(aMap);
}

Map* MapRepository::find(unsigned short id)
{
	std::map<unsigned short, Map*>::iterator it;
	it = maps.find(id);

	return it == maps.end() ? nullptr : &(*it->second);
}