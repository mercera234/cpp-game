#pragma once
#include "Controllable.h"
#include "GridMenu.h"
#include "DrawBoard.h"
#include "ControlManager.h"
#include "ItemBrowser.h"
#include "Actor.h"
#include "SimpleCommand.h"
#include "Inventory.h"
#include "ValueChange.h"

class EquipControl : public Controllable, public InputProcessor
{
private:
	GridMenu equipMenu;
	DrawBoard<DrawablePiece> statusBoard;
	ItemBrowser browser;

	ControlManager cm;
	Actor* actor; //The actor being serviced during the equip process

	std::map<StatType, int> statChanges;

	//dimensional data
	int leftWidth = 27;

	SimpleCommand<EquipControl> equipCmd;
	void processEquipInput();

	SimpleCommand<EquipControl> invCmd;
	void processInventoryInput();

	void buildEquipMenu();
	void buildStatusBoard();
	void updateStatusBoard();
	void clearStatChanges();

	void equipItem();
public:
	EquipControl() {}
	EquipControl(Actor& actorIn);
	~EquipControl();

	void setInventory(Inventory& inventory);
	
	void setWindow(WINDOW* win);

	void processInput();

	void draw();
};

