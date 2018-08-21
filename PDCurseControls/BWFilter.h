#pragma once
#include "FilterPattern.h"

class BWFilter : public FilterPattern
{
private:
	void applyFilterPattern();
public:
	BWFilter(Controllable* c) : FilterPattern(c) {};
};