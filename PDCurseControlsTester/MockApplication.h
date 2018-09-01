#pragma once
#include "Controllable.h"

class MockApplication
{
private:
public:
	int x = -1;
	MockApplication();
	~MockApplication();

	void operation();
	int operation2(int xIn);

	int mockCallback(Controllable* c, int input);
	int mockCallBack2(Controllable* c, int input);
	int setXinMockControl(Controllable* c, int input);
};

