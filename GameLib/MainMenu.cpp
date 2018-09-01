#include <assert.h>
#include "MainMenu.h"
#include "LineItem.h"
#include "menu_drivers.h"
#include "GameInput.h"

MainMenu::MainMenu()
{
	cm.setCaller(this);

	topFrameHeight = 6;
	bottomFrameHeight = screenHeight - topFrameHeight + 1;
	leftFrameWidth = 20;
	rightFrameWidth = screenWidth - leftFrameWidth;

	setupMainMenu();
	setupPlayerMenu();

	bodyWindow = newwin(bottomFrameHeight - 2, rightFrameWidth - 2, 6, leftFrameWidth + 1);
	bodyFrame.setWindow(newwin(bottomFrameHeight, rightFrameWidth, 5, leftFrameWidth));
	bodyFrame.setControl(nullptr);

	mainMenuCmd.setReceiver(this);
	mainMenuCmd.setAction(&MainMenu::processMainMenuInput);

	playerMenuCmd.setReceiver(this);
	playerMenuCmd.setAction(&MainMenu::processPlayerMenuInput);

	cm.registerControl(&mainFrame, KEY_LISTENER, &mainMenuCmd);
	cm.registerControl(&playerFrame, KEY_LISTENER, &playerMenuCmd);
	cm.registerControl(&bodyFrame, KEY_LISTENER, nullptr);
	cm.setFocus(&mainFrame);
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

	mainMenu.setItem(new LineItem("Item", 0, MainMenuOption::INVENTORY), 0, 0);
	mainMenu.setItem(new LineItem("Equip", 1, MainMenuOption::EQUIP), 1, 0);
	mainMenu.setItem(new LineItem("Status", 2, MainMenuOption::STATUS), 2, 0);
	mainMenu.setItem(new LineItem("Skill", 3, MainMenuOption::SKILL), 3, 0);
	mainMenu.setItem(new LineItem("Config", 4, MainMenuOption::CONFIG), 0, 1);
	mainMenu.setItem(new LineItem("Map", 5, MainMenuOption::MAP), 1, 1);
	mainMenu.setItem(new LineItem("Save", 6, MainMenuOption::SAVE), 2, 1);
	mainMenu.setItem(new LineItem("Quit", 7, MainMenuOption::MAIN_QUIT), 3, 1);

	mainMenu.setWrapAround(false);
	mainMenu.setFocus(true);
	mainMenu.post(true);

	mainMenu.setCurrentItem(0);

	mainFrame.setWindow(newwin(topFrameHeight, leftFrameWidth, 0, 0));
	mainFrame.setControl(&mainMenu);
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
	playerFrame.setControl(&playerMenu);
}


int MainMenu::mainMenuCallback(void* caller, void* ptr, int input) //static
{
	MainMenu* mm = (MainMenu*)caller;
	return mm->processMainMenuInput((Frame*)ptr, input);
}

int MainMenu::playerMenuCallback(void* caller, void* ptr, int input) //static
{
	MainMenu* mm = (MainMenu*)caller;
	return mm->processPlayerMenuInput((Frame*)ptr, input);
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
			break;
		case MainMenuOption::MAIN_QUIT: 
			return ExitCode::QUIT_TO_TITLE;
		}
	}

	return HANDLED;
}

int MainMenu::processPlayerMenuInput(Controllable* c, int input)
{
	if (input == GameInput::CANCEL_INPUT)
	{
		playerMenu.setCurrentItem(NO_CUR_ITEM);
		cm.setFocus(&mainFrame);

		return HANDLED;
	}


	MenuItem* item = menuDriver(input, &playerMenu);

	if (item)
	{
		
	}

	return HANDLED;
}

void MainMenu::drawBodyWindow()
{
	LineItem* item = (LineItem*)playerMenu.getCurrentItem();

	if (item->name.compare("") == 0) 
		return;

	Actor* ally = allies[item->index];

	BoundInt hpStat = ally->getStat(StatType::HP);
	BoundInt mpStat = ally->getStat(StatType::MP);
	BoundInt strStat = ally->getStat(StatType::STRENGTH);
	BoundInt defStat = ally->getStat(StatType::DEFENSE);
	BoundInt intStat = ally->getStat(StatType::INTELLIGENCE);
	BoundInt wilStat = ally->getStat(StatType::WILL);
	BoundInt agiStat = ally->getStat(StatType::AGILITY);
	BoundInt expStat = ally->getStat(StatType::EXP);

	mvwprintw(bodyWindow, 0, 0, "HP %+4u/%+4u", hpStat.getCurr(), hpStat.getMax());
	mvwprintw(bodyWindow, 1, 0, "MP %+4u/%+4u", mpStat.getCurr(), mpStat.getMax());
	mvwprintw(bodyWindow, 2, 0, "Strength %+3u", strStat.getCurr());
	mvwprintw(bodyWindow, 3, 0, "Defense %+3u", defStat.getCurr());
	mvwprintw(bodyWindow, 4, 0, "Intel %+3u", intStat.getCurr());
	mvwprintw(bodyWindow, 5, 0, "Will %+3u", wilStat.getCurr());
	mvwprintw(bodyWindow, 6, 0, "Agility %+3u", agiStat.getCurr());
	mvwprintw(bodyWindow, 7, 0, "Exp %+3u", expStat.getCurr());

	touchwin(bodyWindow);
	wnoutrefresh(bodyWindow);
}

void MainMenu::draw()
{
	cm.draw();

	if (cm.getFocus() == &playerFrame)
		drawBodyWindow();
}

MainMenu::~MainMenu()
{
}
