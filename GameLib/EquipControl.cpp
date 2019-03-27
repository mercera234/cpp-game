#include <algorithm>
#include "EquipControl.h"
#include "game_strings.h"
#include "LineItem.h"
#include "TextParamValue.h"
#include "LineFormat.h"
#include "defaults.h"
#include "Divider.h"
#include "GameInput.h"
#include "menu_drivers.h"
#include "EquippedItem.h"

EquipControl::EquipControl(Actor& actorIn)
{
	actor = &actorIn;
	buildEquipMenu();
	buildStatusBoard();


	equipCmd.setReceiver(this);
	equipCmd.setAction(&EquipControl::processEquipInput);

	invCmd.setReceiver(this);
	invCmd.setAction(&EquipControl::processInventoryInput);

	cm.registerControl(&equipMenu, KEY_LISTENER, &equipCmd);
	cm.registerControl(&statusBoard, 0, nullptr);

	cm.setFocusedControl(&equipMenu);
}

void EquipControl::processEquipInput()
{
	if (input == GameInput::CANCEL_INPUT)
	{
		exitCode = ExitCode::GO_BACK;
		return;
	}

	EquippedItem* selectedItem = (EquippedItem*)menuDriver((GameInput)input, &equipMenu);
	
	if (selectedItem)
	{
		browser.runFilter(
			[&selectedItem](OwnedItemRecord* record) {
			if (record == nullptr || record->getPossession() == nullptr)
				return;

			GameItem* item = record->getPossession()->item;
			record->selectable = true;

			if (item->type != GameItemType::EQUIPPABLE || 
				item->part != selectedItem->getPart())
			{
				record->selectable = false;
			}
		});

		cm.registerControl(&browser, KEY_LISTENER, &invCmd);
		cm.setFocusedControl(&browser);

		browser.setCurrentItem(0);
	}

}

void EquipControl::processInventoryInput()
{
	switch (input)
	{
	case GameInput::CANCEL_INPUT:
	{
		cm.unRegisterControl(&browser);
		cm.setFocusedControl(&equipMenu);
		return;
	}
	break;
	case GameInput::OK_INPUT:
	{
		GameItem* selectedItem = browser.getCurrentItem();

		if (selectedItem == nullptr || selectedItem->type != GameItemType::EQUIPPABLE)
			return;
		
		EquippedItem* equippedItem = (EquippedItem*)equipMenu.getCurrentItem();
		EquipPart ePart = equippedItem->getPart();
			
		if (selectedItem->part == ePart)
		{
			//unequip old part
			GameItem* removedItem = actor->unEquip(ePart);
			actor->equip(selectedItem); //equip new item
			equippedItem->setItem(selectedItem); //set item in equipmenu as well

			//decrement new item from inventory
			browser.decrementItem();

			//add removed item back to inventory
			browser.acquireItem(removedItem, 1);

			cm.unRegisterControl(&browser);
			cm.setFocusedControl(&equipMenu);
		}
	}
	break;
	}
	
	::processInput(browser, getCursesKeyFromInput((GameInput)input));
}

void EquipControl::setInventory(Inventory& inventory)
{
	browser.setInventory(&inventory);
}

void EquipControl::buildEquipMenu()
{
	//build equipMenu
	auto& equipment = actor->getEquipment();
	equipMenu.resetItems(equipment.size(), 1);

	int i = 0;
	std::for_each(equipment.begin(), equipment.end(),
		[this, &i](std::pair<EquipPart, GameItem*> p)
	{
		equipMenu.setItem(new EquippedItem(p.first, p.second, i), i, 0);
		i++;
	});

	equipMenu.setWrapAround(false);
	//equipMenu->setFocus(true);
	equipMenu.post(true);
	equipMenu.setCurrentItem(0);
}

void EquipControl::buildStatusBoard()
{
	//build statusContent
	TextParamCurrMaxValue* hpRow, *mpRow;
	TextParamValue<BoundInt>* strengthRow, *defenseRow, *intelRow, *willRow, *agilityRow;

	//values are null, but will be setup later
	hpRow = new TextParamCurrMaxValue(new LineFormat(0, Justf::LEFT), HP, &actor->getStat(StatType::HP));
	mpRow = new TextParamCurrMaxValue(new LineFormat(1, Justf::LEFT), MP, &actor->getStat(StatType::MP), 4);
	strengthRow = new TextParamValue<BoundInt>(new LineFormat(2, Justf::LEFT), STRENGTH.substr(0, 3), &actor->getStat(StatType::STRENGTH));
	defenseRow = new TextParamValue<BoundInt>(new LineFormat(3, Justf::LEFT), DEFENSE.substr(0, 3), &actor->getStat(StatType::DEFENSE));
	intelRow = new TextParamValue<BoundInt>(new LineFormat(4, Justf::LEFT), INTELLIGENCE.substr(0, 3), &actor->getStat(StatType::INTELLIGENCE));
	willRow = new TextParamValue<BoundInt>(new LineFormat(5, Justf::LEFT), WILL.substr(0, 3), &actor->getStat(StatType::WILL));
	agilityRow = new TextParamValue<BoundInt>(new LineFormat(6, Justf::LEFT), AGILITY.substr(0, 3), &actor->getStat(StatType::AGILITY));
	statusBoard.addPiece(hpRow);
	statusBoard.addPiece(mpRow);
	statusBoard.addPiece(strengthRow);
	statusBoard.addPiece(defenseRow);
	statusBoard.addPiece(intelRow);
	statusBoard.addPiece(willRow);
	statusBoard.addPiece(agilityRow);
}

void EquipControl::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);

	int eMenuHeight = equipMenu.getMaxItems();
	Rect eMenuRect(eMenuHeight, leftWidth, Pos(0, 0));

	equipMenu.setWindow(TUI::winMgr.deriveWin(win, eMenuRect));


	//
	Rect statRect(statusBoard.getPieceCount(), leftWidth - 4, Pos(eMenuHeight + 1, 4));
	statusBoard.setWindow(TUI::winMgr.deriveWin(win, statRect));


	Rect invRect(getmaxy(win), getmaxx(win) - leftWidth - 1, Pos(0, leftWidth + 1));
	browser.setWindow(TUI::winMgr.deriveWin(win, invRect));
}

void EquipControl::processInput()
{
	exitCode = ::processInput(cm, input);
}

void EquipControl::draw()
{
	Divider h(Pos(getmaxy(equipMenu.getWindow()), 0), Axis::HORIZONTAL, leftWidth);
	Divider v(Pos(0, leftWidth), Axis::VERTICAL);

	h.draw(win);
	v.draw(win);

	touchwin(win);
	wnoutrefresh(win);

	cm.draw();
}