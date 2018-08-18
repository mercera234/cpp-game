#include "TitleScreenState.h"
#include "LineItem.h"
#include "GameStateManager.h"
#include "ExploreState.h"
#include "GameApp.h"
#include "GameInput.h"

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
TitleScreenState::TitleScreenState() 
{
	WINDOW* menuWin = newwin(4, 25, 1, 1); //TODO window must be deleted later!!
	titleMenu.resetItems(3, 1);
	titleMenu.setWindow(menuWin);
	
	titleMenu.setItem(new LineItem("New Game", 0, TitleMenuOptions::NEW_GAME));
	titleMenu.setItem(new LineItem("Load Game", 1, TitleMenuOptions::LOAD_GAME));
	titleMenu.setItem(new LineItem("Quit Game", 2, TitleMenuOptions::QUIT_GAME));

	titleMenu.setCurrentItem(0);
	titleMenu.setWrapAround(false);
	titleMenu.setFocus(true);
	titleMenu.post(true);
}


void TitleScreenState::processInput(GameStateManager& manager, int input)
{
	LineItem* item = (LineItem*)GameApp::menuDriver(input, &titleMenu);
		
	if (item) //TODO not sure if this will work
	{
		switch (item->getCrossRef())
		{
		case TitleMenuOptions::NEW_GAME:
			manager.setState(ExploreState::getInstance());
			
			return;
		case TitleMenuOptions::LOAD_GAME:
			break;
		case TitleMenuOptions::EDIT_GAME:
			break;
		case TitleMenuOptions::QUIT_GAME:
			manager.setState(nullptr);
			
			//active = false;
			return;
		}
	}
	else //other input may have been received
	{
		if(input == GameInput::QUIT_INPUT) //must be careful here, in case quit input has same value as any title menu options
			manager.setState(nullptr);
	}
}


void TitleScreenState::draw()
{
	mvwprintw(stdscr, 0, 1, "---TITLE OF GAME---");
	wnoutrefresh(stdscr);
	titleMenu.draw();
}