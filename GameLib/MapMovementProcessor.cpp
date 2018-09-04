#include "MapMovementProcessor.h"

MapMovementProcessor::MapMovementProcessor(MegaMap * megaMap, short * curY, short * curX)
{
	this->megaMap = megaMap;

	this->moveControl = megaMap->getCurrMap()->getDisplay();
	this->curY = curY;
	this->curX = curX;

	

	bounded = true;

	//set convenience variables
	setConvenienceVariables();

	setViewMode(VM_DYNAMIC);
}


bool MapMovementProcessor::processStep2(short * axis, int step, int dirInput)
{
	//move cursor by step
	*axis += step;

	//check if character walked off the map
	if (inBounds() == false)
	{
		//check if character walked onto another map

		//! only doing right walkoff for now
		if (*curX >= moveControl->getTotalCols())
		{
			//determine new potential high level coordinates
			int hX = megaMap->blockX + 1;

			//check if map is there
			Map* m = megaMap->findMap(hX, megaMap->blockY);

			if (m != NULL)// map was found
			{
				megaMap->setCurrMap(m);
				this->moveControl = megaMap->getCurrMap()->getDisplay();
				//set new coordinates for megaMap
				megaMap->blockX = hX;
				
				//set new coordinates for cursor
				*axis -= megaMap->unitWidth;
			}
		}
	}
	else
	{
		//else character is still in map so process the shift normally 

		//only dynamic view is handled for maps
		//shift control only if not too close to edge of control
		switch (dirInput)
		{
		case DIR_RIGHT:
			if (*curX > widthCenter && *curX <= moveControl->getTotalCols() - widthCenter - oddOffsetX)
				moveControl->shift(0, step);
			break;
		case DIR_LEFT:
			if (*curX >= widthCenter && *curX < moveControl->getTotalCols() - widthCenter - oddOffsetX)
				moveControl->shift(0, step);
			break;
		case DIR_DOWN:
			if (*curY > heightCenter && *curY <= moveControl->getTotalRows() - heightCenter - oddOffsetY)
				moveControl->shift(step, 0);
			break;
		case DIR_UP:
			if (*curY >= heightCenter && *curY < moveControl->getTotalRows() - heightCenter - oddOffsetY)
				moveControl->shift(step, 0);
			break;
		}

	}
	
	return true;
}

