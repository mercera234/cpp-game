#include "GameData.h"
#include "KeyComparators.h"

GameData::GameData() : bIntData(stringCompare), stringData(stringCompare)
{
}


bool GameData::storeIntData(const std::string& key, BoundInt value)
{
	auto retPair = bIntData.insert(std::make_pair(key, value));
	return retPair.second;
}

BoundInt& GameData::retrieveIntData(const std::string& key)
{
	return bIntData.at(key);
}


bool GameData::updateIntData(const std::string& key, int value)
{
	bIntData.at(key).setCurr(value);
	return true;
}


bool GameData::alterIntData(const std::string& key, int amount)
{
	BoundInt& data = bIntData[key];
	data.alterCurr(amount);
	return true;
}

GameData::~GameData()
{
}
