#pragma once
#include <string>
#include "Actor.h"

void setBoundedStat(BoundInt& stat, int min, int currMax, int max, int value);
Actor* buildActor(ActorType type);
Actor* loadActor(const std::string& filename, ActorType type);
void alterStatValue(BoundInt& stat, int amount); //use negative amount for decreases
bool isAlive(Actor& actor);


void initTestActor(Actor& player);
//Actor* buildActor(int type);

//Actor* createNPCActor();