#pragma once
#include "Browser.h"
#include "GridMenu.h"
#include "GameItem.h"
#include "OwnedItemRecord.h"

class ItemBrowser : public Browser
{
private:
	GridMenu* menu;
	std::vector<OwnedItem*>* inventoryPtr; //a pointer to an inventory

	Frame* playerFrame; //is the whole frame needed? yes, when it receives focus the frame needs to be bold
	Frame* descFrame;

	SimpleCommand<ItemBrowser> itemCmd;
	SimpleCommand<ItemBrowser> playerCmd;

	void processItemInput();
	void processPlayerInput();

public:
	ItemBrowser();
	void setItems(std::vector<OwnedItem*>& items);


	void processInput();
	void getDescription(std::list<std::string>& description);
	std::string getDescription(); //retrieve description of current item

	OwnedItemRecord* getCurrentItem();

	void registerControls();

	
	//setters/getters

	void setPlayerFrame(Frame* playerFrameIn) { playerFrame = playerFrameIn; }
	void setDescFrame(Frame* descFrameIn);

};

