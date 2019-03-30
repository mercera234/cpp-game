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

		//long newHp = getLevelData(player->def.level, "HP");
		//long hpRaise = newHp - player->def.maxHp;

		//player->def.maxHp = (int)newHp;//update max hp
		//player->currHp += (int)hpRaise;//give curr hp bonus
		//if (player->currHp > player->def.maxHp)//current hp should never surpass max hp
		//	player->currHp = player->def.maxHp;

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
		case StatType::HP: break;
			
		}

		return std::round(result);
	}
}