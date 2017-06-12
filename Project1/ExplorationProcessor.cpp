#include "ExplorationProcessor.h"

ExplorationProcessor::ExplorationProcessor(short* curY, short* curX, MapRepository* repo)
{
	this->curY = curY;
	this->curX = curX;
	mapRepo = repo;

	this->currMap = NULL;
	bounded = false;
}


void ExplorationProcessor::setCurrMap(unsigned short id)
{
	currMap = mapRepo->find(id);
	moveControl = currMap->getDisplay();
}


bool ExplorationProcessor::processStep2(short * axis, int step, int dirInput)
{
	//move cursor by step
	*axis += step;

	//verify that cursor is within bounds
	int edge = inBounds();
	if (edge != IN_BOUNDS) //walked off edge
	{
		//search for exit for this map using the mapId, edge, and unit
		
		MapExit* sourceExit = new MapExit();
		sourceExit->mapId = currMap->getId();
		sourceExit->edge = edge;

		short* perpAxis = (axis == curX) ? curY : curX;
		int unitMapSize = (axis == curX) ? mapRepo->getUnitMapHeight() : mapRepo->getUnitMapWidth();
		sourceExit->unit = *perpAxis / unitMapSize;

		MapExit* destExit = mapRepo->getExit(sourceExit);

		if(destExit == NULL) //no opening found, reverse the step
		{
			*axis -= step; 
			return false;
		}

		moveActorAcrossMapSeam(*sourceExit, *destExit); //it->second is destination map
		return true;
	}

	adjustDynamicView(step, dirInput);
		
	return true;
}

bool ExplorationProcessor::processStep(short * axis, int step, int dirInput)
{
	//move cursor by step
	*axis += step;

	//verify that cursor is within bounds
	int edge = inBounds();
	if (edge != IN_BOUNDS) //walked off edge
	{
		//search for exit for this map using the mapId, edge, and unit

		MapExit* sourceExit = new MapExit();
		sourceExit->mapId = currMap->getId();
		sourceExit->edge = edge;

		short* perpAxis = (axis == curX) ? curY : curX;
		int unitMapSize = (axis == curX) ? mapRepo->getUnitMapHeight() : mapRepo->getUnitMapWidth();
		sourceExit->unit = *perpAxis / unitMapSize;

		MapExit* destExit = mapRepo->getExit(sourceExit);

		if (destExit == NULL) //no opening found, reverse the step
		{
			*axis -= step;
			return false;
		}

		moveActorAcrossMapSeam(*sourceExit, *destExit); //it->second is destination map
		return true;
	}

	adjustDynamicView(step, dirInput);

	return true;
}


void ExplorationProcessor::moveActorAcrossMapSeam(MapExit& fromMap, MapExit& toMap)
{
	Actor* controlActor = currMap->controlActor; //save controlActor

	setCurrMap(toMap.mapId);

	int unitMapSize = (toMap.edge == B_WEST || toMap.edge == B_EAST ) ? mapRepo->getUnitMapHeight() : mapRepo->getUnitMapWidth();

	int unitMultiplier = fromMap.unit > toMap.unit ? -unitMapSize : unitMapSize;
	int unitOffset = abs(fromMap.unit - toMap.unit) * unitMultiplier;

	switch (toMap.edge)
	{
	case B_WEST:
		*curX = 0;
		*curY += unitOffset;
		break; //others below not done yet
	case B_EAST:
		*curX = moveControl->getTotalCols() - 1;
		*curY += unitOffset;
		break;
	case B_NORTH:
		*curY = 0;
		*curX += unitOffset;
		break;
	case B_SOUTH:
		*curY = moveControl->getTotalRows() - 1;
		*curX += unitOffset;
		break;
	default:
		break;
	}

	setViewMode(VM_DYNAMIC);
	currMap->controlActor = controlActor; //make sure new map has the same controlActor
}


