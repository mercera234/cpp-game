#include "ActorCard.h"
#include "AbstractMenu.h"
#include <sstream>
#include "TUI.h"

ActorCard::ActorCard(Actor* actor, int element, int crossRefNdx)
{
	this->actor = actor;
	index = element;
	this->crossRef = crossRefNdx;

	displayDamage = false; //only true when damage has been taken and must be displayed
	damageTaken = 0;
}

void ActorCard::draw()
{
	int offY = menu == NULL ? 0 : menu->getUlY();
	int offX = menu == NULL ? 0 : menu->getUlX();

	int topRow = posY - offY;
	int leftEdge = posX - offX;

	std::string titleStr = IS_DEAD(actor) ? "*DEAD*" : actor->def->name;

	WINDOW* win = menu->getWindow();
	mvwaddstr(win, topRow, leftEdge, titleStr.c_str());
	mvwprintw(win, topRow + 1, leftEdge, "HP %+4u/%-4u", actor->currHp, actor->def->maxHp);
	mvwprintw(win, topRow + 2, leftEdge, "MP %+4u/%-4u", actor->currMp, actor->def->maxMp);

	if (menu != NULL && 
		(this == menu->getCurrentItem() || selected)) //draw mark
	{
		std::string mark;
		if (menu->isFocused()) 
		{
			mvwaddstr(win, topRow, leftEdge - 2, menu->getCursor().c_str());
		}
	}

	//display damage taken if any
	if (displayDamage)
	{
		displayDamage = false; //
		std::ostringstream oss;

		char sign = damageTaken > 0 ? '-' : '+';
		oss << sign << damageTaken;
		//string dmgStr = damageTaken;
		wattron(win, COLOR_PAIR(COLOR_RED_BOLD));
		mvwaddstr(win, topRow, leftEdge + 17, oss.str().c_str());
		wattroff(win, COLOR_PAIR(COLOR_RED_BOLD));
	}

}