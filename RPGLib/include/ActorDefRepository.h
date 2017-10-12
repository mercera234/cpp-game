#pragma once
#include <map>
#include <functional>
#include "Repository.h"
#include "Actor.h"

class ActorDefRepository : public Repository<ActorDef>
{
private:
	std::map<std::string, ActorDef*, std::function<bool(std::string, std::string)>> defs;
public:
	ActorDefRepository();
	void add(ActorDef& obj);
	ActorDef* find(std::string& name);

	std::map<std::string, ActorDef*, std::function<bool(std::string, std::string)>>* getActorDefs() { return &defs; }
};