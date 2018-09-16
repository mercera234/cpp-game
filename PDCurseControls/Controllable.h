#pragma once
#include "curses.h"
#include "Rectangle.h"
#include "Drawable.h"
#include "TUI.h"

class ControlManager; //forward declaration
class Controllable : public Drawable
{
private:

protected:
	WINDOW* win = nullptr;
	bool focusable = true; //true if this object can have focus
	bool showing = true; //true if should be drawn 
	bool focus = false; //true if the controllable has focus
	bool standout = false; //true if the control should be drawn with bold attribute on
	bool acceptsMouseInput = false; //true if the control can accept mouse input

	int ulY = 0; //the top row that is visible
	int ulX = 0; //the left most col that is visible

	//The window dimensions are like the viewport on the screen
	//These are for holding the true dimensions of the control, useful for maps, menus, anything that contains more data that can actually be shown at a time
	unsigned int totalRows = 0; //total rows control has whether visible or not
	unsigned int totalCols = 0; //total cols control has whether visible or not
	unsigned int totalTiles = 0; //will only be set if total rows or cols are modified

	unsigned short visibleRows = 0; //total rows that can be displayed on screen
	unsigned short visibleCols = 0; //total cols that can be displayed on screen
	

	/*
	modal - true if all input must be diverted to this control when it is the top of the control stack
	Modal components can only be at the top of the stack
	*/
	bool modal; 
	//ControlManager* cm;

	chtype color; //a color pair that can be used for the entire control if desired

public:
	virtual void move(int y, int x);
	virtual void setPosition(int y, int x); //set position of control within window
	virtual void setDimensions(unsigned int rows, unsigned int cols); //set control dimensions (normally >= window dimensions)
	void shift(int y, int x); //shift control within window

	//getters/setters
	virtual void setWindow(WINDOW* win);
	WINDOW* getWindow() { return win; };
	/*void setControlManager(ControlManager* cm) { this->cm = cm; }
	ControlManager* getControlManager() { return cm; }*/

	bool isFocusable() { return focusable; }
	void setFocusable(bool canFocus) { focusable = canFocus; }
	virtual void setFocus(bool focusIn) { focus = focusIn; }
	bool isFocused() { return focus; }
	void setShowing(bool on) { showing = on; }
	bool getShowing() { return showing; }
	void setModal(bool on) { modal = on; }
	bool isModal() { return modal; }
	void setAcceptsMouseInput(bool status) { acceptsMouseInput = status; }
	bool getAcceptsMouseInput() { return acceptsMouseInput; }
	
	int getUlY() { return ulY; }
	int getUlX() { return ulX; }
	unsigned int getTotalRows() { return totalRows; }
	unsigned int getTotalCols() { return totalCols; }
	unsigned int getTotalTiles() { return totalTiles; }
	unsigned short getVisibleRows() { return visibleRows; }
	unsigned short getVisibleCols() { return visibleCols; }
	
	void setColor(int bkgdColor, int textColor = COLOR_WHITE);
};

