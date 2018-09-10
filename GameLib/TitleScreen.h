#pragma once
#include "Controllable.h"
#include "GridMenu.h"

enum TitleMenuOptions
{
	NEW_GAME,
	LOAD_GAME,
	QUIT_GAME,
	EDIT_GAME
};

class TitleScreen : public Controllable
{
private:
	GridMenu titleMenu;
	int titleSpace = 10;

public:

	int processInput(int input);
	void draw();
	TitleScreen();
	~TitleScreen();
};

