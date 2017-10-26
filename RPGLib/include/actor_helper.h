#pragma once
#include <string>
#include "Actor.h"

Actor* buildActorFromDef(ActorDef* def, int type);
Actor* createActor(const std::string& filename, int type);
Actor* createNPCActor();