#pragma once

#include <iostream>
#include "curses.h"
#include "Thing.h"
#include "BoundInt.h"
#include "Storable.h"
#include "GridMenu.h"
#include "Stats.h"
#include "GameItem.h"

//character types
enum class ActorType
{
	HUMAN,
	CPU
};

////maximums
const int MAX_LEVELS = 99;
const int MAX_PLAYER_HP = 9999;
const int MAX_ENEMY_HP = 999999;
const int MAX_EXP = 9999999;
//const int MAX_PLAYERS = 4;
//
const int MAX_NAME_LENGTH = 16; //max characters in string

//status ailments
enum class Ailment
{
	ALIVE,
	REVEALED
};



/*
An actor represents individual user controlled players, enemies, and NPCs
*/
struct Actor : public Thing//, public Storable
{
	ActorType type;
	
	BoundInt money;
	
	//int ailments; //not sure if this should be with Actor or ActorDef
	/*void save(std::ofstream& saveFile);
	void load(std::ifstream& loadFile);*/
	GameItem* helmet = nullptr;
	GameItem* armor = nullptr;
	GameItem* gloves = nullptr;
	GameItem* boots = nullptr;
	GameItem* weapon = nullptr;
	GameItem* accessory = nullptr;

	bool alterStat(StatType statType, int amount);
	BoundInt& getStat(StatType statType);

	bool ingestConsumable(Possession& posn);

	/*Equip a possession on the actor. When equipped, item is removed from inventory and pointer to item is maintained in Actor object.
	Once equipped, the actor stats are temporarily altered until item is unequipped or switched to a different item.
	For example a sword with +5 strength increases Actor strength by 5 until it is removed.*/
	bool equip(Possession& posn);
	GameItem* unEquip(EquipPart part);

	/*
	The numeric values of an Actor that represent internal non-tangible qualities that determine effectiveness in battle and longterm qualities.
	Numeric values to define an actor stats.
	*/
	struct BaseStats
	{
		BoundInt level; //likely bound is 1-99
		BoundInt exp; //likely bound is 0-9999999

		/*Hit points for actor. Bounds will be 0 to whatever is the max hp for the actor's level. 
		the highest it can ever grow will be 9999 for HUMANS and 999999 for CPU*/
		BoundInt hp; 

		/*Magic points for actor. Bounds will be 0 to whatever is the max mp for the actor's level.
		the highest it can ever grow will be 999 for HUMANS and 9999 for CPU*/
		BoundInt mp;

		/*
		applies to strength of physical attacks
		likely bound is 0-255
		*/
		BoundInt strength; 

		/*a percentage(based on 255) of how much physical damage is absorbed
		Example:
		0 take all damage
		127 take half damage
		255 take no damage
		*/
		BoundInt defense;

		/*
		applies to strength and effectiveness of magic spells on their recipients
		likely bound is 0-255
		*/
		BoundInt intelligence;

		/*a percentage(based on 255) of how much magic is absorbed/resisted
		Example:
		0 take all damage
		127 take half damage
		255 take no damage
		*/
		BoundInt will;

		/*a percentage of how well hits connect with their target
		Example:
		0 no ability to evade at all, take all damage
		127 can halfway evade hits, take half damage
		255 evade all hits entirely, take no damage
		Agility may not factor into all types of attacks
		*/
		BoundInt agility; 

		//not sure if we'll use these
		float accuracy; //this may be better applied to weapons instead of actors
		float luck;

	};
	BaseStats stats;

	Actor();
};




