#pragma once
#include "FilterPattern.h"
#include "Map.h"

//define color/type pairs here

class MapEffectFilterPattern : public FilterPattern
{
private:
	unsigned short color;
	short effectType;
	void applyFilterPattern();
	short getColor(short effect);
public:
	MapEffectFilterPattern(Map* m) : FilterPattern(m) {};
	
	//void setEffectColorPair(unsigned short color, short effect);
	short getEffect() { return effectType; }
};
