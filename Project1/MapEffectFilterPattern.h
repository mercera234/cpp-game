#pragma once
#include "FilterPattern.h"
#include "Map.h"

class MapEffectFilterPattern : public FilterPattern
{
private:
	unsigned short color;
	char effect;
	void applyFilterPattern();
public:
	MapEffectFilterPattern(Map* m) : FilterPattern(m) {};
	
	void setEffectColorPair(unsigned short color, short effect);
	short getEffect() { return effect; }
};
