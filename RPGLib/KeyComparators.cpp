#pragma once
#include "KeyComparators.h"

//return true if first is before second alphabetically
bool stringCompare(const std::string& first, const std::string& second)
{
	return first.compare(second) < 0;
}