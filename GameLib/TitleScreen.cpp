#include "TitleScreen.h"
#include "LineItem.h"
#include "defaults.h"
#include "menu_drivers.h"
#include "ExitCode.h"
#include "Style.h"
#include "TitleMenuOptions.h"


TitleScreen::TitleScreen()
{
	win = TUI::winMgr.newWin(gameScreenHeight, gameScreenWidth, 0, 0);
	
	builder.buildTitleMenu(titleMenu, win, titleSpace);
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

	int yPos = getPosition(Justf::CENTER, titleSpace, 1);
	int xPos = getPosition(Justf::CENTER, getmaxx(win), GAME_TITLE.length());
	mvwprintw(win, yPos, xPos, GAME_TITLE.c_str());
	wnoutrefresh(win);
	titleMenu.draw();
}


