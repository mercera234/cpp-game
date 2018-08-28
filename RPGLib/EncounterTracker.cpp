#include "EncounterTracker.h"
#include <cmath>
#include <time.h>


EncounterTracker::EncounterTracker()
{
	srand((unsigned int)time(NULL));
}


void EncounterTracker::setMinSteps(int value)
{
	minSteps = value;
	if (maxSteps < minSteps)
		maxSteps = minSteps;
}

void EncounterTracker::setMaxSteps(int value)
{
	maxSteps = value;
	if (minSteps > maxSteps)
		minSteps = maxSteps;
}

void EncounterTracker::takeStep(Axis axis)
{
	if (tracking)
	{
		peaceSteps = (axis == Axis::VERTICAL) ? peaceSteps + 2 : peaceSteps + 1;

		if (peaceSteps >= maxSteps)
			occurred = true;
		else if (peaceSteps >= minSteps)
		{
			int r = rand() % 100;
			if (r < chance)
				occurred = true;
		}
	}
}

void EncounterTracker::resetSteps()
{
	peaceSteps = 0;
	occurred = false;
}


EncounterTracker::~EncounterTracker()
{
}
