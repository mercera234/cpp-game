#pragma once
#include "DataPkg.h"
#include "Actor.h"

class DataPkgFactory
{
private:
public:
	DataPkg* createActorDefPackage(ActorDef* actor);
	ActorDef* getActorDefFromPkg(DataPkg* pkg);
};