#pragma once

class BoundInt
{
private:
	int max = 0; //these are different types, but this is ok unless we ever feel the need to go beyond 2.1 billion
	int min = 0;
	int curr = 0;
	//TODO consider changing max to currMax, adding tempMax and finalMax
	//currMax would be what the max is
	//tempMax would be for temporary raising of the currMax
	//finalMax is a max value that cannot change once set. The curr max and temp max cannot grow beyond this value
	void boundCurr();
public:
	BoundInt() {};
	BoundInt(int minIn, int maxIn, int currIn = 0);

	void maxOut();

	//getters/setters
	void setCurr(int currIn);

	//Update the max bound. Will throw an assertion error if lower than the min.
	void setMax(int maxIn);

	//Update the min bound. Will throw an assertion error if greater than the max.
	void setMin(int minIn);

	int getMax() { return max; }
	int getMin() { return min; }
	int getCurr() { return curr; }
};
