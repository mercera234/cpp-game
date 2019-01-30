#pragma once
#include <limits>

/*
Encapsulates an integer value with a min and max value different than what is capable with a 4-byte integer.
min <= curr <= currMax <= tempMax <= max
*/
class BoundInt
{
private:
	int currMax = INT_MAX; //the current maximum, which can be changed
	int min = 0; //the minimum that curr can ever go
	int curr = 0; //the current value
	int tempMax = INT_MAX; //for temporary raising of the currMax
	int max = INT_MAX; //a max value that cannot change once set. The curr max and temp max cannot grow beyond this value
	
	void boundValue(int lowEnd, int& value, int highEnd);
public:
	BoundInt() {};
	BoundInt(int minIn, int maxIn, int currIn = 0);

	void setValues(int minIn, int maxIn, int currIn = 0);
	void maxOut();

	bool isMaxedOut() { return curr == currMax; }
	bool isDrained() { return curr == min; }

	//getters/setters
	
	//Update the min bound. Will throw an assertion error if greater than the currMax.
	void setMin(int minIn);

	//Update the current value. Will be bounded between the min and tempMax
	void setCurr(int currIn);

	/*Update the current value relative what it is currently with amount. 
	Amount is always added so use negative numbers for subtraction*/
	void alterCurr(int amount);

	//Update the currMax bound. Will throw an assertion error if lower than the min or greater than the max.
	void setCurrMax(int currMaxIn);

	//Update the tempmax bound.
	void setTempMax(int tempMaxIn);
	void resetTempMax() { tempMax = currMax; }

	//Set the max. Can be done once during life of BoundInt
	void setMax(int maxIn);

	int getMin() { return min; }
	int getCurr() { return curr; }
	int getCurrMax() { return currMax; }
	int getTempMax() { return tempMax; }
	int getMax() { return max; }

};

