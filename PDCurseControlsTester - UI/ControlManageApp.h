#pragma once
#include "Controllable.h"

class ControlManageApp
{
public:
	ControlManageApp();
	~ControlManageApp();

	int modalCallback(Controllable* ptr, int input);
	int newCallback(Controllable* ptr, int input);
	int quitCallback(Controllable* ptr, int input);
	int callBackTest(Controllable* ptr, int input);
	int callBackTest2(Controllable* ptr, int input);
	int textCallback(Controllable* ptr, int input);
};

