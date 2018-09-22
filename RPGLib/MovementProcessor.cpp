#include "MovementProcessor.h"
#include "TUI.h"
#include "assert.h"

void MovementProcessor::setConvenienceVariables()
{
	if (moveControl == nullptr || moveControl->getWindow() == nullptr)
		return;

	getmaxyx(moveControl->getWindow(), viewHeight, viewWidth);
	widthCenter = viewWidth / 2;
	heightCenter = viewHeight / 2;

	oddOffsetX = 0;
	oddOffsetY = 0;

	if (viewWidth % 2 == 1)
		oddOffsetX = 1;

	if (viewHeight % 2 == 1)
		oddOffsetY = 1;
}

void MovementProcessor::setViewMode(ViewMode mode)
{
	setConvenienceVariables(); //in case window dimensions have changed since view was last set

	viewMode = mode;

	adjustView();
}

void MovementProcessor::adjustView()
{
	switch (viewMode)
	{
	case ViewMode::CENTER: //reposition control so cursor is centered
		centerView();
		break;
	case ViewMode::DYNAMIC:
		dynamicView();
		break;
		//nothing changes in lock mode so it isn't here
	}
}

void MovementProcessor::dynamicView()
{
	centerView(); //center the view first since the cursor may be off screen to start with 

	int ulY = moveControl->getUlY();
	int ulX = moveControl->getUlX();

	int newX = ulX;
	int newY = ulY;
	if (ulY < 0) newY = 0;
	else if (ulY + viewHeight >(int)moveControl->getTotalRows()) newY = moveControl->getTotalRows() - viewHeight;

	if (ulX < 0) newX = 0;
	else if (ulX + viewWidth >(int)moveControl->getTotalCols()) newX = moveControl->getTotalCols() - viewWidth;

	moveControl->setPosition(newY, newX);
}

void MovementProcessor::centerView()
{
	moveControl->setPosition(*curY - heightCenter, *curX - widthCenter);
}


void MovementProcessor::processHomeEndInput(int key)
{
	assert(key == CTL_HOME || key == CTL_END);

	int horzKey;
	int vertKey;
	switch (key)
	{
	case CTL_HOME: //use two movements to upper left corner
		horzKey = KEY_HOME;
		vertKey = KEY_SHOME;
		break;
	case CTL_END: //use two movements to lower right corner
		horzKey = KEY_END;
		vertKey = KEY_SEND;
		break;
	}

	processDirectionalInput(getAxisFromKey(horzKey), getMagnitudeFromKey(horzKey, moveControl));
	processDirectionalInput(getAxisFromKey(vertKey), getMagnitudeFromKey(vertKey, moveControl));
}

std::vector<Movement>& MovementProcessor::processMovementInput(int input)
{
	movementChain.clear();
	
	switch (input)
	{
	case CTL_HOME: 
		processHomeEndInput(CTL_HOME);
		break;
	case CTL_END: 
		processHomeEndInput(CTL_END);
		break;
	default:
	{
		processDirectionalInput(getAxisFromKey(input), getMagnitudeFromKey(input, moveControl));
	}
		break;
	}

	return movementChain;
}

void MovementProcessor::processDirectionalInput(Axis axis, int magnitude)
{
	currMove.axis = axis;
	currMove.magnitude = magnitude;

	processMovement();  //to be handled differently by concrete implementations of this abstract class
	movementChain.push_back(currMove);
}

void MovementProcessor::moveCursor()
{
	if(currMove.magnitude != 0)
		movementChain.push_back(currMove);

	switch (currMove.axis)
	{
	case Axis::VERTICAL: *curY += currMove.magnitude; break;
	case Axis::HORIZONTAL: *curX += currMove.magnitude; break;
	}
}


void MovementProcessor::reverseMovement()
{
	movementChain.pop_back(); //remove last move taken
	currMove.magnitude = -currMove.magnitude;
	moveCursor();
}


Boundary MovementProcessor::inBounds()
{
	if (*curX < 0) return Boundary::WEST;
	if (*curY < 0) return Boundary::NORTH;
	if (*curX >= moveControl->getTotalCols()) return Boundary::EAST;
	if (*curY >= moveControl->getTotalRows()) return Boundary::SOUTH;

	return Boundary::IN_BOUNDS;
}


bool MovementProcessor::inWindow()
{
	int ulY = moveControl->getUlY();
	int ulX = moveControl->getUlX();

	return *curX >= ulX && *curY >= ulY && *curX < ulX + viewWidth && *curY < ulY + viewHeight;
}

