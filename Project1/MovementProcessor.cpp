#include "MovementProcessor.h"

MovementProcessor::MovementProcessor(Controllable* c, short* curY, short* curX)
{
	this->c = c;
	this->curY = curY;
	this->curX = curX;

	bounded = true;
	cursorType = true; //static by default

	//set convenience variables
	setConvenienceVariables();

	setViewMode(VM_CENTER);
}

void MovementProcessor::setConvenienceVariables()
{
	getmaxyx(c->getWindow(), viewHeight, viewWidth);
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
	viewMode = mode;

	switch (viewMode)
	{
	case VM_CENTER: //reposition map so cursor is centered
		c->setPosition(*curY - heightCenter, *curX - widthCenter); 
		break;
	case VM_DYNAMIC:
	{
		int ulY = c->getUlY();
		int ulX = c->getUlX();
		

		int newX = ulX;
		int newY = ulY;
		if (ulY < 0) newY = 0;
		else if (ulY + viewHeight > c->getTotalRows()) newY = c->getTotalRows() - viewHeight;

		if (ulX < 0) newX = 0;
		else if (ulX + viewWidth > c->getTotalCols()) newX = c->getTotalCols() - viewWidth;

		c->setPosition(newY, newX);
	}
		//nothing changes in lock mode so it isn't here
	}
}

void MovementProcessor::processMovementInput(int input)
{
	switch (input)
	{
	case CTL_HOME: //use two movements to upper left corner
		processDirectionalInput(DIR_LEFT, c->getTotalCols());
		processDirectionalInput(DIR_UP, c->getTotalRows());
		break;
	case CTL_END: //use two movements to lower right corner
		processDirectionalInput(DIR_RIGHT, c->getTotalCols());
		processDirectionalInput(DIR_DOWN, c->getTotalRows());
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
	getmaxyx(c->getWindow(), visibleRows, visibleCols);

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
	case KEY_END: magnitude = c->getTotalCols();
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

	int ulY = c->getUlY();
	int ulX = c->getUlX();

	for (int i = 0; i < magnitude; i++) //take repeated steps until move is over
	{
		//move the cursor
		*axis += step;

		//verify that cursor is within bounds
		if (bounded)
		{
			if (*curX < 0 || *curY < 0 || *curX >= c->getTotalCols() || *curY >= c->getTotalRows())
			{
				*axis -= step; break;//reverse step and quit
			}
		}
		
		
		switch(viewMode)
		{
		case VM_LOCK: 
			//just make sure step doesn't go out of window
		{
			if (*curX < ulX || *curY < ulY || *curX >= ulX + viewWidth || *curY >= ulY + viewHeight)
			{
				*axis -= step; break;//reverse step and quit
			}
		}
			break;
		case VM_CENTER: 
			axis == curY ? c->shift(step, 0) : c->shift(0, step);
			break;
		case VM_DYNAMIC: 
		{
			
			switch (dirInput)
			{
			case DIR_RIGHT:
				if (*curX > widthCenter && *curX <= c->getTotalCols() - widthCenter - oddOffsetX)
					c->shift(0, step);
				break;
			case DIR_LEFT:
				if (*curX >= widthCenter && *curX < c->getTotalCols() - widthCenter - oddOffsetX)
					c->shift(0, step);
				break;
			case DIR_DOWN:
				if (*curY > heightCenter && *curY <= c->getTotalRows() - heightCenter - oddOffsetY)
					c->shift(step, 0);
				break;
			case DIR_UP:
				if (*curY >= heightCenter && *curY < c->getTotalRows() - heightCenter - oddOffsetY)
					c->shift(step, 0);
				break;

			}
		}
			break;
		}

		//not sure what to do about this
		////apply brush if necessary
		//if (tool == BRUSH)
		//	applyTool(curY, curX);
	}
}