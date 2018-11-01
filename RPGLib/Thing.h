#pragma once
#include <string>
#include "TUI.h"

struct Thing
{
	/*The symbol representing this thing on a map.
	If background color is set, it will be disregarded.*/
	chtype symbol;

	int id = -1;
	std::string name;
	std::string description = "";
	
	virtual void dummy() {} //virtual method is needed to make this a polymorphic class
};

