#pragma once
#include "Controllable.h"

class ControlDecorator : public Controllable
{
private:
protected:
	Controllable* c;
public:
	Controllable* getControl() { return c; }
	void setControl(Controllable* cIn) { c = cIn; }
};