#include "ActorCard.h"


ActorCard::ActorCard(Actor* actor)
{
	this->actor = actor;
}

void ActorCard::draw(WINDOW* win, int y, int x)
{
	int topRow = y;
	int leftEdge = x;

	mvwaddstr(win, y, x, actor->def->name.c_str());
	mvwprintw(win, y + 1, x, "HP %+4u/%-4u", actor->currHp, actor->def->maxHp);
	mvwprintw(win, y + 2, x, "MP %+3u/%-3u", actor->currMp, actor->def->maxMp);
}