#pragma once
#include "Controllable.h"
#include <iostream>
using namespace std;

class TextLabel : public Controllable
{
private:
	string text;
public:
	TextLabel(WINDOW* win, string text);
	void draw();
};