#pragma once
#include "Controllable.h"

class ControlDecorator : public Controllable
{
private:
protected:
	Controllable* c;
public:
	Controllable* getControl() { return c; }
	virtual void draw() = 0;
};