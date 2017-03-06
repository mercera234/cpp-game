#pragma once
#include "curses.h"

struct TileEffect
{
	short type;
	short value;
	TileEffect(short type, short value) { this->type = type; this->value = value; }
};

class Tile
{
private:
	chtype display;
	TileEffect* effect;
public:
	Tile() { setDisplay(' '); effect = new TileEffect(0, 0); };
	Tile(chtype c) { setDisplay(c); effect = new TileEffect(0, 0); }
	chtype getDisplay() { return display; }
	void setDisplay(chtype c) { display = c; }
	
	TileEffect* getEffect() { return effect; }
	void setEffect(TileEffect* effect) { this->effect = effect; }
	void setEffectType(short type) { effect->type = type; }
	void setEffectValue(short value) { effect->value = value; }
};