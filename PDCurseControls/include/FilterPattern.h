#pragma once
#include "ControlDecorator.h"

class FilterPattern : public ControlDecorator
{
protected:
	bool enabled;
	virtual void applyFilterPattern() = 0;
public:
	FilterPattern(Controllable* c);
	void draw();
	void setEnabled(bool enabled) { this->enabled = enabled; }
	bool isEnabled() { return enabled; }
};
