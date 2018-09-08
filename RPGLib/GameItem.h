#pragma once

#include "Thing.h"

const chtype defaultItemSymbol = '*' | COLOR_MAGENTA_BOLD << TEXTCOLOR_OFFSET;

enum class GameItemType
{
	BLANK, //not an item, just a place holder
	CONSUMABLE,
	WEAPON,
	ARMOR,
	ACCESSORY,
	KEY,
	READABLE,
	MONEY
};

struct GameItem : public Thing
{
	GameItemType type;
	int cost;
};