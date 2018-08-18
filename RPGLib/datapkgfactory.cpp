#include "DataPkgFactory.h"

//DataPkg* DataPkgFactory::createActorDefPackage(ActorDef* def)
//{
//	DataPkg* pkg = new DataPkg();
//
//	pkg->appendData(def->name.c_str(), MAX_NAME_LENGTH);
//	pkg->appendData(&def->symbol, sizeof(char));
//	pkg->appendData(&def->level, sizeof(short));
//	pkg->appendData(&def->exp, sizeof(int));
//	pkg->appendData(&def->money, sizeof(int));
//	pkg->appendData(&def->maxHp, sizeof(int));
//	pkg->appendData(&def->maxMp, sizeof(int));
//	pkg->appendData(&def->strength, sizeof(short));
//	pkg->appendData(&def->defense, sizeof(short));
//	pkg->appendData(&def->intelligence, sizeof(short));
//	pkg->appendData(&def->will, sizeof(short));
//	pkg->appendData(&def->agility, sizeof(short));
//	pkg->appendData(&def->accuracy, sizeof(float));
//	pkg->appendData(&def->luck, sizeof(float));
//
//	return pkg;
//}
//
//ActorDef* DataPkgFactory::getActorDefFromPkg(DataPkg* pkg)
//{
//	ActorDef* def = new ActorDef();
//	
//	char buffer[MAX_NAME_LENGTH]; //this is necessary or else the strings are not initialized properly
//	pkg->readNextData(buffer, MAX_NAME_LENGTH);
//	def->name.assign(buffer);
//	
//	pkg->readNextData(&def->symbol, sizeof(char));
//	pkg->readNextData(&def->level, sizeof(short));
//	pkg->readNextData(&def->exp, sizeof(int));
//	pkg->readNextData(&def->money, sizeof(int));
//	pkg->readNextData(&def->maxHp, sizeof(int));
//	pkg->readNextData(&def->maxMp, sizeof(int));
//	pkg->readNextData(&def->strength, sizeof(short));
//	pkg->readNextData(&def->defense, sizeof(short));
//	pkg->readNextData(&def->intelligence, sizeof(short));
//	pkg->readNextData(&def->will, sizeof(short));
//	pkg->readNextData(&def->agility, sizeof(short));
//	pkg->readNextData(&def->accuracy, sizeof(float));
//	pkg->readNextData(&def->luck, sizeof(float));
//
//	return def;
//}