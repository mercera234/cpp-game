#include <iostream>
#include "actor_helper.h"

void setBoundedStat(BoundInt& stat, int min, int max, int value)
{
	//min and max are 0 by default, so it is safer to set the max first before setting the min for most stats (if not all)
	stat.setMax(max);
	stat.setMin(min);
	stat.setCurr(value);
}

Actor* buildActor(ActorType type)
{
	Actor* actor = new Actor();

	//set thing properties
	actor->x = 30;
	actor->y = 5;
	actor->symbol = '?';
	actor->name = "Unnamed";

	//set actor properties
	actor->prevX = -1;
	actor->prevY = -1;

	actor->type = type;

	setBoundedStat(actor->money, 0, 9999999, 0);
	setBoundedStat(actor->stats.level, 1, 99, 1);
	setBoundedStat(actor->stats.exp, 0, 9999999, 0);
	setBoundedStat(actor->stats.hp, 0, 25, 25);
	setBoundedStat(actor->stats.mp, 0, 10, 0);
	setBoundedStat(actor->stats.strength, 0, 255, 0);
	setBoundedStat(actor->stats.defense, 0, 255, 0);
	setBoundedStat(actor->stats.intelligence, 0, 255, 0);
	setBoundedStat(actor->stats.will, 0, 255, 0);
	setBoundedStat(actor->stats.agility, 0, 255, 0);

	//not all stats are set yet!

	
	return actor;
}


Actor* loadActor(const std::string& filename, ActorType type)
{
	Actor* actor = new Actor();

	std::ifstream is;
	is.open("data\\" + filename, std::ios::binary);

	actor->load(is);
	actor->type = type;

	is.close();

	return actor;
}


void alterStatValue(BoundInt& stat, int amount)
{
	int startValue = stat.getCurr();
	stat.setCurr(startValue += amount);
}

bool isAlive(Actor& actor)
{
	return actor.stats.hp.getCurr() > 0;
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
