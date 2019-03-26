#pragma once

#include "Thing.h"
#include "Stats.h"
#include "BoundInt.h"
#include "Target.h"

const chtype defaultItemSymbol = '*' | COLOR_MAGENTA_BOLD << TEXTCOLOR_OFFSET;

enum class GameItemType
{
	BLANK, //not an item, just a place holder
	USABLE,
	EQUIPPABLE,
	KEY,
	MONEY
};

const int NO_COST = -1; //for items that can't be bought or sold

struct TargetEffects
{
	/*Stat values
	Consumables: What stats are affected and by what amount
	Equippable: What stats are affected by wearer and amount
	*/
	std::map<StatType, int> statValues; //a collection of all stats affected and by what amount

	/*Target identifies:
	Consumables: Who can consume the item
	Weapon: Who is attacked by the item (one or many)
	Armor/Accessory: n/a
	*/
	TargetScheme target;

	//ailments 
};


enum class EquipPart
{
	BLANK, //not an item, just a place holder
	WEAPON,
	HEAD,
	BODY,
	HANDS,
	FEET,
	ACCESSORY
};


struct GameItem : public Thing
{
	GameItemType type;
	int cost;

	TargetEffects effects; //for usable and equippable types
	EquipPart part; //the body part that the item can be equipped on(for equippables only)
	//std::bitset<32> CharClass  class of character than can equip this 
	int lockId; //for keys only
};


/*The item and amount of it owned in an inventory. Max is 99 of any type.*/
struct Possession
{
	GameItem* item;
	BoundInt quantity;
};

