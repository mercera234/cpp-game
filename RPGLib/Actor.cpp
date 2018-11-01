#include "Actor.h"

Actor::Actor()
{
	money.setCurrMax(9999999);

	////set default stat limits
	stats.hp.setMax(MAX_ENEMY_HP);
	stats.hp.setCurrMax(MAX_PLAYER_HP);

	stats.mp.setMax(999);
	stats.mp.setCurrMax(999);

	stats.level.setMax(MAX_LEVELS);
	stats.exp.setMax(MAX_EXP);
	stats.strength.setMax(255);
	stats.defense.setMax(255);
	stats.intelligence.setMax(255);
	stats.will.setMax(255);
	stats.agility.setMax(255);

}

void Actor::alterStat(StatType statType, int amount)
{
	BoundInt& stat = getStat(statType);

	int startValue = stat.getCurr();
	stat.setCurr(startValue += amount);
}


BoundInt& Actor::getStat(StatType statType)
{
	BoundInt* stat = nullptr;
	switch (statType)
	{
	case StatType::HP: stat = &stats.hp; break;
	case StatType::MP: stat = &stats.mp; break;
	case StatType::EXP: stat = &stats.exp; break;
	case StatType::LEVEL: stat = &stats.level; break;
	//case StatType::MONEY: stat = &money; break;
	case StatType::STRENGTH: stat = &stats.strength; break;
	case StatType::DEFENSE: stat = &stats.defense; break;
	case StatType::INTELLIGENCE: stat = &stats.intelligence; break;
	case StatType::WILL: stat = &stats.will; break;
	case StatType::AGILITY: stat = &stats.agility; break;
			
	}

	return *stat;
}







//int Actor::save(std::ofstream& saveFile)
//{
//	if (saveFile.is_open() == false)
//		return false;
//
//	char buf[MAX_NAME_LENGTH];
//	memset(buf, 0, MAX_NAME_LENGTH);
//	memcpy_s(buf, MAX_NAME_LENGTH, name.data(), name.length());
//
//	std::streampos startPos = saveFile.tellp();
//
//	saveFile.write(buf, MAX_NAME_LENGTH);
////	saveFile.write((char*)&symbol, sizeof(char));
//	/*saveFile.write((char*)&level, sizeof(short));
//	saveFile.write((char*)&exp, sizeof(int));
//	saveFile.write((char*)&money, sizeof(int));
//	saveFile.write((char*)&maxHp, sizeof(int));
//	saveFile.write((char*)&maxMp, sizeof(int));
//	saveFile.write((char*)&strength, sizeof(short));
//	saveFile.write((char*)&defense, sizeof(short));
//	saveFile.write((char*)&intelligence, sizeof(short));
//	saveFile.write((char*)&will, sizeof(short));
//	saveFile.write((char*)&agility, sizeof(short));
//	saveFile.write((char*)&accuracy, sizeof(float));
//	saveFile.write((char*)&luck, sizeof(float));*/
//	
//	std::streampos endPos = saveFile.tellp();
//
//	return (int)(endPos - startPos);
//}
//
//
//int Actor::load(std::ifstream& loadFile)
//{
//	if (loadFile.is_open() == false)
//		return false;
//
//	std::streampos startPos = loadFile.tellg();
//
//	char buf[MAX_NAME_LENGTH + 1];
//	loadFile.read(buf, MAX_NAME_LENGTH);
//	name.assign(buf);
//
////	loadFile.read((char*)&symbol, sizeof(char));
//	/*loadFile.read((char*)&level, sizeof(short));
//	loadFile.read((char*)&exp, sizeof(int));
//	loadFile.read((char*)&money, sizeof(int));
//	loadFile.read((char*)&maxHp, sizeof(int));
//	loadFile.read((char*)&maxMp, sizeof(int));
//	loadFile.read((char*)&strength, sizeof(short));
//	loadFile.read((char*)&defense, sizeof(short));
//	loadFile.read((char*)&intelligence, sizeof(short));
//	loadFile.read((char*)&will, sizeof(short));
//	loadFile.read((char*)&agility, sizeof(short));
//	loadFile.read((char*)&accuracy, sizeof(float));
//	loadFile.read((char*)&luck, sizeof(float));*/
//
//	std::streampos endPos = loadFile.tellg();
//
//	return (int)(endPos - startPos);
//}
//
