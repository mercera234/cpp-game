#pragma once
#include "Sprite.h"

const chtype defaultMoneySymbol = '$' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;

struct Money : public Thing
{
	int amount;
};