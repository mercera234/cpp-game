#pragma once
#include "Controllable.h"

/*A class meant to act as a local or class member to wrap a volatile pointer object*/
class ControlHandle : public Controllable
{
public:
	Controllable* control = nullptr;

	ControlHandle() {}
	void draw();
};

