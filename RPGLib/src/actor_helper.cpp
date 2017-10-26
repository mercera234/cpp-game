#include <iostream>
#include "actor_helper.h"

Actor* buildActorFromDef(ActorDef* def, int type)
{
	Actor* actor = new Actor();
	actor->def = def;
	actor->currHp = def->maxHp;
	actor->currMp = def->maxMp;
	actor->defIndex = 1;
	actor->prevX = -1;
	actor->prevY = -1;
	actor->x = 30;
	actor->y = 5;
	actor->type = type;
	return actor;
}


Actor* createActor(const std::string& filename, int type)
{
	ActorDef* def = new ActorDef();

	std::ifstream is;
	is.open("data\\" + filename, std::ios::binary);

	def->load(is);

	is.close();

	return buildActorFromDef(def, type);
}


Actor* createNPCActor()
{
	ActorDef* def = new ActorDef();
	def->name = "NPC1";
	def->symbol = (chtype) '&' | 0x0e000000;
	def->level = 5;
	def->exp = 0;
	def->money = 0;
	def->maxHp = 1;
	def->maxMp = 1;
	def->strength = 1;
	def->defense = 1;
	def->agility = 1;
	def->accuracy = .9f;
	def->luck = .05f;

	Actor* actor = new Actor();
	actor->def = def;
	actor->currHp = def->maxHp;
	actor->currMp = def->maxMp;
	actor->defIndex = 1;
	actor->prevX = -1;
	actor->prevY = -1;
	actor->x = 40;
	actor->y = 4;
	actor->type = AT_CPU; //not sure if we'll use npc actors yet
	return actor;
}
