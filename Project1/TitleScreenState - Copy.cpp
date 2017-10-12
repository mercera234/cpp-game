#include "TitleScreenState.h"
#include "LineItem.h"
#include "GameStateManager.h"

//Title Menu options
TitleScreenState::TitleScreenState(WINDOW* menuWin)
	: titleMenu(menuWin, 3, 1)
{
	titleMenu.setItem(new LineItem("New Game", (int)TitleOption::NEW_GAME, -1));
	titleMenu.setItem(new LineItem("Load Game", (int)TitleOption::LOAD_GAME, -1));
	titleMenu.setItem(new LineItem("Quit Game", (int)TitleOption::QUIT_GAME, -1));
	
	titleMenu.setCurrentItem(0);
	int markSide = LEFT_MARK;
	titleMenu.setMarkSide(markSide);
	titleMenu.setWrapAround(false);
	titleMenu.post(true);
}

void TitleScreenState::processInput(int input)
{
	MenuItem* item = AbstractMenu::basicMenuDriver(input, &titleMenu);
	
	
	if (item) //TODO not sure if this will work
	{
		switch ((TitleOption)item->index)
		{
		case TitleOption::NEW_GAME:
			
			//g->stateManager.push(explore, g);
			
			return;
		case TitleOption::LOAD_GAME:
			break;
		case TitleOption::EDIT_GAME:
			break;
		case TitleOption::QUIT_GAME:
			active = false;
			return;
		}
	}
}


void TitleScreenState::draw()
{
	mvwprintw(stdscr, 0, 1, "---TITLE OF GAME---");
	wnoutrefresh(stdscr);
	titleMenu.draw();
}