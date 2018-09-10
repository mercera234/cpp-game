#pragma once
#include "Controllable.h"
#include "Direction.h"

enum class ViewMode
{
	LOCK, //lock map in place, cursor cannot move outside of window
	CENTER, //cursor stays in center of screen, maps moves around it constantly
	DYNAMIC //cursor stays in center of screen, until it gets close to map edge
};


class MovementProcessor
{
protected:
	//The cursor to be moved by this object
	int* curY;
	int* curX;
	
	//the controllable that the cursor is being moved within
	Controllable* moveControl = nullptr;

	//the way the control is being viewed. A valid control and cursor must be set before this can be setup
	ViewMode viewMode;

	//True if movement outside the edges of the control is disallowed. (not the window, it would always be bounded in that)
	bool bounded = true;

	//convenience variables
	int viewHeight = 0;
	int viewWidth = 0;
	int widthCenter = 0;
	int heightCenter = 0;

	//offsets that are set to 1 if dimension is odd
	int oddOffsetX = 0;
	int oddOffsetY = 0;

	void setConvenienceVariables();

	int getMoveMagnitudeFromKey(int key);
	Direction getDirectionFromKey(int key);

	/*Process movement after pressing Home or End keys. 
	Return value is the move that was taken.*/
	MovementChain processHomeEndInput(int key);

	/*Process movement after pressing directional arrows. 
	Return value is the move that was taken.*/
	Movement processDirectionalInput(Dir input, int magnitude);
	void moveCursor(Movement& move);

	void adjustView();
	void centerView();
	void dynamicView();

	/* Return whether cursor is in bounds by returning edge boundary cursor has gone beyond */
	Boundary inBounds();

	/* Return true if coordinates are not less than the controls upper left coordinate, and within the visible area */
	bool inWindow();

	/* Adjust the view if viewMode is set to dynamic. Adjustment occurs when cursor is too close to the boundaries of control.
	Return true if movement took place. move is updated with the actual move taken.*/
	virtual bool processMovement(Movement& move) = 0;
	void reverseMovement(Movement& move);
	
public:
	/*Process movement after pressing any key.
	Return value is the move that was taken.*/
	MovementChain processMovementInput(int input);

	//getters/setters
	void setViewMode(ViewMode mode);
	ViewMode getViewMode() { return viewMode; }
	void setMoveControl(Controllable* control) { moveControl = control; }
	Controllable* getMoveControl() { return moveControl; }
	void setCursor(int* y, int* x) { curY = y; curX = x; }
	void setBounded(bool bounded) { this->bounded = bounded; }
	bool getBounded() { return bounded; }
};
