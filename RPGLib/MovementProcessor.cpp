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


MovementChain MovementProcessor::processHomeEndInput(int key)
{
	assert(key == CTL_HOME || key == CTL_END);
	bool movedHorz = false;
	bool movedVert = false;
	Dir horzDir;
	Dir vertDir;
	switch (key)
	{
	case CTL_HOME: //use two movements to upper left corner
		horzDir = Dir::LEFT;
		vertDir = Dir::UP;
		break;
	case CTL_END: //use two movements to lower right corner
		horzDir = Dir::RIGHT;
		vertDir = Dir::DOWN;
		break;
	}

	MovementChain movesTaken;
	Movement horzMove, vertMove;
	horzMove = processDirectionalInput(horzDir, moveControl->getTotalCols());
	vertMove = processDirectionalInput(vertDir, moveControl->getTotalRows());

	movesTaken.moves.push_back(horzMove);
	movesTaken.moves.push_back(vertMove);
	return movesTaken;
}

MovementChain MovementProcessor::processMovementInput(int input)
{
	MovementChain movesTaken;
	
	switch (input)
	{
	case CTL_HOME: 
		movesTaken = processHomeEndInput(CTL_HOME);
		break;
	case CTL_END: 
		movesTaken = processHomeEndInput(CTL_END);
		break;
	default:
	{
		Movement move = processDirectionalInput(getDirectionFromKey(input), getMoveMagnitudeFromKey(input));
		movesTaken.moves.push_back(move);
	}
		break;
	}

	return movesTaken;
}

int MovementProcessor::getMoveMagnitudeFromKey(int key)
{
	int magnitude = 0;
	int visibleRows;
	int visibleCols;
	getmaxyx(moveControl->getWindow(), visibleRows, visibleCols);

	switch (key)
	{
	case KEY_UP:
	case KEY_DOWN:
	case KEY_LEFT:
	case KEY_RIGHT: magnitude = 1;
		break;
	case KEY_PGUP://up down paging
	case KEY_PGDN: magnitude = visibleRows;
		break;
	case CTL_PGUP://left right paging
	case CTL_PGDN: magnitude = visibleCols;
		break;

	case KEY_HOME:
	case KEY_END: magnitude = moveControl->getTotalCols();
		break;
	}
	return magnitude;
}

Dir MovementProcessor::getDirectionFromKey(int key)
{
	Dir dir;
	switch (key)
	{
	case KEY_PGUP:
	case KEY_UP: dir = Dir::UP;
		break;
	case KEY_PGDN:
	case KEY_DOWN: dir = Dir::DOWN;
		break;
	case KEY_HOME:
	case CTL_PGUP:
	case KEY_LEFT: dir = Dir::LEFT;
		break;
	case KEY_END:
	case CTL_PGDN:
	case KEY_RIGHT: dir = Dir::RIGHT;
		break;
	default:
		dir = Dir::UNKNOWN;
	}
	return dir;
}

Movement MovementProcessor::processDirectionalInput(Dir dirInput, int magnitude)
{
	Movement move;
	move.dir = dirInput;
	move.magnitude = magnitude;

	processMovement(move);  //to be handled differently by concrete implementations of this abstract class
	return move;
}

void MovementProcessor::moveCursor(Movement& move)
{
	switch (move.dir)
	{
	case Dir::UP: *curY -= move.magnitude; break;
	case Dir::DOWN: *curY += move.magnitude; break;
	case Dir::LEFT: *curX -= move.magnitude; break;
	case Dir::RIGHT: *curX += move.magnitude; break;
	}
}


void MovementProcessor::reverseMovement(Movement& move)
{
	Movement reverseMove(move.dir, -move.magnitude);
	moveCursor(reverseMove);
	move.magnitude = 0; //original move could not complete
}


Boundary MovementProcessor::inBounds()
{
	if (*curX < 0) return Boundary::WEST;
	if (*curY < 0) return Boundary::NORTH;
	if ((unsigned int)*curX >= moveControl->getTotalCols()) return Boundary::EAST;
	if ((unsigned int)*curY >= moveControl->getTotalRows()) return Boundary::SOUTH;

	return Boundary::IN_BOUNDS;
}



bool MovementProcessor::inWindow()
{
	int ulY = moveControl->getUlY();
	int ulX = moveControl->getUlX();

	return *curX >= ulX && *curY >= ulY && *curX < ulX + viewWidth && *curY < ulY + viewHeight;
}

