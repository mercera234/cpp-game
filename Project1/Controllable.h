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
};

