#pragma once
#include "MenuItem.h"
#include "Actor.h"
#include "curses.h"

/*
A class to represent an actor with specific data on screen.
Good for bestiaries, battle scenarios, or anywhere else that a character's stats can be viewed.
*/
class ActorCard : public MenuItem
{
private:
	Actor* actor;
	bool displayDamage;
	int damageTaken;

public:
	ActorCard(Actor* actor, int element, int crossRefNdx);
	Actor* getActor() { return actor; }
	void setDamage(int amount) { damageTaken = amount; displayDamage = true; }
	void draw();
};