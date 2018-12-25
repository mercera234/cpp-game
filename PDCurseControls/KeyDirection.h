#pragma once
#include "Direction.h"
#include "Controllable.h"

Direction getDirectionFromKey(int key);
Axis getAxisFromKey(int key);
int getMagnitudeFromKey(int key, Controllable* c = nullptr);