#pragma once
#include "Controllable.h"

class FormControl : public Controllable
{
protected:
public:
	virtual void clear() = 0;
};