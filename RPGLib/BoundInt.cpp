#include "BoundInt.h"
#include <assert.h>


BoundInt::BoundInt(int minIn, int maxIn, int currIn)
{
	assert(maxIn >= minIn);
	min = minIn;
	max = maxIn;
	setCurr(currIn);
}

void BoundInt::boundCurr()
{
	if (curr > max)
		curr = max;
	else if (curr < min)
		curr = min;
}

void BoundInt::setCurr(int currIn)
{
	curr = currIn;
	boundCurr();
}

void BoundInt::setMax(int maxIn)
{ 
	assert(maxIn >= min);
	max = maxIn; 
	boundCurr();
}

void BoundInt::setMin(int minIn)
{
	assert(max >= minIn);
	min = minIn;
	boundCurr();
}

void BoundInt::maxOut()
{
	curr = max;
}