#include "MapRepository.h"
#include "MovementProcessor.h"

bool keyCompare(MapExit* first, MapExit* second)
{
	if (first->mapId > second->mapId)
		return false;

	if (first->mapId < second->mapId)
		return true;

	if (first->edge > second->edge)
		return false;

	if (first->edge < second->edge)
		return true;

	return (first->unit < second->unit);
}

MapRepository::MapRepository(unsigned short unitMapHeight, unsigned short unitMapWidth)
	: exits(keyCompare)
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
	bool xAxis, //true if seam is between West and East maps
	unsigned short negUnit, unsigned short posUnit, //the unit portion of the map to join together
	unsigned short length) //not used just yet
{
	MapExit* negExit = new MapExit();
	MapExit* posExit = new MapExit();

	negExit->mapId = negMap.getId();
	negExit->edge = xAxis ? B_EAST : B_SOUTH;
	negExit->unit = negUnit;

	posExit->mapId = posMap.getId();
	posExit->edge = xAxis ? B_WEST : B_NORTH;
	posExit->unit = posUnit;

	addExit(negExit, posExit);
	addExit(posExit, negExit);
}


MapExit* MapRepository::getExit(MapExit* sourceExit)
{
	std::map<MapExit*, MapExit*>::iterator it;
	it = exits.find(sourceExit);

	return it == exits.end() ? NULL : &(*it->second);
}

void MapRepository::add(Map& obj) //add one object of type T
{
	pair<unsigned short, Map*> aMap(obj.getId(), &obj);
	maps.insert(aMap);
}

Map* MapRepository::find(unsigned short id)
{
	std::map<unsigned short, Map*>::iterator it;
	it = maps.find(id);

	return it == maps.end() ? NULL : &(*it->second);
}