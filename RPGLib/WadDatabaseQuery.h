#pragma once
#include "WadDatabase.h"
//#include "Repository.h"
#include <map>
#include <functional>


template <typename Key, typename Val>
class WadDatabaseQuery
{
private:
public:
	void getRepo(WadDatabase& db, LumpType type, std::map<Key, Val>& repo);
};

template <typename Key, typename Val>
void WadDatabaseQuery::getRepo(WadDatabase& db, LumpType type, std::map<Key, Val, function<bool(Key, Key)>>& repo)
{
	
	switch (type)
	{
	case LumpType::ACTOR: 
		
		//db.getActorRepository(std::map<std::string, ActorDef*, std::function<bool(std::string, std::string)>>& mapRepo, std::ifstream& wadFile);
		
		break;
	//case LumpType::MAP: repo = MapRepository(3, 5); break;
	}
	
	return repo;
}