#include "DialogBuilder.h"
#include "LineItem.h"
#include "defaults.h"
#include "MainMenuOption.h"
#include "TextBoard.h"
#include "TextParamValue.h"
#include "LineFormat.h"



void DialogBuilder::buildMainMenu(DialogWindow& dWin, Rect r)
{
	GridMenu* mainMenu = new GridMenu(); //the dialogWindow will delete

	mainMenu->resetItems(4, 2);


	mainMenu->setWindow(TUI::winMgr.newWin(r.height - 2, r.width - 2, r.origin.y + 1, r.origin.x + 1));

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

	dWin.setControl(mainMenu);
	dWin.setWindow(TUI::winMgr.newWin(r.height, r.width, r.origin.y, r.origin.x));
}

void DialogBuilder::buildPlayerMenu(DialogWindow& dWin, Rect r)
{
	GridMenu* playerMenu = new GridMenu(); //the dialogWindow will delete
	playerMenu->resetItems(playerCapacity, 1);
	playerMenu->setWindow(TUI::winMgr.newWin(r.height - 2, r.width - 2, r.origin.y + 1, r.origin.x + 1));
	playerMenu->setItemHeight(4);

	//std::vector<Actor>* allies = &rm->playerParty;
	//auto it = rm->playerParty.begin();

	for (int i = 0; i < playerCapacity; i++)
	{
		std::string name = "";
		if (i < rm->playerParty.size())
			name = rm->playerParty[i].name;

		playerMenu->setItem(new LineItem(name, i, -1), i, 0); //sets blank items to start
	}

	playerMenu->setWrapAround(false);
	playerMenu->setFocus(false);
	playerMenu->post(true);

	dWin.setControl(playerMenu);
	dWin.setWindow(TUI::winMgr.newWin(r.height, r.width, r.origin.y, r.origin.x));

}


void DialogBuilder::buildDesc(DialogWindow& dWin, Rect r)
{
	TextBoard* descContent = new TextBoard();

	descContent->setWindow(TUI::winMgr.newWin(r.height - 2, r.width - 2, r.origin.y + 1, r.origin.x + 1));

	dWin.setControl(descContent);
	dWin.setWindow(TUI::winMgr.newWin(r.height, r.width, r.origin.y, r.origin.x));


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
}


void DialogBuilder::buildMainMenuBody(DialogWindow& dWin, Rect r)
{
	TextBoard* bodyContent = new TextBoard();

	bodyContent->setWindow(TUI::winMgr.newWin(r.height - 2, r.width - 2, r.origin.y + 1, r.origin.x + 1));
	dWin.setControl(bodyContent);
	dWin.setWindow(TUI::winMgr.newWin(r.height, r.width, r.origin.y, r.origin.x));

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

	controlCache.insert(std::make_pair("DEFAULT BODY", bodyContent));
}

//TODO status window needs some rethinking, like adding a map to identify text pieces, and adding an Actor
//void DialogBuilder::buildMainMenuStatus(DialogWindow& dWin, Rect r)
//{
//	TextBoard* statusContent = new TextBoard();
//
//	statusContent->setWindow(TUI::winMgr.newWin(r.height - 2, r.width - 2, r.origin.y + 1, r.origin.x + 1));
//	dWin.setControl(statusContent);
//	dWin.setWindow(TUI::winMgr.newWin(r.height, r.width, r.origin.y, r.origin.x));
//
//	TextParamCurrMaxValue* hpRow, *mpRow;
//	TextParamValue<BoundInt>* strengthRow, *defenseRow, *intelRow, *willRow, *agilityRow, *expRow;
//
//	//values are null, but will be setup later
//	hpRow = new TextParamCurrMaxValue(new LineFormat(0, Justf::LEFT), HP, nullptr);
//	mpRow = new TextParamCurrMaxValue(new LineFormat(1, Justf::LEFT), MP, nullptr, 4);
//	strengthRow = new TextParamValue<BoundInt>(new LineFormat(2, Justf::LEFT), STRENGTH, nullptr);
//	defenseRow = new TextParamValue<BoundInt>(new LineFormat(3, Justf::LEFT), DEFENSE, nullptr);
//	intelRow = new TextParamValue<BoundInt>(new LineFormat(4, Justf::LEFT), INTELLIGENCE, nullptr);
//	willRow = new TextParamValue<BoundInt>(new LineFormat(5, Justf::LEFT), WILL, nullptr);
//	agilityRow = new TextParamValue<BoundInt>(new LineFormat(6, Justf::LEFT), AGILITY, nullptr);
//	expRow = new TextParamValue<BoundInt>(new LineFormat(7, Justf::LEFT), EXP, nullptr);
//
//	statusContent->addPiece(hpRow); //the bound int values will need to be setup later
//	statusContent->addPiece(mpRow);
//	statusContent->addPiece(strengthRow);
//	statusContent->addPiece(defenseRow);
//	statusContent->addPiece(intelRow);
//	statusContent->addPiece(willRow);
//	statusContent->addPiece(agilityRow);
//	statusContent->addPiece(expRow);
//
//	controlCache.insert(std::make_pair("MM_STATUS", statusContent));
//}