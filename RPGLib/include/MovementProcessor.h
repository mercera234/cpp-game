#pragma once
#include "Controllable.h"
#include "Direction.h"

#define VM_LOCK 0 //lock map in place, cursor cannot move outside of window
#define VM_CENTER 1 //cursor stays in center of screen, maps moves around it constantly
#define VM_DYNAMIC 2 //cursor stays in center of screen, until it gets close to map edge

#define B_NORTH -1
#define B_SOUTH 1
#define IN_BOUNDS 0
#define B_WEST  -2
#define B_EAST  2

class MovementProcessor
{
protected:
	//The cursor to be moved by this object
	short* curY;
	short* curX;
	
	int viewMode;

	//if bounded by the control (not the window, it would always be bounded in that)
	bool bounded;

	//the controllable that the cursor is being moved within
	Controllable* moveControl;

	//convenience variables
	int viewHeight;
	int viewWidth;
	int widthCenter;
	int heightCenter;

	//set to 1 if dimension is odd
	short oddOffsetX;
	short oddOffsetY;

	void setConvenienceVariables();

	int  getMoveMagnitudeFromKey(int key);
	Direction  getDirectionFromKey(int key);
	void processDirectionalInput(Direction input, int magnitude);
	void centerView();
	int inBounds();
	bool inWindow();
	void adjustDynamicView(int step, Direction dirInput);

	virtual bool processStep(short* axis, int step, Direction dirInput) = 0;

public:
	
	void processMovementInput(int input);
	void setBounded(bool bounded) { this->bounded = bounded; }
	void setViewMode(int mode);
	int getViewMode() { return viewMode; }
};
