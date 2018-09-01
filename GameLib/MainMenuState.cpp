#include "MainMenuState.h"
#include "GameStateManager.h"
#include "curses.h"
#include "TitleScreenState.h"
#include "ExploreState.h"
#include "GameInput.h"
#include "LineItem.h"
#include "GameApp.h"
#include "defaults.h"
#include "menu_drivers.h"

GameState* MainMenuState::instance = nullptr;

GameState* MainMenuState::getInstance()
{
	if (instance == nullptr)
		instance = new MainMenuState;

	return instance;
}

MainMenuState::MainMenuState()
{
	win = newwin(screenHeight, screenWidth, 0, 0);

	mm.setWindow(newwin(screenHeight, screenWidth, 0, 0));
	//mm.addPlayerParty(playerParty);

	/*cm.setCaller(this);

	topFrameHeight = 6;
	bottomFrameHeight = screenHeight - topFrameHeight + 1;
	leftFrameWidth = 20;
	rightFrameWidth = screenWidth - leftFrameWidth;


	setupMainMenu();
	setupPlayerMenu();
	
	bodyWindow = newwin(bottomFrameHeight - 2, rightFrameWidth - 2, 6, leftFrameWidth+ 1);
	bodyFrame = new Frame(newwin(bottomFrameHeight, rightFrameWidth, 5, leftFrameWidth), nullptr);


	cm.registerControl(mainFrame, KEY_LISTENER, mainMenuCallback);
	cm.registerControl(playerFrame, KEY_LISTENER, playerMenuCallback);
	cm.registerControl(bodyFrame, KEY_LISTENER, nullptr);
	cm.setFocus(mainFrame);*/
}


void MainMenuState::loadState()
{
//	mainMenu.setCurrentItem(MainMenuOption::INVENTORY);
}
//
//void MainMenuState::setupMainMenu()
//{
//	mainMenu.resetItems(4, 2);
//
//
//	mainMenu.setWindow(newwin(topFrameHeight - 2, leftFrameWidth - 2, 1, 1)); //more windows to be deleted later
//
//	mainMenu.setItemWidth(6);
//	mainMenu.setColSepLength(0);
//
//	mainMenu.setItem(new LineItem("Item", 0, MainMenuOption::INVENTORY), 0, 0);
//	mainMenu.setItem(new LineItem("Equip", 1, MainMenuOption::EQUIP), 1, 0);
//	mainMenu.setItem(new LineItem("Status", 2, MainMenuOption::STATUS), 2, 0);
//	mainMenu.setItem(new LineItem("Skill", 3, MainMenuOption::SKILL), 3, 0);
//	mainMenu.setItem(new LineItem("Config", 4, MainMenuOption::CONFIG), 0, 1);
//	mainMenu.setItem(new LineItem("Map", 5, MainMenuOption::MAP), 1, 1);
//	mainMenu.setItem(new LineItem("Save", 6, MainMenuOption::SAVE), 2, 1);
//	mainMenu.setItem(new LineItem("Quit", 7, MainMenuOption::MAIN_QUIT), 3, 1);
//
//	mainMenu.setWrapAround(false);
//	mainMenu.setFocus(true);
//	mainMenu.post(true);
//
//	mainFrame = new Frame(newwin(topFrameHeight, leftFrameWidth, 0, 0), &mainMenu);
//}
//
//
//void MainMenuState::setupPlayerMenu()
//{
//	playerMenu.resetItems(1, 1);
//	playerMenu.setWindow(newwin(bottomFrameHeight - 2, leftFrameWidth - 2, 6, 1));
//	playerMenu.setItemHeight(4);
//
//	playerMenu.setItem(new LineItem(player1Name, 0, -1), 0, 0); //temporary
//
//	playerMenu.setWrapAround(false);
//	playerMenu.setFocus(false);
//	playerMenu.post(true);
//
//	playerFrame = new Frame(newwin(bottomFrameHeight, leftFrameWidth, 5, 0), &playerMenu);
//}


