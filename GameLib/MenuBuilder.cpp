#include "MenuBuilder.h"
#include "Style.h"
#include "LineItem.h"
#include "defaults.h"
#include "TitleMenuOptions.h"

void MenuBuilder::buildTitleMenu(GridMenu& menu, WINDOW* win, int titleSpace)
{
	//setup menu
	menu.resetItems(3, 1);
	int menuHeight = 4;
	int menuWidth = 12;
	int yPos = getPosition(Justf::CENTER, getmaxy(win) - titleSpace, menuHeight) + titleSpace; //centered under title image
	int xPos = getPosition(Justf::CENTER, getmaxx(win), menuWidth);

	menu.setWindow(TUI::winMgr.newWin(menuHeight, menuWidth, yPos, xPos));

	menu.setItem(new LineItem(S_NEW_GAME, 0, TitleMenuOptions::NEW_GAME));
	menu.setItem(new LineItem(S_LOAD_GAME, 1, TitleMenuOptions::LOAD_GAME));
	menu.setItem(new LineItem(S_QUIT_GAME, 2, TitleMenuOptions::QUIT_GAME));

	menu.setCurrentItem(0);
	menu.setWrapAround(false);
	menu.setFocus(true);
	menu.post(true);
}