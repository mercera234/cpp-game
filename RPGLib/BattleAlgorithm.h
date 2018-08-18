#pragma once

const int MAX_STAT_VALUE = 255;
const double MAX_PERCENT = 256.0;

/*Calculate damage incurred based on a physical attack */
int calcAttackDamage(int attackerStrength, int receiverDefense);
int calcAgilityAdvantage(int attackerAgility, int attackerStrength, int receiverAgility);
int calcMagicDamage(double spellValue, int attackerIntelligence, unsigned char receiverWill);
int calcMagicRestore(double spellValue, int attackerIntelligence, unsigned char receiverWill);
int calcVariance(int value, unsigned int percentVariance);
