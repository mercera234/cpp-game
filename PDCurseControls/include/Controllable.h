#pragma once
#include "curses.h"
#include "Rectangle.h"
#include "Drawable.h"

class ControlManager; //forward declaration
class Controllable : public Drawable
{
private:

protected:
	WINDOW* win;
	bool focusable; //true if control can accept keyboard input
	bool showing; //true if should be drawn 
	bool focus; //true if the controllable is focused
	
	Rect* r; //A rectangle to save the true dimensions of the window in case it should go off screen
	
	int ulY; //the top row that is visible
	int ulX; //the left most col that is visible

	//The window dimensions are like the viewport on the screen
	//These are for holding the true dimensions of the control, useful for maps, menus, anything that contains more data that can actually be shown at a time
	unsigned int totalRows; //total rows control has whether visible or not
	unsigned int totalCols; //total cols control has whether visible or not
	unsigned int totalTiles; //will only be set if total rows or cols are modified

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
//	virtual void draw() = 0;
	virtual void move(int y, int x);
	virtual void setPosition(int y, int x);
	virtual void setDimensions(unsigned int rows, unsigned int cols);
	void shift(int y, int x);

	//getters/setters
	WINDOW* getWindow() { return win; };
	bool isFocusable() { return focusable; }
	virtual void setFocus() {};
	virtual void setCursorFocus() {}; //for moving the cursor to this component
	virtual void setFocus(bool focusIn) { focus = focusIn; }
	bool isFocused() { return focus; }
	void setShowing(bool on) { showing = on; }
	void setModal(bool on) { modal = on; }
	bool isModal() { return modal; }
	void setControlManager(ControlManager* cm) { this->cm = cm; }
	ControlManager* getControlManager() { return cm; }
	int getUlY() { return ulY; }
	int getUlX() { return ulX; }
	unsigned short getTotalRows() { return totalRows; }
	unsigned short getTotalCols() { return totalCols; }
	unsigned short getVisibleRows() { return visibleRows; }
	unsigned short getVisibleCols() { return visibleCols; }
	
};

