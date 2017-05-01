#pragma once
#include "Controllable.h"
#include <iostream>
using namespace std;

#define TL_HOR 0
#define TL_VER 1

#define JUST_LEFT 0
#define JUST_CENTER 1
#define JUST_RIGHT 2

class TextLabel : public Controllable
{
private:
	string text;
	//unsigned short orientation;
	unsigned short justf; //justification
public:
	TextLabel(WINDOW* win, string text);
	void setText(string text) { this->text = text; }
	void setJustification(unsigned short justIn) { justf = justIn; }
	void draw();
};