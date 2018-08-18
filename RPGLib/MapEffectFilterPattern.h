#pragma once
#include "FilterPattern.h"
#include "Map.h"


class MapEffectFilterPattern : public FilterPattern
{
private:
	void applyFilterPattern(); //override
	short getColor(EffectType effect);
public:
	MapEffectFilterPattern() {};
	MapEffectFilterPattern(Map* m) : FilterPattern(m) {};
	void setMap(Map* m);
};
