#include <cmath>
#include <assert.h>
#include "GameAlgorithm.h"

namespace alg {

	int toNext(Actor& player)
	{
		int level = player.getStat(StatType::LEVEL).getCurr();
		assert(level > 0 && level <= MAX_LEVELS);
		if (level == MAX_LEVELS)
			return 0;

		int expNeeded = (int)getLevelData(level + 1, StatType::EXP);
		return expNeeded - player.getStat(StatType::EXP).getCurr();
	}

	void awardLevelBonuses(Actor& player)
	{
		player.alterStat(StatType::LEVEL, 1);

		BoundInt& level = player.getStat(StatType::LEVEL);

		//hp
		int newHp = getLevelData(level.getCurr(), StatType::HP);
		BoundInt& hpStat = player.getStat(StatType::HP);
		int diffHp = newHp - hpStat.getCurrMax();
		hpStat.setCurrMax(newHp);
		hpStat.alterCurr(diffHp); //give curr hp bonus
	
		//str
		player.alterStat(StatType::STRENGTH, getLevelData(level.getCurr(), StatType::STRENGTH));
	

		//player->def.strength = (int)getLevelData(player->def.level, "STRENGTH");
		//update other stats as well here
	}

	int gainExp(Actor& player, int amount)
	{
		BoundInt& exp = player.getStat(StatType::EXP);
		exp.alterCurr(amount);

		int startLevel = player.getStat(StatType::LEVEL).getCurr();
		bool checkingLevels = true;
		while (checkingLevels)
		{
			int expNeeded = getLevelData(player.getStat(StatType::LEVEL).getCurr() + 1, StatType::EXP);
			
			if (exp.getCurr() >= expNeeded)
			{
				//a level was gained
				awardLevelBonuses(player);
				
			}
			else
				checkingLevels = false;
		}

		return player.getStat(StatType::LEVEL).getCurr() - startLevel;
	}

	int getLevelData(int curLevel, StatType type)
	{
		assert(curLevel > 1 && curLevel <= 99);
		double result;
		switch (type)
		{
		case StatType::EXP: 
			result =  
			0.0625 * std::pow(curLevel, 4) + 
			0.6329 * std::pow(curLevel, 3) +
			1.8787 * std::pow(curLevel, 2) +
			9.717 * curLevel + 
			-21.066
			; break;
		case StatType::HP: 
			result = 1.01 * std::pow(curLevel, 2) + 24; //not the best formula for this just yet, but it'll do short term			
			break;
		case StatType::STRENGTH:
			result = 1 + curLevel * 2; //not the best formula for this just yet, but it'll do short term			
			break;
		}

		return std::round(result);
	}
}