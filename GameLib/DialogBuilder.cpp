#include "DialogBuilder.h"
#include "LineItem.h"
#include "defaults.h"
#include "MainMenuOption.h"
#include "TextBoard.h"
#include "TextParamValue.h"
#include "LineFormat.h"
#include "ItemBrowser.h"
#include "TextLabel.h"
#include "CenteredFormat.h"
#include "ConfigMenu.h"
#include "AutoMap.h"
#include "ActorCard.h"
#include "OwnedItemRecord.h"
#include "game_strings.h"
#include "EquipControl.h"
#include "GameAlgorithm.h"

void DialogBuilder::setDialogControl(Controllable* control, DialogWindow& dWin, Rect& r)
{
	dWin.setControl(control);
	dWin.setWindow(TUI::winMgr.newWin(r.height, r.width, r.origin.y, r.origin.x)); //this will set the window of the inner control as well
}


void DialogBuilder::buildMainMenu(DialogWindow& dWin, Rect r)
{
	GridMenu* mainMenu = new GridMenu(); //the dialogWindow will delete

	mainMenu->resetItems(4, 2);

	mainMenu->setItemWidth(6);
	mainMenu->setColSepLength(0);

	mainMenu->setItem(new LineItem(S_ITEM, 0, MainMenuOption::INVENTORY), 0, 0);
	mainMenu->setItem(new LineItem(S_EQUIP, 1, MainMenuOption::EQUIP), 1, 0);
	mainMenu->setItem(new LineItem(S_STATUS, 2, MainMenuOption::STATUS), 2, 0);
	mainMenu->setItem(new LineItem(S_SKILL, 3, MainMenuOption::SKILL), 3, 0);
	mainMenu->setItem(new LineItem(S_CONFIG, 4, MainMenuOption::CONFIG), 0, 1);
	mainMenu->setItem(new LineItem(S_MAP, 5, MainMenuOption::MAP), 1, 1);
	mainMenu->setItem(new LineItem(S_SAVE, 6, MainMenuOption::SAVE), 2, 1);
	mainMenu->setItem(new LineItem(S_QUIT, 7, MainMenuOption::MAIN_QUIT), 3, 1);

	mainMenu->setWrapAround(false);
	mainMenu->setFocus(true);
	mainMenu->post(true);

	mainMenu->setCurrentItem(0);

	setDialogControl(mainMenu, dWin, r);
}

void DialogBuilder::buildPlayerMenu(DialogWindow& dWin, Rect r)
{
	GridMenu* playerMenu = new GridMenu(); //the dialogWindow will delete
	playerMenu->resetItems(playerCapacity, 1);
	playerMenu->setItemHeight(4);

	for (int i = 0; i < playerCapacity; i++)
	{
		//std::string name = "";
		if (i < (int)rm->playerParty.size())
			playerMenu->setItem(new ActorCard(&rm->playerParty[i], i), i, 0);
		else
			playerMenu->setItem(new LineItem("", i, -1), i, 0);
			//name = rm->playerParty[i].name;

		
		//playerMenu->setItem(new LineItem(name, i, -1), i, 0); //sets blank items to start
	}

	playerMenu->setWrapAround(false);
	playerMenu->setFocus(false);
	playerMenu->post(true);

	setDialogControl(playerMenu, dWin, r);
}


void DialogBuilder::buildDesc(DialogWindow& dWin, Rect r)
{
	TextBoard* descContent = new TextBoard();

	MegaMap* currMap = rm->currMap;
	int id = currMap->getCurrMapRoomId();
	MapRoom& room = rm->getData().getRoom(id);

	TextPiece* mapText = new TextPiece(new LineFormat(0, Justf::LEFT), currMap->name);
	TextPiece* roomText = new TextPiece(new LineFormat(1, Justf::LEFT), room.name);
	TextParamValue<std::string>* floor = new TextParamValue<std::string>(
		new LineFormat(2, Justf::LEFT), S_LEVEL, &currMap->getFloorLabel(), 2);

	descContent->addPiece(mapText);
	descContent->addPiece(roomText);
	descContent->addPiece(floor);

	setDialogControl(descContent, dWin, r);
}


void DialogBuilder::buildMainMenuBody(DialogWindow& dWin, Rect r)
{
	TextBoard* bodyContent = new TextBoard();

	TextParamValue<BoundInt>* gold, *steps, *enemiesKilled, *battlesWon;

	gold = new TextParamValue<BoundInt>(
		new LineFormat(0, Justf::LEFT), GOLD$, &rm->getData().retrieveIntData(GOLD$));

	steps = new TextParamValue<BoundInt>(
		new LineFormat(1, Justf::LEFT), STEPS, &rm->getData().retrieveIntData(STEPS));

	enemiesKilled = new TextParamValue<BoundInt>(
		new LineFormat(2, Justf::LEFT), ENEMIES_KILLED, &rm->getData().retrieveIntData(ENEMIES_KILLED));

	battlesWon = new TextParamValue<BoundInt>(
		new LineFormat(3, Justf::LEFT), BATTLES_WON, &rm->getData().retrieveIntData(BATTLES_WON));
	
	bodyContent->addPiece(gold);
	bodyContent->addPiece(steps);
	bodyContent->addPiece(enemiesKilled);
	bodyContent->addPiece(battlesWon);

	
	setDialogControl(bodyContent, dWin, r);
//	controlCache.insert(std::make_pair("DEFAULT BODY", bodyContent));
}


