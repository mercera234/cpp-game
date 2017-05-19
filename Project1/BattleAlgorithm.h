#pragma once

class BattleAlgorithm
{
public:
	static int calcAttackDamage(int strength, unsigned char defense);
	static int calcMagicDamage(int value, int intelligence, unsigned char will);
};
