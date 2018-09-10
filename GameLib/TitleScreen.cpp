#include "TitleScreen.h"
#include "LineItem.h"
#include "defaults.h"
#include "menu_drivers.h"
#include "input_return_codes.h"

TitleScreen::TitleScreen()
{
	win = newwin(screenHeight, screenWidth, 0, 0);
	titleMenu.resetItems(3, 1);


	int menuHeight = 4;
	int menuWidth = 12;
	int yPos = (((getmaxy(win) - titleSpace) - menuHeight) / 2) + titleSpace; //centered under title image
	int xPos = (getmaxx(win) - menuWidth) / 2; //centered horizontally
	
	titleMenu.setWindow(newwin(menuHeight, menuWidth, yPos, xPos));

	titleMenu.setItem(new LineItem(S_NEW_GAME, 0, TitleMenuOptions::NEW_GAME));
	titleMenu.setItem(new LineItem(S_LOAD_GAME, 1, TitleMenuOptions::LOAD_GAME));
	titleMenu.setItem(new LineItem(S_QUIT_GAME, 2, TitleMenuOptions::QUIT_GAME));

	titleMenu.setCurrentItem(0);
	titleMenu.setWrapAround(false);
	titleMenu.setFocus(true);
	titleMenu.post(true);
}

int TitleScreen::processInput(int input)
{
	MenuItem* item = menuDriver(input, &titleMenu);

	int exitCode = HANDLED;
	if (item)
	{
		switch (((LineItem*)item)->getCrossRef())
		{
		case TitleMenuOptions::NEW_GAME:
			return ExitCode::START_NEW;
			break;
		case TitleMenuOptions::LOAD_GAME:

			break;
		case TitleMenuOptions::EDIT_GAME:
			break;
		case TitleMenuOptions::QUIT_GAME:
			return ExitCode::TERMINATE;
			break;
			
		}
	}
	return exitCode;
}

void TitleScreen::draw()
{
	werase(win);

	int yPos = (titleSpace - 1) / 2;
	int xPos = (getmaxx(win) - GAME_TITLE.length()) / 2;
	mvwprintw(win, yPos, xPos, GAME_TITLE.c_str());
	wnoutrefresh(win);
	titleMenu.draw();
}

TitleScreen::~TitleScreen()
{
}
