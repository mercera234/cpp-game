#pragma once
#include "ControlDecorator.h"
#include "curses.h"
#include <iostream>
using namespace std;

#define BORDER_NONE 0
#define BORDER_NORMAL 1
#define BORDER_BOLD 2
class Frame : public ControlDecorator
{
private:
	unsigned short border;
	//Controllable* c;
	string text; 
	unsigned short textY, textX; //where to position the text
public:
	Frame(WINDOW* win, Controllable* c);
	//Controllable* getControl() { return c; }
	void setBorder(int border) { this->border = border; }
	void setText(string text, int y, int x);
	void draw();
	~Frame();
};