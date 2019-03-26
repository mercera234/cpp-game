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
	Actor* actor = nullptr;
	bool displayStatChanges;
	int statChange;

	void init(Actor* actor, int element);

public:
	ActorCard() {}
	ActorCard(Actor* actor, int element);
	ActorCard(Actor* actor, int element, int crossRefNdx);
	
	void applyDamage(int amount);
	
	void draw(); //override

	//setters/getters
	Actor* getActor() { return actor; }
	void setStatChange(int amount) { statChange = amount; displayStatChanges = true; }
	void setDisplayStatChanges(bool on) { displayStatChanges = on; }
};