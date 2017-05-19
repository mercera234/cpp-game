#include "SimpleMapMovementProcessor.h"

int theHasher(MapOpening* key)
{
	return key->mapId; //worst hashing algorithm ever!
}

bool theKey(MapOpening* first, MapOpening* second)
{
	return (first->mapId == second->mapId && first->location == second->location);
}


SimpleMapMovementProcessor::SimpleMapMovementProcessor(short * curY, short * curX) : exits((size_t)2, theHasher, theKey)
{
	this->currMap = NULL;
	
	this->curY = curY;
	this->curX = curX;

	bounded = false;
}

void SimpleMapMovementProcessor::addMap(Map& map)
{
	pair<unsigned short, Map*> aMap(map.getId(), &map);
	maps.insert(aMap);

	if (currMap == NULL)
		setCurrMap(map.getId());
}



void SimpleMapMovementProcessor::setCurrMap(unsigned short id)
{
	unordered_map<unsigned short, Map*>::iterator it;
	it = maps.find(id);

	currMap = &(*it->second);

	moveControl = currMap->getDisplay();
	setConvenienceVariables();

	setViewMode(VM_DYNAMIC);
}

void SimpleMapMovementProcessor::addOpening(MapOpening& source, MapOpening& dest)
{
	pair<MapOpening*, MapOpening*> oneWayMapConnection(&source, &dest);
	exits.insert(oneWayMapConnection);
}

bool SimpleMapMovementProcessor::processStep(short * axis, int step, int dirInput)
{
	//move cursor by step
	*axis += step;

	//verify that cursor is within bounds
	int edge = inBounds();
	if (edge != IN_BOUNDS) //walked off edge
	{
		//using the edge and current map id, search for exit for this map
		MapOpening opening;
		opening.mapId = currMap->getId();
		opening.location = edge;

		unordered_map<MapOpening*, MapOpening*>::iterator it;
		it = exits.find(&opening);

		if (it == exits.end()) //no opening found, reverse the step
		{
			*axis -= step; 
			return false;
		}

		MapOpening dest = *it->second;

		switch (dest.location)
		{
		case B_WEST: *curX = 0; break;
		case B_EAST: *curX = moveControl->getTotalRows() - 1; break;
		case B_NORTH: *curY = 0; break;
		case B_SOUTH: *curY = moveControl->getTotalCols() - 1; break;
		default:
			break;
		}
		setCurrMap(dest.mapId);
		return true;
	}

	adjustDynamicView(step, dirInput);
		
	return true;
}

