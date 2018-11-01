#include "ItemBrowser.h"
#include "DialogWindow.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "GameInput.h"
#include "ExitCode.h"
#include "menu_drivers.h"
#include "GameItem.h"
#include "OwnedItemRecord.h"
#include "TextLabel.h"

ItemBrowser::ItemBrowser()
{
	menu = new GridMenu;
	setControl(menu);

	itemCmd.setReceiver(this);
	itemCmd.setAction(&ItemBrowser::processItemInput);

	playerCmd.setReceiver(this);
	playerCmd.setAction(&ItemBrowser::processPlayerInput);

	cm.registerControl(this, KEY_LISTENER, &itemCmd);
	cm.setFocusedControl(this);
}


void ItemBrowser::processInput()
{
	cm.setExitCode(HANDLED);
	cm.handleInput(input);
}

void ItemBrowser::processItemInput()
{
	if (input == GameInput::CANCEL_INPUT)
	{
		exitCode = ExitCode::GO_BACK;
		return;
	}

	OwnedItemRecord* record = (OwnedItemRecord*)menuDriver(input, menu);

	if (record != nullptr) //an item was selected
	{
		OwnedItem* possession = record->getPossession();

		switch (possession->item->type)
		{
		case GameItemType::CONSUMABLE: 
			cm.setFocusedControl(playerFrame);
			break;
		}
	}
	else //update description box
	{
		if (descFrame == nullptr)
			return;

		TextLabel* lbl = (TextLabel*)descFrame->getControl();
		lbl->setText(getDescription());
	}
	
}

void ItemBrowser::processPlayerInput()
{
	//TODO stub
	cm.setFocusedControl(this);
	return;
}


void ItemBrowser::setItems(std::vector<OwnedItem*>& items)
{
	inventoryPtr = &items; //save reference to items

	//if (items.size() <= 0)
	//{
	//	items.push_back(nullptr); //make sure inventory always has at least 1 item
	//}

	menu->resetItems(items.size(), 1);

	int element = 0;
	for each (OwnedItem* item in items)
	{
		menu->setItem(new OwnedItemRecord(item, element++));
	}

	
	
	menu->setCurrentItem(0);
	menu->post(true);
}




void ItemBrowser::getDescription(std::list<std::string>& description)
{
	OwnedItemRecord* record = getCurrentItem();
	
	if (record == nullptr)
		return;

	std::string desc = record->getPossession()->item->description;
	description.push_back(desc);
}

std::string ItemBrowser::getDescription()
{
	OwnedItemRecord* record = getCurrentItem();

	if (record == nullptr)
		return "";

	if (record->getPossession()->item == nullptr)
		return "";

	return record->getPossession()->item->description;
}

OwnedItemRecord* ItemBrowser::getCurrentItem()
{
	return (OwnedItemRecord*)menu->getCurrentItem();
}

void ItemBrowser::registerControls()
{
	//cm.registerControl(playerFrame, KEY_LISTENER, &playerCmd);
}

void ItemBrowser::setDescFrame(Frame* descFrameIn)
{
	descFrame = descFrameIn;

	TextLabel* lbl = (TextLabel*)descFrame->getControl();
	lbl->setText(getDescription());
}