#include "MockApplication.h"
#include "ControlManager.h"
#include "MockControl.h"


MockApplication::MockApplication()
{
}


MockApplication::~MockApplication()
{
}


void MockApplication::operation()
{
	x = 234;
}

int MockApplication::operation2(int xIn)
{
	x = xIn;
	return x;
}

int MockApplication::mockCallback(Controllable* c, int input)
{
	c->setPosition(input, 0);
	return HANDLED;
}


int MockApplication::mockCallBack2(Controllable* c, int input)
{
	return HANDLED;
}

int MockApplication::setXinMockControl(Controllable* c, int input)
{
	MockControl* mc = (MockControl*)c;
	mc->setX(input);
	return HANDLED;
}