void DialogBuilder::buildMainMenuStatus(DialogWindow& dWin, Rect r, Actor& actor)
{
	TextBoard* statusContent = new TextBoard();

	TextParamCurrMaxValue* hpRow, *mpRow;
	TextParamValue<BoundInt>* lvRow, *strengthRow, *defenseRow, *intelRow, *willRow, *agilityRow, *expRow;
	TextPiece* toNextRow;

	
	//values are null, but will be setup later
	lvRow = new TextParamValue<BoundInt>(new LineFormat(0, Justf::LEFT), LEVEL, &actor.getStat(StatType::LEVEL));
	hpRow = new TextParamCurrMaxValue(new LineFormat(1, Justf::LEFT), HP, &actor.getStat(StatType::HP));
	mpRow = new TextParamCurrMaxValue(new LineFormat(2, Justf::LEFT), MP, &actor.getStat(StatType::MP), 4);
	strengthRow = new TextParamValue<BoundInt>(new LineFormat(3, Justf::LEFT), STRENGTH, &actor.getStat(StatType::STRENGTH));
	defenseRow = new TextParamValue<BoundInt>(new LineFormat(4, Justf::LEFT), DEFENSE, &actor.getStat(StatType::DEFENSE));
	intelRow = new TextParamValue<BoundInt>(new LineFormat(5, Justf::LEFT), INTELLIGENCE, &actor.getStat(StatType::INTELLIGENCE));
	willRow = new TextParamValue<BoundInt>(new LineFormat(6, Justf::LEFT), WILL, &actor.getStat(StatType::WILL));
	agilityRow = new TextParamValue<BoundInt>(new LineFormat(7, Justf::LEFT), AGILITY, &actor.getStat(StatType::AGILITY));
	expRow = new TextParamValue<BoundInt>(new LineFormat(8, Justf::LEFT), EXP, &actor.getStat(StatType::EXP));

	int toNext = alg::toNext(actor);
	std::ostringstream oss;
	oss << TO_NEXT << ' ' << toNext;

	toNextRow = new TextPiece(new LineFormat(9, Justf::LEFT), oss.str());

	statusContent->addPiece(lvRow);
	statusContent->addPiece(hpRow); 
	statusContent->addPiece(mpRow);
	statusContent->addPiece(strengthRow);
	statusContent->addPiece(defenseRow);
	statusContent->addPiece(intelRow);
	statusContent->addPiece(willRow);
	statusContent->addPiece(agilityRow);
	statusContent->addPiece(expRow);
	statusContent->addPiece(toNextRow);

	setDialogControl(statusContent, dWin, r);
//	controlCache.insert(std::make_pair("MM_STATUS", statusContent));
}

void DialogBuilder::buildInventory(DialogWindow& dWin, Rect r)
{
	ItemBrowser* invBrowser = new ItemBrowser;

	invBrowser->setInventory(&rm->getInventory());
	
	invBrowser->runFilter(
		[](OwnedItemRecord* record) {
		
		if (record == nullptr || record->getPossession() == nullptr)
			return;

		GameItem* item = record->getPossession()->item;
		if (item->type != GameItemType::USABLE)
		{
			record->selectable = false;
		}

	});

	setDialogControl(invBrowser, dWin, r);
}

void DialogBuilder::buildCenteredTextWin(DialogWindow& dWin, Rect r)
{
	TextLabel* descLbl = new TextLabel;
	descLbl->setFormat(new CenteredFormat);
	
	setDialogControl(descLbl, dWin, r);
}

void DialogBuilder::buildConfigMenu(DialogWindow& dWin, Rect r)
{
	ConfigMenu* configMenu = new ConfigMenu(rm);

	setDialogControl(configMenu, dWin, r);
}

void DialogBuilder::buildAutoMap(DialogWindow& dWin, Rect r)
{
	AutoMap* autoMap = new AutoMap();
	autoMap->setCurrMap(rm->currMap);
	
	setDialogControl(autoMap, dWin, r);
}

void DialogBuilder::buildEquipDialog(DialogWindow& dWin, Rect r, Actor& actor)
{
	EquipControl* eControl = new EquipControl(actor);
	eControl->setInventory(rm->getInventory());

	setDialogControl(eControl, dWin, r);

}











