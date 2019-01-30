#include <iostream>
#include "actor_helper.h"

void setBoundedStat(BoundInt& stat, int min, int currMax, int max, int value)
{
	//min and max are 0 by default, so it is safer to set the max first before setting the min for most stats (if not all)
	stat.setValues(min, max, value);
	stat.setCurrMax(currMax);
}


void initDefaultActor(Actor& player)
{
	//set thing properties
	player.symbol = '?';
	player.name = "Unnamed";

	//set actor properties
	player.setType(ActorType::HUMAN);

	setBoundedStat(player.getMoney(), 0, 9999999, 9999999, 0);

	
	setBoundedStat(player.getStat(StatType::LEVEL), 1, MAX_LEVELS, MAX_LEVELS, 1);
	setBoundedStat(player.getStat(StatType::EXP), 0, 9999999, 9999999, 0);
	setBoundedStat(player.getStat(StatType::HP), 0, 25, MAX_PLAYER_HP, 25);
	setBoundedStat(player.getStat(StatType::MP), 0, 10, 999, 0);
	setBoundedStat(player.getStat(StatType::STRENGTH), 0, 255, 255, 0);
	setBoundedStat(player.getStat(StatType::DEFENSE), 0, 255, 255, 0);
	setBoundedStat(player.getStat(StatType::INTELLIGENCE), 0, 255, 255, 0);
	setBoundedStat(player.getStat(StatType::WILL), 0, 255, 255, 0);
	setBoundedStat(player.getStat(StatType::AGILITY), 0, 255, 255, 0);
}



//
//Actor* loadActor(const std::string& filename, ActorType type)
//{
//	Actor* actor = new Actor();
//
//	std::ifstream is;
//	is.open("data\\" + filename, std::ios::binary);
//
//	actor->load(is);
//	actor->type = type;
//
//	is.close();
//
//	return actor;
//}


void alterStatValue(BoundInt& stat, int amount)
{
	int startValue = stat.getCurr();
	stat.setCurr(startValue += amount);
}

bool isAlive(Actor& actor)
{
	return actor.getStat(StatType::HP).getCurr() > 0;
}

//
//
//Actor* createNPCActor()
//{
//	ActorDef* def = new ActorDef();
//	def->name = "NPC1";
//	def->symbol = (chtype) '&' | 0x0e000000;
//	def->level = 5;
//	def->exp = 0;
//	def->money = 0;
//	def->maxHp = 1;
//	def->maxMp = 1;
//	def->strength = 1;
//	def->defense = 1;
//	def->agility = 1;
//	def->accuracy = .9f;
//	def->luck = .05f;
//
//	Actor* actor = new Actor();
//	actor->def = def;
//	actor->currHp = def->maxHp;
//	actor->currMp = def->maxMp;
//	actor->defIndex = 1;
//	actor->prevX = -1;
//	actor->prevY = -1;
//	actor->x = 40;
//	actor->y = 4;
//	actor->type = ActorType::CPU; //not sure if we'll use npc actors yet
//	return actor;
//}
