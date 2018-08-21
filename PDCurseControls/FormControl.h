#pragma once
#include "Controllable.h"

class FormControl : public Controllable
{
protected:
	bool editable;
public:
	void setEditable(bool editable);
	virtual void clear() = 0;
};