void MainMenuState::processInput(GameStateManager& manager, int input)
{
	int exitCode = mm.processInput(input);

	switch (exitCode)
	{
	case ExitCode::GO_BACK: 
		manager.setState(ExploreState::getInstance());
		break;
	case ExitCode::QUIT_TO_TITLE: 
		manager.setState(TitleScreenState::getInstance()); 
		break;
	}
	
}

//
//void MainMenuState::mainMenuCallback(void* caller, void* ptr, int input) //static
//{
//	MainMenuState* state = (MainMenuState*)caller;
//	state->processMainMenuInput((Frame*)ptr, input);
//}
//
//void MainMenuState::playerMenuCallback(void* caller, void* ptr, int input) //static
//{
//	MainMenuState* state = (MainMenuState*)caller;
//	state->processPlayerMenuInput((Frame*)ptr, input);
//}
//
//void MainMenuState::processMainMenuInput(Frame* f, int input)
//{
//	GameStateManager* manager = getManager();
//	if(input == GameInput::CANCEL_INPUT)
//	{
//		manager->setState(ExploreState::getInstance());
//		return;
//	}
//
//
//	MenuItem* item = menuDriver(input, &mainMenu);
//
//	if (item)
//	{
//		switch (((LineItem*)item)->getCrossRef())
//		{
//		case MainMenuOption::STATUS: 
//			cm.setFocus(playerFrame); 
//			playerMenu.setCurrentItem(0);
//			break;
//		case MainMenuOption::MAIN_QUIT: manager->setState(TitleScreenState::getInstance()); break;
//		}
//	}
//}

//void MainMenuState::processPlayerMenuInput(Frame* f, int input)
//{
//	if (input == GameInput::CANCEL_INPUT)
//	{
//		playerMenu.setCurrentItem(NO_CUR_ITEM);
//		cm.setFocus(mainFrame);
//		
//		return;
//	}
//
//
//	MenuItem* item = menuDriver(input, &playerMenu);
//
//	if (item)
//	{
//		
//	}
//}
//
//void MainMenuState::drawBodyWindow()
//{
//	auto it = resourceManager->actors.find(player1Name);
//	Actor a = it->second;
//	
//	BoundInt hpStat = a.getStat(StatType::HP);
//	BoundInt mpStat = a.getStat(StatType::MP);
//	BoundInt strStat = a.getStat(StatType::STRENGTH);
//	BoundInt defStat = a.getStat(StatType::DEFENSE);
//	BoundInt intStat = a.getStat(StatType::INTELLIGENCE);
//	BoundInt wilStat = a.getStat(StatType::WILL);
//	BoundInt agiStat = a.getStat(StatType::AGILITY);
//	BoundInt expStat = a.getStat(StatType::EXP);
//
//	mvwprintw(bodyWindow, 0, 0, "HP %+4u/%+4u", hpStat.getCurr(), hpStat.getMax());
//	mvwprintw(bodyWindow, 1, 0, "MP %+4u/%+4u", mpStat.getCurr(), mpStat.getMax());
//	mvwprintw(bodyWindow, 2, 0, "Strength %+3u", strStat.getCurr());
//	mvwprintw(bodyWindow, 3, 0, "Defense %+3u", defStat.getCurr());
//	mvwprintw(bodyWindow, 4, 0, "Intel %+3u", intStat.getCurr());
//	mvwprintw(bodyWindow, 5, 0, "Will %+3u", wilStat.getCurr());
//	mvwprintw(bodyWindow, 6, 0, "Agility %+3u", agiStat.getCurr());
//	mvwprintw(bodyWindow, 7, 0, "Exp %+3u", expStat.getCurr());
//
//	touchwin(bodyWindow);
//	wnoutrefresh(bodyWindow);
//}

void MainMenuState::draw()
{


	werase(win);
	wnoutrefresh(win);

	mm.draw();

	/*cm.draw();

	if(cm.getFocus() == playerFrame)
		drawBodyWindow();*/
}