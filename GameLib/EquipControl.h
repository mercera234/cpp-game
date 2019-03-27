#pragma once
#include "Controllable.h"
#include "GridMenu.h"
#include "TextBoard.h"
#include "ControlManager.h"
#include "ItemBrowser.h"
#include "Actor.h"
#include "SimpleCommand.h"
#include "Inventory.h"

class EquipControl : public Controllable, public InputProcessor
{
private:
	GridMenu equipMenu;
	TextBoard statusBoard;
	ItemBrowser browser;

	ControlManager cm;
	Actor* actor; //The actor being serviced during the equip process

	//dimensional data
	int leftWidth = 27;

	SimpleCommand<EquipControl> equipCmd;
	void processEquipInput();

	SimpleCommand<EquipControl> invCmd;
	void processInventoryInput();

	void buildEquipMenu();
	void buildStatusBoard();

public:
	EquipControl() {}
	EquipControl(Actor& actorIn);

	void setInventory(Inventory& inventory);
	
	void setWindow(WINDOW* win);

	void processInput();

	void draw();
};

