#include "PlayerActor.h"
#include "actor_helper.h"

PlayerActor::PlayerActor()
{
	turnCount = 0;
}


bool PlayerActor::canPlay()
{
	return isAlive(*actor);
}

void PlayerActor::setActor(Actor* actorIn)
{
	actor = actorIn;
}