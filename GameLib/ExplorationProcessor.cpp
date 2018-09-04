#include "ExplorationProcessor.h"

ExplorationProcessor::ExplorationProcessor()
{
	init();
}

ExplorationProcessor::ExplorationProcessor(int* curY, int* curX, MapRepository* repo)
{
	setCursor(curY, curX);
	setMapRepo(repo);
	
	init();
}

void ExplorationProcessor::init()
{
	
}

void ExplorationProcessor::setCursor(int* curY, int* curX)
{
	this->curY = curY;
	this->curX = curX;
}

void ExplorationProcessor::setMapRepo(MapRepository* repo)
{
	mapRepo = repo;
}

void ExplorationProcessor::setCurrMap(unsigned short id)
{
	currMap = mapRepo->find(id);
	moveControl = currMap->getDisplay();
}


bool ExplorationProcessor::processMovement(Movement& move)
{
	//move cursor by step
	moveCursor(move);

	//verify that cursor is within bounds
	Boundary edge = inBounds();
	if (edge != Boundary::IN_BOUNDS) //walked off edge
	{
		//search for exit for this map using the mapId, edge, and unit

		MapExit* sourceExit = new MapExit();
		sourceExit->mapId = currMap->getId();
		sourceExit->edge = edge;

		Axis axis = getAxis(move.dir);

		int* perpAxis = (axis == Axis::HORIZONTAL) ? curY : curX;
		int unitMapSize = (axis == Axis::HORIZONTAL) ? mapRepo->getUnitMapHeight() : mapRepo->getUnitMapWidth();
		sourceExit->unit = *perpAxis / unitMapSize;

		MapExit* destExit = mapRepo->getExit(sourceExit);

		if (destExit == nullptr) //no opening found, reverse the step
		{
			Movement reverseMove(move.dir, -move.magnitude);
			moveCursor(reverseMove);
			return false;
		}

		moveActorAcrossMapSeam(*sourceExit, *destExit); //it->second is destination map
		return true;
	}
	else //still on same map
	{
		

	}

	adjustView();

	return true;
}


void ExplorationProcessor::moveActorAcrossMapSeam(MapExit& fromMap, MapExit& toMap)
{
	Actor* controlActor = currMap->getControlActor(); //save controlActor

	setCurrMap(toMap.mapId);

	int unitMapSize = (toMap.edge == Boundary::WEST || toMap.edge == Boundary::EAST ) ? mapRepo->getUnitMapHeight() : mapRepo->getUnitMapWidth();

	int unitMultiplier = fromMap.unit > toMap.unit ? -unitMapSize : unitMapSize;
	int unitOffset = abs(fromMap.unit - toMap.unit) * unitMultiplier;

	switch (toMap.edge)
	{
	case Boundary::WEST:
		*curX = 0;
		*curY += unitOffset;
		break; //others below not done yet
	case Boundary::EAST:
		*curX = moveControl->getTotalCols() - 1;
		*curY += unitOffset;
		break;
	case Boundary::NORTH:
		*curY = 0;
		*curX += unitOffset;
		break;
	case Boundary::SOUTH:
		*curY = moveControl->getTotalRows() - 1;
		*curX += unitOffset;
		break;
	default:
		break;
	}

	setViewMode(ViewMode::DYNAMIC);
	currMap->setControlActor(controlActor); //make sure new map has the same controlActor
}


void ExplorationProcessor::draw()
{
	currMap->draw();
}