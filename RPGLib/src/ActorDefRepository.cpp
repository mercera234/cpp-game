#include "ActorDefRepository.h"
#include "KeyComparators.h"

ActorDefRepository::ActorDefRepository()
	: defs(stringCompare)
{
}

void ActorDefRepository::add(ActorDef& obj)
{
	std::pair<std::string, ActorDef*> newObj(obj.name, &obj);
	defs.insert(newObj);
}

ActorDef* ActorDefRepository::find(std::string& name)
{
	std::map<std::string, ActorDef*>::iterator it;
	it = defs.find(name);

	return it == defs.end() ? NULL : &(*it->second);
}