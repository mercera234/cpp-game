#pragma once
#include "Controllable.h"
#include "TUI.h" //for now, but the dir defines should be in this class!

#define VM_LOCK 0 //lock map in place, cursor cannot move outside of window
#define VM_CENTER 1 //cursor stays in center of screen, maps moves around it constantly
#define VM_DYNAMIC 2 //cursor stays in center of screen, until it gets close to map edge

class MovementProcessor
{
private:
	//The cursor to be moved by this object
	short* curY;
	short* curX;
	bool cursorType; //true = static, false = dynamic
	int viewMode;

	//if bounded by the control (not the window, it would always be bounded in that)
	bool bounded;
	bool superbounded; //bounded, and can't even see outside of the control frame

	//the controllable that the cursor is being moved within
	Controllable* c;

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
	int  getDirectionFromKey(int key);
	void processDirectionalInput(int input, int magnitude);
public:
	MovementProcessor(Controllable* c, short* curY, short* curX);
	void processMovementInput(int input);
	void setBounded(bool bounded) { this->bounded = bounded; }
	void setCursorType(bool cType) { cursorType = cType; }
	void setViewMode(int mode); 
};