#pragma once
#include <map>
#include <functional>
#include "BoundInt.h"


class GameData
{
private:
//	std::map<std::string, int> integerData;
	std::map<std::string, std::string, std::function<bool(std::string, std::string)> > stringData;
	
public:
	GameData();
	~GameData();

	//Contains all global BoundInts in the game. For some reason this doesn't provide useful data in debugger such as size() ... etc.
	std::map<std::string, BoundInt, std::function<bool(std::string, std::string)> > bIntData;

	void clearData();
	bool storeIntData(const std::string& key, BoundInt value);
	BoundInt& retrieveIntData(const std::string& key);
	bool updateIntData(const std::string& key, int value);
	bool alterIntData(const std::string& key, int amount);
};

