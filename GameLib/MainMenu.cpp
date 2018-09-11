#include <assert.h>
#include "MainMenu.h"
#include "LineItem.h"
#include "menu_drivers.h"
#include "GameInput.h"
#include "TextBoard.h"
#include "TextParamValue.h"
#include "defaults.h"

MainMenu::MainMenu()
{
	cm.setCaller(this);

	//link frames to controls
	mainFrame.setControl(&mainMenu);
	playerFrame.setControl(&playerMenu);
	bodyFrame.setControl(&bodyContent);

	//setup cmds
	mainMenuCmd.setReceiver(this);
	mainMenuCmd.setAction(&MainMenu::processMainMenuInput);

	playerMenuCmd.setReceiver(this);
	playerMenuCmd.setAction(&MainMenu::processPlayerMenuInput);

	setupBodyFields();
	setupStatusFields();



	//register controls
	cm.registerControl(&mainFrame, KEY_LISTENER, &mainMenuCmd);
	cm.registerControl(&playerFrame, KEY_LISTENER, &playerMenuCmd);
	cm.registerControl(&bodyFrame, 0, nullptr);
	cm.setFocus(&mainFrame);
}

void MainMenu::setupStatusFields()
{
	//values are null, but will be setup later
	hpRow = new TextParamCurrMaxValue(new LineFormat(0, Justf::LEFT), HP, nullptr);
	mpRow = new TextParamCurrMaxValue(new LineFormat(1, Justf::LEFT), MP, nullptr, 4);
	strengthRow = new TextParamValue<BoundInt>(new LineFormat(2, Justf::LEFT), STRENGTH, nullptr);
	defenseRow = new TextParamValue<BoundInt>(new LineFormat(3, Justf::LEFT), DEFENSE, nullptr);
	intelRow = new TextParamValue<BoundInt>(new LineFormat(4, Justf::LEFT), INTELLIGENCE, nullptr);
	willRow = new TextParamValue<BoundInt>(new LineFormat(5, Justf::LEFT), WILL, nullptr);
	agilityRow = new TextParamValue<BoundInt>(new LineFormat(6, Justf::LEFT), AGILITY, nullptr);
	expRow = new TextParamValue<BoundInt>(new LineFormat(7, Justf::LEFT), EXP, nullptr);

	statusContent.addPiece(hpRow); //the bound int values will need to be setup later
	statusContent.addPiece(mpRow);
	statusContent.addPiece(strengthRow);
	statusContent.addPiece(defenseRow);
	statusContent.addPiece(intelRow);
	statusContent.addPiece(willRow);
	statusContent.addPiece(agilityRow);
	statusContent.addPiece(expRow);
}

void MainMenu::setupBodyFields()
{
	gold = new TextParamValue<BoundInt>(new LineFormat(0, Justf::LEFT), GOLD$, nullptr);
	steps = new TextParamValue<BoundInt>(new LineFormat(1, Justf::LEFT), STEPS, nullptr);
	enemiesKilled = new TextParamValue<BoundInt>(new LineFormat(2, Justf::LEFT), ENEMIES_KILLED, nullptr);
	battlesWon = new TextParamValue<BoundInt>(new LineFormat(3, Justf::LEFT), BATTLES_WON, nullptr);
	bodyContent.addPiece(gold);
	bodyContent.addPiece(steps);
	bodyContent.addPiece(enemiesKilled);
	bodyContent.addPiece(battlesWon);
}


void MainMenu::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);

	int controlHeight = getmaxy(win);
	int controlWidth = getmaxx(win);

	topFrameHeight = 6;
	bottomFrameHeight = controlHeight - topFrameHeight + 1;
	leftFrameWidth = 20;
	rightFrameWidth = controlWidth - leftFrameWidth;

	setupMainMenu();
	setupPlayerMenu();

	bodyContent.setWindow(newwin(bottomFrameHeight - 2, rightFrameWidth - 2, 6, leftFrameWidth + 1));
	bodyFrame.setWindow(newwin(bottomFrameHeight, rightFrameWidth, 5, leftFrameWidth));

	statusContent.setWindow(newwin(bottomFrameHeight - 2, rightFrameWidth - 2, 6, leftFrameWidth + 1));
}


void MainMenu::addPlayerParty(std::vector<Actor*>& allies)
{
	assert(allies.size() <= playerCapacity);
	this->allies = allies;

	int row = 0;
	for each (Actor* actor in allies)
	{
		LineItem* item = (LineItem*)playerMenu.getItem(row++, 0);

		item->name = actor->name;
	}
}


