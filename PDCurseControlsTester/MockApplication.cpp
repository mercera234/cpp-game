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

void MockApplication::mockCallback()
{
	cm->getFocusedControl()->setPosition(cm->getInput(), 0);
}


void MockApplication::mockCallBack2()
{
	
}

void MockApplication::setXinMockControl()
{
	MockControl* mc = (MockControl*)cm->getFocusedControl();
	mc->setX(cm->getInput());
	
}

void MockApplication::setXinMockControl2()
{
	MockControl* mc = (MockControl*)childCM->getFocusedControl();
	mc->setX(childCM->getInput());
}

void MockApplication::transferControl()
{
	ControlManager* manager = (ControlManager*)cm->getFocusedControl();
	processInput(*manager, cm->getInput());
	cm->setExitCode(manager->getExitCode());
}