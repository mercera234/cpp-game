#pragma once
#include "Controllable.h"
#include <iostream>

#define TL_HOR 0
#define TL_VER 1

#define JUST_LEFT 0
#define JUST_CENTER 1
#define JUST_RIGHT 2

class TextLabel : public Controllable
{
private:
	std::string text;
	//unsigned short orientation;
	unsigned short justf; //justification
public:
	TextLabel(WINDOW* win, std::string text);
	void setText(std::string text) { this->text = text; }
	std::string getText() { return text; }
	void setJustification(unsigned short justIn) { justf = justIn; }
	void draw();
};