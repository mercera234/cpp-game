#pragma once
#include "Actor.h"

namespace alg {
	int toNext(Actor& player);
	int gainExp(Actor& player, int amount); //add exp to actor stat, return number of levels gained
	int getLevelData(int curLevel, StatType type);
}



