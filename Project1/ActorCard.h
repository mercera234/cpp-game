#pragma once
#include "Actor.h"
#include "curses.h"

class ActorCard
{
private:
	Actor* actor;

public:
	ActorCard(Actor* actor);
	void draw(WINDOW* win, int y, int x);
};