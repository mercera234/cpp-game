#include "MovementProcessor.h"
#include "TUI.h"

void MovementProcessor::setConvenienceVariables()
{
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

void MovementProcessor::setViewMode(int mode)
{
	setConvenienceVariables(); //in case window dimensions have changed since view was last set

	viewMode = mode;

	switch (viewMode)
	{
	case VM_CENTER: //reposition control so cursor is centered
		//c->setPosition(*curY - heightCenter, *curX - widthCenter);
		centerView();
		break;
	case VM_DYNAMIC:
	{
		centerView(); //center the view first since the cursor may be off screen to start with 

		int ulY = moveControl->getUlY();
		int ulX = moveControl->getUlX();


		int newX = ulX;
		int newY = ulY;
		if (ulY < 0) newY = 0;
		else if (ulY + viewHeight > moveControl->getTotalRows()) newY = moveControl->getTotalRows() - viewHeight;

		if (ulX < 0) newX = 0;
		else if (ulX + viewWidth > moveControl->getTotalCols()) newX = moveControl->getTotalCols() - viewWidth;

		moveControl->setPosition(newY, newX);
	}
	//nothing changes in lock mode so it isn't here
	}
}

void MovementProcessor::centerView()
{
	moveControl->setPosition(*curY - heightCenter, *curX - widthCenter);
}

void MovementProcessor::processMovementInput(int input)
{
	switch (input)
	{
	case CTL_HOME: //use two movements to upper left corner
		processDirectionalInput(DIR_LEFT, moveControl->getTotalCols());
		processDirectionalInput(DIR_UP, moveControl->getTotalRows());
		break;
	case CTL_END: //use two movements to lower right corner
		processDirectionalInput(DIR_RIGHT, moveControl->getTotalCols());
		processDirectionalInput(DIR_DOWN, moveControl->getTotalRows());
		break;
	default:
		int dir = getDirectionFromKey(input);
		int magnitude = getMoveMagnitudeFromKey(input);
		processDirectionalInput(dir, magnitude);
		break;
	}
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

int MovementProcessor::getDirectionFromKey(int key)
{
	int dir;
	switch (key)
	{
	case KEY_PGUP:
	case KEY_UP: dir = DIR_UP;
		break;
	case KEY_PGDN:
	case KEY_DOWN: dir = DIR_DOWN;
		break;
	case KEY_HOME:
	case CTL_PGUP:
	case KEY_LEFT: dir = DIR_LEFT;
		break;
	case KEY_END:
	case CTL_PGDN:
	case KEY_RIGHT: dir = DIR_RIGHT;
		break;
	default:
		dir = DIR_ERR;
	}
	return dir;
}

void MovementProcessor::processDirectionalInput(int dirInput, int magnitude)
{
	short* axis = NULL;
	int step = 1;

	switch (dirInput)
	{
	case DIR_UP:
		axis = curY;
		step = -step;
		break;
	case DIR_DOWN:
		axis = curY;
		break;
	case DIR_LEFT:
		axis = curX;
		step = -step;
		break;
	case DIR_RIGHT:
		axis = curX;
		break;
	}

	setConvenienceVariables(); //do this for every movement in case dimensions change

	for (int i = 0; i < magnitude; i++) //take repeated steps until move is over
	{
		if (processStep(axis, step, dirInput) == false)
			break;
	}
}

/*
Return integer value representing the edge of the control that we have moved off of.
Return 0 if in bounds

true if cursor coordinates are nonnegative and less than the size of the controllable
*/
int MovementProcessor::inBounds()
{
	if (*curX < 0) return B_WEST;
	if (*curY < 0) return B_NORTH;
	if (*curX >= moveControl->getTotalCols()) return B_EAST;
	if (*curY >= moveControl->getTotalRows()) return B_SOUTH;

	return 0;// *curX >= 0 && *curY >= 0 && *curX < c->getTotalCols() && *curY < c->getTotalRows();
}


/*
Return true if coordinates are not less than the controls upper left coordinate, and within the visible area
*/
bool MovementProcessor::inWindow()
{
	int ulY = moveControl->getUlY();
	int ulX = moveControl->getUlX();

	return *curX >= ulX && *curY >= ulY && *curX < ulX + viewWidth && *curY < ulY + viewHeight;
}


//not sure if this is the best name for this method
void MovementProcessor::adjustDynamicView(int step, int dirInput)
{
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