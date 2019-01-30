#include "Actor.h"
#include <assert.h>
#include <algorithm>

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

bool Actor::alterStat(StatType statType, int amount)
{
	BoundInt& stat = getStat(statType);

	const int startValue = stat.getCurr();
	stat.setCurr(startValue + amount);

	return stat.getCurr() != startValue; //true if current value changed
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


bool Actor::ingestConsumable(Possession& posn)
{
	if (posn.item == nullptr)
		return false;

	GameItem* item = posn.item;
	if (item->type != GameItemType::USABLE)
	{
		return false;
	}
	/*if ((item = dynamic_cast<UsableItem*>(posn.item)) == nullptr)
		return false;*/

	TargetEffects& t = item->effects;
	bool anyChange = false;

	std::for_each(t.statValues.begin(), t.statValues.end(),
		[this, &anyChange](std::pair<StatType, int> p) {
		bool altered = alterStat(p.first, p.second);

		if (anyChange == false && altered)
			anyChange = true;
	});

	if (anyChange) //item was consumed
	{
		posn.quantity.alterCurr(-1);
	}
	
	return true;
}


bool Actor::equip(Possession& posn)
{
	if (posn.item == nullptr)
		return false;

	GameItem* item = posn.item;
	if (item->type != GameItemType::EQUIPPABLE)
		return false;

	/*if ((item = dynamic_cast<EquippableItem*>(posn.item)) == nullptr)
		return false;*/

	bool anyChange = true;
	switch (item->part)
	{
	case EquipPart::WEAPON: weapon = item; break;
	case EquipPart::HEAD: helmet = item; break;
		//TODO more to do here
	default:
		anyChange = false; //item not equipped
		break;
	}

	if (anyChange) //item should be removed from inventory since it is with actor
	{
		TargetEffects& t = item->effects;
		std::for_each(t.statValues.begin(), t.statValues.end(),
			[this, &anyChange](std::pair<StatType, int> p) {
			alterStat(p.first, p.second);
		});

		posn.quantity.alterCurr(-1);
	}

	return true;
}

GameItem* Actor::unEquip(EquipPart part)
{
	GameItem* removedItem = nullptr;
	switch (part)
	{
	case EquipPart::WEAPON: removedItem = weapon; weapon = nullptr; break;
	case EquipPart::HEAD: removedItem = helmet; helmet = nullptr; break;
	}

	if (removedItem == nullptr) //nothing was unequipped
		return nullptr;

	TargetEffects& t = removedItem->effects;
	std::for_each(t.statValues.begin(), t.statValues.end(),
		[this](std::pair<StatType, int> p) {
		alterStat(p.first, -p.second); //negate the amount since it is being removed
	});

	return removedItem; //once removed it can be returned to inventory or sold, etc...
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
