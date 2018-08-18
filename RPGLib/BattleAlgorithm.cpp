#include "BattleAlgorithm.h"
#include <cmath>
#include <ctime>


int calcAttackDamage(int attackerStrength, int receiverDefense)
{
	double percentAbsorbed = receiverDefense / MAX_PERCENT;
	double percentDamage = 1 - percentAbsorbed;
	double finalDamage = attackerStrength * percentDamage;
	return (int)(floor(finalDamage)); //return as an integer rounded down
}

int calcAgilityAdvantage(int attackerAgility, int attackerStrength, int receiverAgility)
{
	double agilityAdvantagePercent = (attackerAgility - receiverAgility) / MAX_PERCENT; //if negative, then it is technically a disadvantage
	return (int)(floor(attackerStrength * agilityAdvantagePercent));
}

int calcMagicDamage(double spellValue, int attackerIntelligence, unsigned char receiverWill)
{
	double percentAbsorbed = receiverWill / MAX_PERCENT;
	double percentDamage = 1 - percentAbsorbed;
	double damage = attackerIntelligence * percentDamage;
	double finalDamage = spellValue * damage;
	return (int)(floor(finalDamage)); //return as an integer rounded down
}

int calcMagicRestore(double spellValue, int attackerIntelligence, unsigned char receiverWill)
{
	double percentAbsorbed = receiverWill / MAX_PERCENT;
	double damage = attackerIntelligence * percentAbsorbed;
	double finalDamage = spellValue * damage;
	return (int)(floor(finalDamage)); //return as an integer rounded down
}


int calcVariance(int value, unsigned int percentVariance)
{
	if (percentVariance > 100)
		percentVariance = 100;

	int varianceTotal = (int)((percentVariance / 100.0) * value);
	if (varianceTotal < 1)
		return 0; //no variance because value is too small

	//value -= varianceTotal / 2;
	srand((unsigned int)time(NULL));

	return rand() % varianceTotal + 1;
}