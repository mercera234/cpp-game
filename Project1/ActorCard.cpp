#include "ActorCard.h"
#include "AbstractMenu.h"

ActorCard::ActorCard(Actor* actor, int element, int crossRefNdx)
{
	this->actor = actor;
	index = element;
	this->crossRef = crossRefNdx;
}

//void ActorCard::draw(WINDOW* win, int offY, int offX)
//{
//	int topRow = posY - offY;
//	int leftEdge = posX - offX;
//
//	mvwaddstr(win, topRow, leftEdge, actor->def->name.c_str());
//	mvwprintw(win, topRow + 1, leftEdge, "HP %+4u/%-4u", actor->currHp, actor->def->maxHp);
//	mvwprintw(win, topRow + 2, leftEdge, "MP %+3u/%-3u", actor->currMp, actor->def->maxMp);
//}

void ActorCard::draw(WINDOW* win)
{
	int offY = menu == NULL ? 0 : menu->getUlY();
	int offX = menu == NULL ? 0 : menu->getUlX();

	int topRow = posY - offY;
	int leftEdge = posX - offX;

	mvwaddstr(win, topRow, leftEdge, actor->def->name.c_str());
	mvwprintw(win, topRow + 1, leftEdge, "HP %+4u/%-4u", actor->currHp, actor->def->maxHp);
	mvwprintw(win, topRow + 2, leftEdge, "MP %+3u/%-3u", actor->currMp, actor->def->maxMp);

	//mark side not implemented yet!

	//switch (menu->getMarkSide())
	//{
	//case LEFT_MARK:
	//	markPosX = posX - offX;
	//	itemPosX = markPosX + 2;
	//	break;
	//case RIGHT_MARK:
	//	itemPosX = posX - offX;
	//	markPosX = itemPosX + name.length(); //should maybe use width of the item, but this is not an abstract property currently
	//	break;
	//}


	if (menu != NULL && 
		(this == menu->getCurrentItem() || selected)) //draw mark
	{
		mvwaddstr(win, topRow, leftEdge - 2, menu->getMark().c_str());
	}
}