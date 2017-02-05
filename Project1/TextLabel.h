#pragma once
#include "Controllable.h"
#include <iostream>
using namespace std;

#define TL_HOR 0
#define TL_VER 1
class TextLabel : public Controllable
{
private:
	string text;
	//unsigned short orientation;
public:
	TextLabel(WINDOW* win, string text);
	void setText(string text) { this->text = text; }
	void draw();
};