#include "BoundInt.h"
#include <assert.h>


BoundInt::BoundInt(int minIn, int maxIn, int currIn)
{
	setValues(minIn, maxIn, currIn);
}

void BoundInt::setValues(int minIn, int maxIn, int currIn)
{
	assert(maxIn >= minIn);
	min = minIn;
	setMax(maxIn);
	setCurr(currIn);
}


void BoundInt::boundValue(int lowEnd, int& value, int highEnd)
{
	if (value > highEnd)
		value = highEnd;
	else if (value < lowEnd)
		value = lowEnd;
}

void BoundInt::setMin(int minIn)
{
	assert(currMax >= minIn);
	min = minIn;
	boundValue(min, curr, tempMax);
}

void BoundInt::setCurr(int currIn)
{
	curr = currIn;
	if (tempMax > currMax)
		setTempMax(curr);

	boundValue(min, curr, tempMax);
}

void BoundInt::alterCurr(int amount)
{
	setCurr(curr + amount);
}

void BoundInt::setCurrMax(int currMaxIn)
{ 
	assert(currMaxIn >= min && currMax <= max);
	currMax = tempMax = currMaxIn; 
	boundValue(min, curr, tempMax);
}

void BoundInt::setTempMax(int tempMaxIn)
{
	tempMax = tempMaxIn;
	boundValue(currMax, tempMax, max);
}

void BoundInt::setMax(int maxIn)
{
	assert(maxIn >= min);
	max = maxIn;
	if (currMax > max) currMax = max;
	resetTempMax();
}


void BoundInt::maxOut()
{
	curr = tempMax;
}