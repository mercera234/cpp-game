#pragma once
#include "curses.h"
#include "Rectangle.h"

class ControlManager; //forward declaration
class Controllable
{
private:

protected:
	WINDOW* win;
	bool focusable; //true if control can accept keyboard input
	bool showing; //true if should be drawn 
	
	Rect* r; //A rectangle to save the true dimensions of the window in case it should go off screen
	
	int ulY; //the top row that is visible
	int ulX; //the left most col that is visible

	//The window dimensions are like the viewport on the screen
	//These are for holding the true dimensions of the control, useful for maps, menus, anything that contains more data that can actually be shown at a time
	unsigned int totalRows; //total rows control has whether visible or not
	unsigned int totalCols; //total cols control has whether visible or not
	unsigned short visibleRows; //total rows that can be displayed on screen
	unsigned short visibleCols; //total cols that can be displayed on screen


	/*
	modal - true if all input must be diverted to this control when it is the top of the control stack
	Modal components can only be at the top of the stack
	*/
	bool modal; 
	ControlManager* cm;
	void setWindow(Rect* bounds);
public:
	void setWindow(WINDOW* win);
	virtual void draw() = 0;
	virtual void move(int y, int x);
	WINDOW* getWindow() { return win; };
	bool isInWindow(int y, int x);
	void translateCoords(int inY, int inX, int &outY, int &outX);
	bool isFocusable() { return focusable; }
	void setModal(bool on) { modal = on; }
	bool isModal() { return modal; }
	void setControlManager(ControlManager* cm) { this->cm = cm; }
	ControlManager* getControlManager() { return cm; }
	int getUlY() { return ulY; }
	int getUlX() { return ulX; }
	unsigned short getTotalRows() { return totalRows; }
	unsigned short getTotalCols() { return totalCols; }
	void setPosition(int y, int x);
	void shift(int y, int x);
};

