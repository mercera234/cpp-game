#pragma once
#include "Controllable.h"
#include "GridMenu.h"
#include "TextBoard.h"
#include "ControlManager.h"
#include "ItemBrowser.h"
#include "Actor.h"

class EquipControl : public Controllable, public InputProcessor
{
private:
	GridMenu equipMenu;
	TextBoard statusBoard;
	ItemBrowser inventory;

	ControlManager cm;

	//dimensional data
	int leftWidth = 27;

	void buildEquipMenu(Actor& actor);
	void buildStatusBoard(Actor& actor);

public:
	EquipControl() {}
	EquipControl(Actor& actor);
	
	void setWindow(WINDOW* win);

	void processInput();

	void draw();
};

