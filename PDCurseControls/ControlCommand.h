#pragma once
#include "Controllable.h"

class ControlCommand
{
private:
public:
	virtual int execute(Controllable*, int) = 0;
};
