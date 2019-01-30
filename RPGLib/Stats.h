#pragma once
#include <map>

//modifiable stat types
enum class StatType
{
	LEVEL,
	EXP,
	HP,
	MP,
	STRENGTH,
	DEFENSE,
	INTELLIGENCE,
	WILL,
	AGILITY
};

/*A combination of what is manipulated and to what degree*/
struct StatManipulator
{
	StatType type;
	int value;
	//may need to include duration variable (temporary, permanent, until condition, etc...)
};


//a mechanism for containing a variety of stats and values
struct StatDistribution
{
	std::map<StatType, int> stats;

	/*StatDistribution()
	{
		stats[StatType::LEVEL] = 0;
		stats[StatType::EXP] = 0;
		stats[StatType::HP] = 0;
		stats[StatType::MP] = 0;
		stats[StatType::STRENGTH] = 0;
		stats[StatType::DEFENSE] = 0;
		stats[StatType::INTELLIGENCE] = 0;
		stats[StatType::WILL] = 0;
		stats[StatType::AGILITY] = 0;
	}*/
};