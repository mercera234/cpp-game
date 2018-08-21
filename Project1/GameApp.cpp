#include "GameApp.h"
#include "curses.h"
#include "LineItem.h"
#include <ctime>
#include "GameStateManager.h"
#include "TitleScreenState.h"
#include "ExploreState.h"
#include "GameInput.h"

GameApp::GameApp()
{	
	//setup input manager and default keys
	inputManager.setInput('c', GameInput::OK_INPUT);
	inputManager.setInput('x', GameInput::CANCEL_INPUT);
	inputManager.setInput('d', GameInput::OPEN_MENU_INPUT);

	inputManager.setInput('C', GameInput::OK_INPUT);
	inputManager.setInput('X', GameInput::CANCEL_INPUT);
	inputManager.setInput('D', GameInput::OPEN_MENU_INPUT);

	inputManager.setInput(KEY_DOWN, GameInput::DOWN_INPUT);
	inputManager.setInput(KEY_UP, GameInput::UP_INPUT);
	inputManager.setInput(KEY_LEFT, GameInput::LEFT_INPUT);
	inputManager.setInput(KEY_RIGHT, GameInput::RIGHT_INPUT);

	//set up inputs for debugging
	inputManager.setInput(CTRL_F, GameInput::FIGHT_TRIGGER);
	inputManager.setInput(KEY_ESC, GameInput::QUIT_INPUT);

	tui.init();
	
	TitleScreenState::getInstance();
}

int GameApp::getInput()
{
	int input = getch();
	return inputManager.getInput(input);
}

MenuItem* GameApp::menuDriver(int input, AbstractMenu* m) //static
{
	MenuItem* item = nullptr;
	int retval = -1;
	switch (input)
	{
	case GameInput::DOWN_INPUT: retval = m->driver(REQ_DOWN_ITEM); break;
	case GameInput::UP_INPUT: retval = m->driver(REQ_UP_ITEM); break;
	case GameInput::LEFT_INPUT: retval = m->driver(REQ_LEFT_ITEM); break;
	case GameInput::RIGHT_INPUT: retval = m->driver(REQ_RIGHT_ITEM); break;
		/*case KEY_PPAGE: m->driver(REQ_SCR_DPAGE); break;
		case KEY_NPAGE: m->driver(REQ_SCR_UPAGE); break;*/
	case GameInput::OK_INPUT:
		item = m->getCurrentItem();
		break;
	default: break;
	}

	return item;
}

bool GameApp::run()
{
	//input event loop
	GameStateManager stateMngr;
	stateMngr.setState(TitleScreenState::getInstance());
	//((TitleScreenState*)TitleScreenState::getInstance())->setWindow(screen);
	
	bool playing = true;

	/*resize the terminal once before first draw. 
	Having it resize in the loop below before every draw causes issues.
	Lots of black gets drawn where there shouldn't be any*/
	resize_term(screenHeight, screenWidth);

	while (playing)
	{
		//draw
		stateMngr.draw();
		doupdate();

		//input/process
		switch (int input = getInput())
		{
		default:
			playing = stateMngr.processInput(input);
			break;
		}
	}

	return true;
}



void GameApp::fight(void* dataPtr, int purpose)
{
	//GameApp* g = (GameApp*)dataPtr;
	//if (purpose == INIT_PURPOSE)
	//{
	//	//initialize enemy pool
	//	//initialize fight menu
	//	int fightWinHeight = 6;
	//	WINDOW* fightWin = newwin(fightWinHeight, 16, screenHeight - fightWinHeight, 1);
	//	//g->fightMenu = new Menu(fightWin, 4, 1);
	//	//fightmenu will be dynamically altered since it is different for every character

	//	g->fightInSession = false;
	//}

	//if (purpose == RUN_PURPOSE)
	//{
	//	if (g->fightInSession == false) //display intro panel
	//	{
	//		g->fightInSession = true; //true for next loop
	//		std::string header = "They've come to kill you!";

	//		//render panel
	//		clear();
	//		mvaddstr((screenHeight - 1) / 2, (screenWidth - header.length()) / 2, header.c_str());
	//		wnoutrefresh(stdscr);
	//	}
	//	else //process fight logic
	//	{
	//		//decide who fights first,
	//		//if player then build fight menu
	//		//!!!stub!!!
	//		g->fightMenu.setItem(new LineItem("Run", 0, 0));

	//		//handle input
	//		if (g->input != -1) //process input
	//		{
	//			switch(g->input)
	//			{ 
	//				case CANCEL_INPUT: //this is temporary, exiting back to title will be done from status screen
	//				g->stateManager.pop(g);
	//				return;
	//			}
	//		
	//		}
	//		clear();
	//		wnoutrefresh(stdscr);
	//		g->fightMenu.draw();
	//	}

	//	
	//}

	//if (purpose == SHUTDOWN_PURPOSE)
	//{
	//	
	//}
}


