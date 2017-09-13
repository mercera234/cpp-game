#include "MockControl.h"
#include "ControlManager.h"

void mockCallBack(void* caller, void* ptr, int input)
{

}

void mockCallBack2(void* caller, void* ptr, int input)
{

}

void setCyclicKeyInControlManager(void* caller, void* ptr, int input)
{
	ControlManager* cm = (ControlManager*)ptr;
	cm->setCycleKey(input);
}

void setXinMockControl(void* caller, void* ptr, int input)
{
	MockControl* mc = (MockControl*)caller;
	mc->setX(input);
}