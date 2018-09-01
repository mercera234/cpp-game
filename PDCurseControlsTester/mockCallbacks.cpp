#include "MockControl.h"
#include "ControlManager.h"

int mockCallBack(void* caller, void* ptr, int input)
{
	return HANDLED;
}

int mockCallBack2(void* caller, void* ptr, int input)
{
	return HANDLED;
}

int setCyclicKeyInControlManager(void* caller, void* ptr, int input)
{
	ControlManager* cm = (ControlManager*)ptr;
	cm->setCycleKey(input);
	return HANDLED;
}

int setXinMockControl(void* caller, void* ptr, int input)
{
	MockControl* mc = (MockControl*)caller;
	mc->setX(input);
	return HANDLED;
}

