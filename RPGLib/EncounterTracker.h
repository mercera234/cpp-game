#pragma once
#include "Direction.h"

class EncounterTracker
{
private:
	/*total steps taken without an encounter
	vertical steps count for 2 steps versus horizontal steps since they are longer in a command prompt window*/
	int peaceSteps = 0; 
	int minSteps = 0; //an encounter can occur once this threshold has been hit
	int maxSteps = 0; //an encounter must occur once this threshold has been hit

	int chance = 0; //a percentage out of one hundred
	bool occurred = false; //true if encounter has occurred
	bool tracking = true; //true if encounters are possible and must be tracked
public:
	EncounterTracker();
	~EncounterTracker();

	void setMinSteps(int value);
	void setMaxSteps(int value);

	void takeStep(Axis axis);
	void resetSteps();

	//setters/getters
	int getMinSteps() { return minSteps; }
	int getMaxSteps() { return maxSteps; }
	int getPeaceSteps() { return peaceSteps; }
	void setEncounterTracking(bool track) { tracking = track; }
	void setEncounterChance(int chance) { this->chance = chance; }
	bool didEncounterOccur() { return occurred; }
	
};