void GameApp::mainMenuState(void* dataPtr, int purpose)
{
	//GameApp* g = (GameApp*)dataPtr;
	//
	//if (purpose == INIT_PURPOSE)
	//{
	//	g->initMainMenu();
	//}

	//if (purpose == RUN_PURPOSE)
	//{
	//	if (g->input != NO_INPUT) //process input
	//	{
	//		MenuResponse* resp = g->menuDriver(g->mainMenu, g->input);

	//		if (resp->itemChosen)
	//		{
	//			switch (resp->index)
	//			{
	//			case MainMenuOption::CONFIG:
	//				g->stateManager.push(config, g);
	//				return;
	//			case MainMenuOption::MAIN_QUIT:
	//				delete resp;
	//				g->stateManager.popAll(g);
	//				return;
	//			case CANCEL:
	//				delete resp;
	//				g->stateManager.pop(g);
	//				return;
	//			}
	//			
	//		}
	//		delete resp;
	//		
	//	}

	//	erase();
	//	wnoutrefresh(stdscr);
	//	g->mainMenu.draw();
	//}

	//if (purpose == SHUTDOWN_PURPOSE)
	//{
	//	
	//}
}


//void GameApp::config(void* dataPtr, int purpose)
//{
//	GameApp* g = (GameApp*)dataPtr;
//	if (purpose == INIT_PURPOSE)
//	{
//		WINDOW* frame = newwin(screenHeight, screenWidth, 0, 0);//fill the screen (!memory leak!)
//		WINDOW* configWin = derwin(frame, 8, 30, 3, 15); //dimensions here are for testing!
//		g->configMenu = new Menu(configWin, 8, 1);
//
//		g->configMenu->setItem("OK", "", 0, 0);
//		g->configMenu->setItem("Cancel", "", 1, 1);
//
//	}
//
//	if (purpose == RUN_PURPOSE)
//	{
//
//		if (g->input != NO_INPUT) //process input
//		{
//			MenuResponse* resp = g->menuDriver(g->configMenu, g->input);
//
//			if (resp->itemChosen)
//			{
//				switch (resp->index)
//				{
//					
//				}
//
//			}
//			delete resp;
//
//		}
//
//		g->configMenu->draw();
//
//	}
//
//	if (purpose == SHUTDOWN_PURPOSE)
//	{
//		delete g->configMenu;
//	}
//}


//MenuResponse* GameApp::mainMenuDriver(int input)
//{
//	MenuResponse* resp = new MenuResponse;
//	resp->itemChosen = false;
//	switch (input)
//	{
//	case GameInput::DOWN_INPUT: mainMenu.driver(REQ_DOWN_ITEM); break;
//	case GameInput::UP_INPUT: mainMenu.driver(REQ_UP_ITEM); break;
//		//LEFT and RIGHT are not implemented for this menu
//	case GameInput::OK_INPUT:
//		resp->index = mainMenu.getCurrentIndex();
//		resp->itemChosen = true;
//		break;
//	case GameInput::CANCEL_INPUT:
//		resp->index = CANCEL;
//		resp->itemChosen = true;
//		break;
//	default: break;
//	}
//
//	return resp;
//}
//


//void GameApp::initMainMenu()
//{
//	//int mainWidth = 11;
//
//	////WINDOW* menuFrame = newwin(10, mainWidth, 0, SCREEN_HEIGHT - mainWidth);
//	////	WINDOW* menuWin = derwin(menuFrame, 8, 8, 1, 1);
//
//	//WINDOW* menuWin = newwin(8, 8, 1, 1);
//	////mainMenu = new Menu(menuWin, 8, 1);
//
//	//mainMenu.setItem(new LineItem("Item", 0, 0));
//	//mainMenu.setItem(new LineItem("Equip", 1, 1));
//	//mainMenu.setItem(new LineItem("Status", 2, 2));
//	//mainMenu.setItem(new LineItem("Skill", 3, 3));
//	//mainMenu.setItem(new LineItem("Config", 4, 4));
//	//mainMenu.setItem(new LineItem("Map", 5, 5));
//	//mainMenu.setItem(new LineItem("Save", 6, 6));
//	//mainMenu.setItem(new LineItem("Quit", 7, 7));
//}

GameApp::~GameApp()
{
	
}