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
	
	void drawDownArrow();
	void drawUpArrow();
public:
	ScrollBar();
	ScrollBar(Controllable* c);
	void setTargetControl(Controllable* c);
	void draw();

	~ScrollBar() { if(win == nullptr) delwin(win); }
};