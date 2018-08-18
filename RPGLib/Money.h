#pragma once
#include "Pickup.h"

const chtype defaultMoneySymbol = '$' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;

struct Money : public Pickup
{
	unsigned int amount;
};