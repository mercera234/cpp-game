#pragma once
#include "Controllable.h"

class ControlManageApp
{
public:
	ControlManager* cm;

	ControlManageApp();
	~ControlManageApp();

	void modalCallback();
	void newCallback();
	void quitCallback();
	void callBackTest();
	void callBackTest2();
	void textCallback();
};

