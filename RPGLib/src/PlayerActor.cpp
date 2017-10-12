#include "PlayerActor.h"

PlayerActor::PlayerActor()
{
	turnCount = 0;
}


bool PlayerActor::canPlay()
{
	return IS_ALIVE(actor);
}

void PlayerActor::setActor(Actor* actorIn)
{
	actor = actorIn;
}