#pragma once
#include <bitset>

enum TargetSet //a combination of options may be chosen
{
	SINGLE = 0x1 //usable on any single target
	,SIDE = 0x2 //usable on either side
	,ALL = 0x4 //usable on every combatant at one time
};

enum class TargetSide //only one option can be chosen
{
	OPPOSITE = -1
	,ANY_SIDE = 0 //self and opp sides intentionally made opposite of each other
	,SELF = 1
};

struct TargetScheme
{
	std::bitset<3> set = { 0 };
	TargetSide side;
};