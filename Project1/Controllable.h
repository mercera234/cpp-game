#pragma once
#include "curses.h"

class ControlManager; //forward declaration
class Controllable
{
private:

protected:
	WINDOW* win;
	bool focusable; //true if control can accept keyboard input
	bool showing; //true if should be drawn 
	
	unsigned short topRow; //the top row that is visible
	unsigned short leftCol; //the left most col that is visible

	unsigned int totalRows; //total rows control has whether visible or not
	unsigned int totalCols; //total cols control has whether visible or not
	/*
	modal - true if all input must be diverted to this control when it is the top of the control stack
	Modal components can only be at the top of the stack
	*/
	bool modal; 
	ControlManager* cm;
public:
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
	unsigned short getTopRow() { return topRow; }
	unsigned short getLeftCol() { return leftCol; }
	unsigned short getTotalRows() { return totalRows; }
	unsigned short getTotalCols() { return totalCols; }
};

