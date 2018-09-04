#include "ActorCard.h"
#include "AbstractMenu.h"
#include <sstream>
#include "TUI.h"
#include "actor_helper.h"

ActorCard::ActorCard()
{

}

ActorCard::ActorCard(Actor* actor, int element, int crossRefNdx)
{
	this->actor = actor;
	index = element;
//	this->crossRef = crossRefNdx;

	displayDamage = false; //only true when damage has been taken and must be displayed
	damageTaken = 0;
}

void ActorCard::draw()
{
	MenuItem::draw();

	//draw item
	std::string titleStr = isAlive(*actor)? actor->name : "*DEAD*" ;

	mvwaddstr(win, topRow, itemX, titleStr.c_str());

	BoundInt hpStat = actor->getStat(StatType::HP);
	BoundInt mpStat = actor->getStat(StatType::MP);

	mvwprintw(win, topRow + 1, itemX, "HP %+4u/%+4u", hpStat.getCurr(), hpStat.getCurrMax());
	mvwprintw(win, topRow + 2, itemX, "MP %+4u/%+4u", mpStat.getCurr(), mpStat.getCurrMax());

	//display damage taken if any
	if (displayDamage)
	{
		std::ostringstream oss;

		char sign = damageTaken > 0 ? '-' : '+';
		oss << sign << damageTaken;
		
		chtype dmgColor = COLOR_PAIR(COLOR_RED_BOLD);
		wattron(win, dmgColor);
		mvwaddstr(win, topRow, cursorX + 17, oss.str().c_str());
		wattroff(win, dmgColor);
	}

}

void ActorCard::applyDamage(int amount)
{
	displayDamage = true;
	damageTaken = amount;

	alterStatValue(actor->getStat(StatType::HP), -damageTaken);
}