#pragma once
#include "Controllable.h"
#include "Style.h"
#include <iostream>

class TextLabel : public Controllable
{
private:
	std::string text;
	//unsigned short orientation;
	Justf justf;
	void init();
public:
	TextLabel();
	TextLabel(WINDOW* win, const std::string& text);

	void draw();

	//getters/setters
	void setText(const std::string& text) { this->text = text; }
	std::string getText() { return text; }
	void setJustification(Justf justIn) { justf = justIn; }
	
};