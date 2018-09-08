#pragma once
#include "FilterPattern.h"
#include "MapRoom.h"


class MapEffectFilterPattern : public FilterPattern
{
private:
	void applyFilterPattern(); //override
	short getColor(EffectType effect);
public:
	MapEffectFilterPattern() {};
	MapEffectFilterPattern(MapRoom* m) : FilterPattern(m) {};
	void setMap(MapRoom* m);
};
