#include "FreeMovementProcessor.h"
#include "TUI.h" 

/*
Controllable c should be something like an Image object, where is like a grid of tiles
The cursor does not have to have any relation to the controllable itself.
*/
FreeMovementProcessor::FreeMovementProcessor(Controllable* c, short* curY, short* curX)
{
	this->moveControl = c;
	this->curY = curY;
	this->curX = curX;

	bounded = true;

	setViewMode(VM_CENTER);
}



/*
return true if step was successful
*/
bool FreeMovementProcessor::processStep(short* axis, int step, int dirInput)
{
	//move cursor by step
	*axis += step;

	//verify that cursor is within bounds
	if (bounded)
	{
		if (inBounds() != IN_BOUNDS)
		{
			*axis -= step; 
			return false; //reverse step and quit
		}
	}


	switch (viewMode)
	{
	case VM_LOCK:
		//just make sure step doesn't go out of window
	{
		if (inWindow() == false)
		{
			*axis -= step; 
			return false;//reverse step and quit
		}
	}
	break;
	case VM_CENTER: //shift control along the appropriate axis
		axis == curY ? moveControl->shift(step, 0) : moveControl->shift(0, step);
		break;
	case VM_DYNAMIC:
	{
		//shift control only if not too close to edge of control
		adjustDynamicView(step, dirInput);
	}
	break;
	}

	return true;
}