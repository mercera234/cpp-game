#pragma once

#include "Pickup.h"

const chtype defaultItemSymbol = '*' | COLOR_MAGENTA_BOLD << TEXTCOLOR_OFFSET;

enum class GameItemType
{
	BLANK, //not an item, just a place holder
	CONSUMABLE,
	WEAPON,
	ARMOR,
	ACCESSORY,
	KEY,
	READABLE
};

struct GameItem : public Pickup
{
	GameItemType type;
	unsigned int cost;
};