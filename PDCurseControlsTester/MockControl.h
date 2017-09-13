#pragma once
#include "Controllable.h"

class MockControl : public Controllable
{
private:
	int x;
public:
	void setX(int value) { x = value; }
	int getX() { return x; }
	void draw() {}
};
