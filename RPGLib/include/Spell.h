#pragma once

#include <iostream>
using namespace std;


//#define SetBit(v,c) (v |= (1 << c))
//#define ClearBit(v,c) (v &= (~(1 << c)))
//#define CheckBit(v,c) (v & (1 << c))
//

//

//
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


//targetSet types
#define	TGT_SINGLE 0x1 // = 0x1, usable on any single target
#define TGT_SIDE 0x2 //, // = 0x2 usable on either side
#define TGT_ALL	0x4 //usable on every combatant

//targetSide defines
#define ANY_SIDE 0
#define SELF_SIDE 1 //self and opp sides intentionally made opposite of each other
#define OPP_SIDE -1

struct TargetScheme
{
	unsigned char set;
	char side;
};

class Actor;
struct Spell
{
	string name;
	string desc; //short description of spell
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

