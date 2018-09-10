#pragma once

enum class EffectType
{
	UNKNOWN = -1,
	NONE = 0, //functionally equivalent to using a nullptr for an EffectType
	OBSTR = 1,
	JUMPABLE = 2,
	HP_DECREASE = 3,
	HP_INCREASE = 4,
	UP_EXIT = 5,
	DOWN_EXIT = 6,
	SAVE = 7
};

const int effectTypeColors[8] = { 
	COLOR_BLACK, 
	COLOR_RED_BOLD,
	COLOR_BLUE,
	COLOR_RED,
	COLOR_MAGENTA_BOLD,
	COLOR_GREEN_BOLD,
	COLOR_GREEN,
	COLOR_YELLOW_BOLD
};


//We're not even using this right now
class TileEffect
{
private:
	EffectType type;
public:
	TileEffect() { type = EffectType::NONE; }
	TileEffect(EffectType typeIn) { type = typeIn; } //constructor
	EffectType getType() { return type; }
	//no setType since we don't want to change effects at runtime
};

class ComplexTileEffect : public TileEffect
{
private:
public:
	virtual void process() = 0;
	//no setType since we don't want to change effects at runtime
};

class HPAlterTileEffect : public ComplexTileEffect
{
private:
	unsigned short amount;
	unsigned short growthRateMultiplier; //1 means no growth
	unsigned short maxAmount; //0 means there is no max

public:
	void process() {};//just so it compiles
	//no setType since we don't want to change effects at runtime
};

//TODO consider flywheel design pattern?
//const TileEffect* const noEffect = new TileEffect(); //singleton for the no tile effect
//const std::vector<TileEffect> tileEffects = { noEffect };



