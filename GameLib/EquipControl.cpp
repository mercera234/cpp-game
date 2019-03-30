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

	statChanges.insert(std::make_pair(StatType::STRENGTH, 0));
	statChanges.insert(std::make_pair(StatType::DEFENSE, 0));
	statChanges.insert(std::make_pair(StatType::AGILITY, 0));
	statChanges.insert(std::make_pair(StatType::WILL, 0));
	statChanges.insert(std::make_pair(StatType::INTELLIGENCE, 0));

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
		cm.setExitCode(exitCode);
		return;
	}

	EquippedItem* selectedItem = (EquippedItem*)menuDriver((GameInput)input, &equipMenu);
	
	if (selectedItem)
	{
		browser.runFilter(
			[&selectedItem](OwnedItemRecord* record) {
			if (record == nullptr)
				return;

			record->selectable = true;
			if (record->getPossession() == nullptr)  //blank spaces will be selectable in equip inventory
				return;

			GameItem* item = record->getPossession()->item;

			//only equippable items matching the part we are equipping will be selectable
			if (item->type != GameItemType::EQUIPPABLE ||  
				item->part != selectedItem->getPart())
			{
				record->selectable = false;
			}
		});

		cm.registerControl(&browser, KEY_LISTENER, &invCmd);
		cm.setFocusedControl(&browser);

		browser.setCurrentItem(0);
		updateStatusBoard();
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
		clearStatChanges();
		return;
	}
	break;
	case GameInput::OK_INPUT:
	{
		equipItem();
		return;
	}
	break;
	}
	
	::processInput(browser, getCursesKeyFromInput((GameInput)input));

	updateStatusBoard();
}

void EquipControl::clearStatChanges()
{
	for (auto& p : statChanges)
	{
		p.second = 0;
	}
}

void EquipControl::updateStatusBoard()
{
	clearStatChanges();

	GameItem* item = browser.getCurrentItem();
	if (item == nullptr || item->type != GameItemType::EQUIPPABLE)
		return;

	auto& equipment = actor->getEquipment();

	TargetEffects& t = item->effects;
	std::for_each(t.statValues.begin(), t.statValues.end(),
		[this, &equipment, item]
		(std::pair<StatType, int> p) 
	{
		int newObjStat = p.second;
		int currObjStat = 0;
		
		GameItem* equippedItem = equipment[item->part];
			
		if (equippedItem != nullptr)
			currObjStat = equippedItem->effects.statValues[p.first];

		statChanges[p.first] = newObjStat - currObjStat;
	});
}

void EquipControl::equipItem()
{
	GameItem* selectedItem = browser.getCurrentItem();

	EquippedItem* equippedItem = (EquippedItem*)equipMenu.getCurrentItem();
	EquipPart ePart = equippedItem->getPart();

	if (selectedItem == nullptr)
	{
		//only unequip item
		GameItem* removedItem = actor->unEquip(ePart);
		equippedItem->setItem(selectedItem);
		
		browser.acquireItem(removedItem, 1);
		//clearStatChanges();
		updateStatusBoard();
		return;
	}
		

	if (selectedItem->type != GameItemType::EQUIPPABLE)
		return;

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
		clearStatChanges();
	}
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
	TextParamCurrMaxValue* hpRow, *mpRow;
	TextParamValue<BoundInt>* strengthRow, *defenseRow, *intelRow, *willRow, *agilityRow;

	hpRow = new TextParamCurrMaxValue(new LineFormat(0, Justf::LEFT), HP, &actor->getStat(StatType::HP));
	mpRow = new TextParamCurrMaxValue(new LineFormat(1, Justf::LEFT), MP, &actor->getStat(StatType::MP), 4);
	strengthRow = new TextParamValue<BoundInt>(new LineFormat(2, Justf::LEFT), STRENGTH.substr(0, 3), &actor->getStat(StatType::STRENGTH));
	defenseRow = new TextParamValue<BoundInt>(new LineFormat(3, Justf::LEFT), DEFENSE.substr(0, 3), &actor->getStat(StatType::DEFENSE));
	intelRow = new TextParamValue<BoundInt>(new LineFormat(4, Justf::LEFT), INTELLIGENCE.substr(0, 3), &actor->getStat(StatType::INTELLIGENCE));
	willRow = new TextParamValue<BoundInt>(new LineFormat(5, Justf::LEFT), WILL.substr(0, 3), &actor->getStat(StatType::WILL));
	agilityRow = new TextParamValue<BoundInt>(new LineFormat(6, Justf::LEFT), AGILITY.substr(0, 3), &actor->getStat(StatType::AGILITY));

	ValueChange* strMod = new ValueChange(new PosFormat(2, 9), &statChanges[StatType::STRENGTH]);
	ValueChange* defMod = new ValueChange(new PosFormat(3, 9), &statChanges[StatType::DEFENSE]);
	ValueChange* intMod = new ValueChange(new PosFormat(4, 9), &statChanges[StatType::INTELLIGENCE]);
	ValueChange* wilMod = new ValueChange(new PosFormat(5, 9), &statChanges[StatType::WILL]);
	ValueChange* agiMod = new ValueChange(new PosFormat(6, 9), &statChanges[StatType::AGILITY]);

	strMod->setDrawZero(false);
	defMod->setDrawZero(false);
	intMod->setDrawZero(false);
	wilMod->setDrawZero(false);
	agiMod->setDrawZero(false);

	statusBoard.addPiece(hpRow);
	statusBoard.addPiece(mpRow);
	statusBoard.addPiece(strengthRow);
	statusBoard.addPiece(defenseRow);
	statusBoard.addPiece(intelRow);
	statusBoard.addPiece(willRow);
	statusBoard.addPiece(agilityRow);
	statusBoard.addPiece(strMod);
	statusBoard.addPiece(defMod);
	statusBoard.addPiece(agiMod);
	statusBoard.addPiece(intMod);
	statusBoard.addPiece(wilMod);

	statusBoard.setStandout(true);
}

void EquipControl::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);

	int eMenuHeight = equipMenu.getMaxItems();
	Rect eMenuRect(eMenuHeight, leftWidth, Pos(1, 0));

	equipMenu.setWindow(TUI::winMgr.deriveWin(win, eMenuRect));

	Rect statRect(7, leftWidth - 4, Pos(eMenuHeight + 2, 4));
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
	Divider h(Pos(getmaxy(equipMenu.getWindow()) + 1, 0), Axis::HORIZONTAL, leftWidth);
	Divider v(Pos(0, leftWidth), Axis::VERTICAL);

	h.draw(win);
	v.draw(win);

	mvwaddstr(win, 0, 1, actor->name.c_str());

	touchwin(win);
	wnoutrefresh(win);

	cm.draw();
}

EquipControl::~EquipControl()
{
	TUI::winMgr.delWin(equipMenu.getWindow());
	TUI::winMgr.delWin(statusBoard.getWindow());
	TUI::winMgr.delWin(browser.getWindow());
}