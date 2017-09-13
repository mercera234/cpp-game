#pragma once
#include "MenuItem.h"

struct LineItem : public MenuItem
{
	std::string name;
	LineItem(std::string name, int element, int crossRefNdx);
	void draw();
	void clear();
};