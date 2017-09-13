#pragma once
#include "AbstractMenu.h"

class MockMenu : public AbstractMenu
{
private:
	void dirDriver(int input) {}
public:
	MockMenu();
	int driver(int input) { return -1; }
	void draw() {}
};