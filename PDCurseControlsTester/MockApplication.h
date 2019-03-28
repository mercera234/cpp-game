#pragma once
#include "Controllable.h"
#include "ControlManager.h"

class MockApplication
{
private:
public:
	ControlManager* cm;
	ControlManager* childCM; 
	int x = -1;
	MockApplication();
	~MockApplication();

	void operation();
	int operation2(int xIn);

	void mockCallback();
	void mockCallBack2();
	void setXinMockControl();
	void setXinMockControl2();
	void transferControl();
};

