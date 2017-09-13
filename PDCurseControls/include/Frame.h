#pragma once
#include "ControlDecorator.h"
#include "curses.h"
#include <iostream>
using namespace std;

class Frame : public ControlDecorator
{
private:
	unsigned short border;
	
	string text; 
	unsigned short textY, textX; //where to position the text
public:
	Frame(WINDOW* win, Controllable* c);
	
	void setFocus(bool focusIn);
	void setBorder(int border) { this->border = border; }

	/*
	Sets text on the top border of the frame
	*/
	void setText(const std::string& text, int y, int x);
	void draw();
	~Frame();
};