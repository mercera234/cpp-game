#pragma once
#include <string>
#include "TUI.h"

struct Thing
{
	chtype symbol;
	std::string name;
	std::string description = "";
	int y, x; //location in map
};