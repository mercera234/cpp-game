#pragma once
#include "MenuItem.h"
#include "Actor.h"
#include "curses.h"

class ActorCard : public MenuItem
{
private:
	Actor* actor;

public:
	ActorCard(Actor* actor, int element, int crossRefNdx);
	Actor* getActor() { return actor; }
	//void draw(WINDOW* win, int offY, int offX);
	void draw(WINDOW* win);
};