#pragma once
#include "MenuItem.h"

class MockMenuItem : public MenuItem
{
private:
public:
	MockMenuItem() : MenuItem() {};
	MockMenuItem(unsigned short y, unsigned short x) : MenuItem(y, x) {};
	void draw() {}
};