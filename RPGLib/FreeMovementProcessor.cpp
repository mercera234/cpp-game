#include "FreeMovementProcessor.h"
#include "TUI.h" 


FreeMovementProcessor::FreeMovementProcessor(Controllable* c, int* curY, int* curX)
{
	setMoveControl(c);
	setCursor(curY, curX);
	setViewMode(ViewMode::CENTER);
}


bool FreeMovementProcessor::processMovement(Movement& move)
{
	//save start pos
	int prevY = *curY;
	int prevX = *curX;

	setConvenienceVariables(); //do this for every movement in case dimensions change

	moveCursor(move);

	//verify that cursor is within bounds
	if (bounded && (inBounds() != Boundary::IN_BOUNDS))
	{
		bringCursorInBounds();
	}

	if (viewMode == ViewMode::LOCK && inWindow() == false) //just make sure move doesn't go out of window
	{
		bringCursorInWindow();
	}

	if (prevY == *curY && prevX == *curX)
		return false;

	adjustView();

	return true;
}

void FreeMovementProcessor::bringCursorInBounds()
{
	if (*curX < 0)
		*curX = 0;
	else if (*curY < 0)
		*curY = 0;
	else if (*curX >= (int)(moveControl->getTotalCols()))
		*curX = moveControl->getTotalCols() - 1;
	else if (*curY >= (int)(moveControl->getTotalRows()))
		*curY = moveControl->getTotalRows() - 1;
	
}

void FreeMovementProcessor::bringCursorInWindow()
{
	int ulY = moveControl->getUlY();
	int ulX = moveControl->getUlX();

	int winX, winY; //window relative coordinate
	winX = *curX - ulX;
	winY = *curY - ulY;

	if (winX < 0)
		*curX = ulX;
	else if (winY < 0)
		*curY = ulY;
	else if (winX >= viewWidth)
		*curX = viewWidth - 1 + ulX;
	else if (winY >= viewHeight)
		*curY = viewHeight - 1 + ulY;
}