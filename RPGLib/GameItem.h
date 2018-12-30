#pragma once

#include "Thing.h"

const chtype defaultItemSymbol = '*' | COLOR_MAGENTA_BOLD << TEXTCOLOR_OFFSET;

enum class GameItemType
{
	BLANK, //not an item, just a place holder
	CONSUMABLE,
	EQUIPPABLE,
	KEY,
	MONEY
};

struct GameItem : public Thing
{
	GameItemType type;
	int cost;
	int value; 
};

#include "BoundInt.h"

struct OwnedItem
{
	GameItem* item;
	int quantity;
};