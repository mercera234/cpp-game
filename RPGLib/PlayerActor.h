#pragma once
#include "Player.h"
#include "Actor.h"

class PlayerActor : public Player
{
private:
	Actor* actor;
public:
	PlayerActor();
	void setActor(Actor* actorIn); 
	Actor* getActor() { return actor; }
	virtual bool canPlay();
};