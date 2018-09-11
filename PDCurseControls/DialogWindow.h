#pragma once
#include "Controllable.h"
#include "Frame.h"


class DialogWindow : public Controllable
{
protected:
	Frame frame;
	Controllable* control = nullptr; //a new allocated control for temporary display
	WINDOW* innerWin = nullptr; //a window for the control
public:
	DialogWindow();
	~DialogWindow();
	void setWindow(WINDOW* win);
	void setControl(Controllable* control);

	void setFocus(bool focusIn);

	//setters/getters

	Controllable* getControl() { return control; }
	Frame& getFrame() { return frame; }

	WINDOW* getInnerWin() { return innerWin; }

	void draw();
};
