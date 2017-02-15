#pragma once
#include "ControlDecorator.h"

class ScrollBar : public ControlDecorator
{
private:
	/*
	vertical position 
	vPos = 0 at top ; arrow down only 
	0 < vPos < vMax = in the middle ; arrow up and down
	vMax = at bottom ; arrow up only
	*/
	int vPos; 
	unsigned short visibleRows; //this will be the same as the height of the window

	void drawDownArrow();
	void drawUpArrow();
public:
	ScrollBar(Controllable* c);
	void draw();

};