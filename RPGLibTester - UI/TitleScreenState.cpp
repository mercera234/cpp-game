#include "TitleScreenState.h"
#include "LineItem.h"
#include "GameStateManager.h"
#include "TestState.h"

GameState* TitleScreenState::instance = nullptr;

GameState* TitleScreenState::getInstance()
{
	if (instance == nullptr)
	{
		instance = new TitleScreenState;
	}
		
	return instance;
}

//Title Menu options
TitleScreenState::TitleScreenState()//WINDOW* menuWin)
	//: titleMenu(menuWin, 3, 1)
{
	WINDOW* menuWin = newwin(4, 25, 1, 1);
	titleMenu = new GridMenu(menuWin, 3, 1);
	titleMenu->setItem(new LineItem("New Game", (int)TitleOption::NEW_GAME, -1));
	titleMenu->setItem(new LineItem("Load Game", (int)TitleOption::LOAD_GAME, -1));
	titleMenu->setItem(new LineItem("Quit Game", (int)TitleOption::QUIT_GAME, -1));
	
	titleMenu->setCurrentItem(0);
	titleMenu->setWrapAround(false);
	titleMenu->post(true);
}

void TitleScreenState::processInput(GameStateManager& manager, int input)
{
	MenuItem* item = AbstractMenu::basicMenuDriver(input, titleMenu);
	
	
	if (item) //TODO not sure if this will work
	{
		switch ((TitleOption)item->index)
		{
		case TitleOption::NEW_GAME:
			manager.setState(TestState::getInstance());
			
			return;
		case TitleOption::LOAD_GAME:
			break;
		case TitleOption::EDIT_GAME:
			break;
		case TitleOption::QUIT_GAME:
			manager.setState(nullptr);
			
			//active = false;
			return;
		}
	}
}


void TitleScreenState::draw()
{
	mvwprintw(stdscr, 0, 1, "---TITLE OF GAME---");
	wnoutrefresh(stdscr);
	titleMenu->draw();
}