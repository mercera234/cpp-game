#pragma once

#include <iostream>

//enum //ability effects(type for Effect)
//{
//	ALTER_HP = 0,
//	ALTER_FIGHT,
//	ALTER_STAT
//};
//
//enum //basis for effects
//{
//	//0 would be no basis
//	PHYSICAL = 1
//};

//typedef struct Effect
//{
//	int type;
//	
//	//target variables define potential targets for an ability
//	int targetSet;//a bit mask
//	int targetSide;
//	
//	int basis;//the attribute that the effect originates from 
//	float multiplier; //what is used to influence the degree of the effect
//	int status; //status effect bitmask
//}Effect;


//#define FIRE 1
//#define ICE 2
//#define WATER 3
//#define EARTH 4
//#define LIGHTNING 5

#include "Target.h"


struct Actor;
struct Spell
{
	//string name;
	//string desc; //short description of spell
	unsigned short cost; //mp cost of spell
	int value;
	int elemental; //identifies the affinity of the spell (fire, ice, water, earth, lightning)
	TargetScheme targetScheme;

	Actor* caster;
	//what happens before ability is executed
//	string pretext;//usually a pronoun verb format
	
	//what happens after ability is executed often a verb pronoun
	//string posttext;//i may not need this if i have damage amts show up to the right of the names
	
//	Effect effect;

};

