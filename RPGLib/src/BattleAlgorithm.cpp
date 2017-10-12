#include "BattleAlgorithm.h"
#include <cmath>
/*
Calculate damage incurred based on a physical attack
strength - 
defense -

percentAbsorbed = defense / 256.0
percentDamage = 1 - percentAbsorbed
actualDamage = floor(strength * percentDamage)
*/
int BattleAlgorithm::calcAttackDamage(int strength, unsigned char defense)
{
	double percentAbsorbed = defense / 256.0;
	double percentDamage = 1 - percentAbsorbed;
	double actualDamage = strength * percentDamage;
	return (int)(floor(actualDamage)); //return as an integer rounded down
}

//int calcMagicDamage(int value, int intelligence, unsigned char will)
//{
//	double percentAbsorbed = will / 256.0;
//	double percentDamage = 1 - percentAbsorbed;
//	double actualDamage = intelligence * percentDamage;
//	return (int)(floor(actualDamage)); //return as an integer rounded down
//}