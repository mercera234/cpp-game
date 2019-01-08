#pragma once
#include "Controllable.h"
#include "GridMenu.h"
#include "MenuBuilder.h"

class TitleScreen : public Controllable
{
private:
	GridMenu titleMenu;
	int titleSpace = 10; //rows needed for title
	MenuBuilder builder;
public:

	int processInput(int input);
	void draw();
	TitleScreen();
};