void MainMenu::setupMainMenu()
{
	mainMenu.resetItems(4, 2);


	mainMenu.setWindow(newwin(topFrameHeight - 2, leftFrameWidth - 2, 1, 1)); //more windows to be deleted later

	mainMenu.setItemWidth(6);
	mainMenu.setColSepLength(0);

	mainMenu.setItem(new LineItem(S_ITEM, 0, MainMenuOption::INVENTORY), 0, 0);
	mainMenu.setItem(new LineItem(S_EQUIP, 1, MainMenuOption::EQUIP), 1, 0);
	mainMenu.setItem(new LineItem(S_STATUS, 2, MainMenuOption::STATUS), 2, 0);
	mainMenu.setItem(new LineItem(S_SKILL, 3, MainMenuOption::SKILL), 3, 0);
	mainMenu.setItem(new LineItem(S_CONFIG, 4, MainMenuOption::CONFIG), 0, 1);
	mainMenu.setItem(new LineItem(S_MAP, 5, MainMenuOption::MAP), 1, 1);
	mainMenu.setItem(new LineItem(S_SAVE, 6, MainMenuOption::SAVE), 2, 1);
	mainMenu.setItem(new LineItem(S_QUIT, 7, MainMenuOption::MAIN_QUIT), 3, 1);

	mainMenu.setWrapAround(false);
	mainMenu.setFocus(true);
	mainMenu.post(true);

	mainMenu.setCurrentItem(0);

	mainFrame.setWindow(newwin(topFrameHeight, leftFrameWidth, 0, 0));
	
}


void MainMenu::setupPlayerMenu()
{
	playerMenu.resetItems(playerCapacity, 1);
	playerMenu.setWindow(newwin(bottomFrameHeight - 2, leftFrameWidth - 2, 6, 1));
	playerMenu.setItemHeight(4);
	
	for (int i = 0; i < playerCapacity; i++)
	{
		playerMenu.setItem(new LineItem("", i, -1), i, 0); //temporary
	}
	

	playerMenu.setWrapAround(false);
	playerMenu.setFocus(false);
	playerMenu.post(true);

	playerFrame.setWindow(newwin(bottomFrameHeight, leftFrameWidth, 5, 0));
	
}


int MainMenu::processInput(int input)
{
	return cm.handleInput(input);
}


int MainMenu::processMainMenuInput(Controllable* c, int input)
{
	if (input == GameInput::CANCEL_INPUT)
	{		
		return ExitCode::GO_BACK;
	}


	MenuItem* item = menuDriver(input, &mainMenu);

	if (item)
	{
		switch (((LineItem*)item)->getCrossRef())
		{
		case MainMenuOption::STATUS:
			cm.setFocus(&playerFrame);
			playerMenu.setCurrentItem(0);
			bodyFrame.setControl(&statusContent);
			setupStatusContent();
			break;
		case MainMenuOption::MAIN_QUIT: 
			return ExitCode::QUIT_TO_TITLE;
		}
	}
	else //no item selected, render default data
	{
		setupHubContent();
	}

	return HANDLED;
}

void MainMenu::setResourceManager(ResourceManager* resourceManagerIn)
{
	resourceManager = resourceManagerIn;
	setupHubContent();
}

void MainMenu::setupHubContent()
{
	gold->setValue(&resourceManager->theData.retrieveIntData(GOLD$));
	steps->setValue(&resourceManager->theData.retrieveIntData(STEPS));
	enemiesKilled->setValue(&resourceManager->theData.retrieveIntData(ENEMIES_KILLED));
	battlesWon->setValue(&resourceManager->theData.retrieveIntData(BATTLES_WON));
}

int MainMenu::processPlayerMenuInput(Controllable* c, int input)
{
	//right now this only handles the status menu!!!
	if (input == GameInput::CANCEL_INPUT)
	{
		playerMenu.setCurrentItem(NO_CUR_ITEM);
		cm.setFocus(&mainFrame);
		bodyFrame.setControl(&bodyContent);
		return HANDLED;
	}


	MenuItem* item = menuDriver(input, &playerMenu);

	setupStatusContent();

	return HANDLED;
}

void MainMenu::setupStatusContent()
{
	MenuItem* item = playerMenu.getCurrentItem();

	if (item && ((LineItem*)item)->name.compare("") != 0)
	{
		Actor* ally = allies[item->index];
		hpRow->setValue(&ally->getStat(StatType::HP));
		mpRow->setValue(&ally->getStat(StatType::MP));
		strengthRow->setValue(&ally->getStat(StatType::STRENGTH));
		defenseRow->setValue(&ally->getStat(StatType::DEFENSE));
		intelRow->setValue(&ally->getStat(StatType::INTELLIGENCE));
		willRow->setValue(&ally->getStat(StatType::WILL));
		agilityRow->setValue(&ally->getStat(StatType::AGILITY));
		expRow->setValue(&ally->getStat(StatType::EXP));
	}
}


void MainMenu::draw()
{
	werase(win);
	wnoutrefresh(win);
	cm.draw();
}

MainMenu::~MainMenu()
{
}