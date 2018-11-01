#pragma once
#include "ControlDecorator.h"
#include "curses.h"
#include <iostream>

class Frame : public ControlDecorator
{
private:
	unsigned short border;
	
	std::string text; 
	unsigned short textY, textX; //where to position the text
public:
	Frame() {}
	Frame(WINDOW* win, Controllable* c);
	
	void setFocus(bool focusIn);
	void setBorder(int border) { this->border = border; }

	/*
	Sets text anywhere within frame(even on border)
	*/
	void setText(const std::string& text, int y, int x);
	void draw();
};