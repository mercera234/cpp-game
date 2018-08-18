#include "GameItemMenuRecord.h"

void GameItemMenuRecord::draw()
{
	MenuItem::draw();

	//draw item details
	mvwaddstr(win, topRow, itemX, item->name.c_str());
}