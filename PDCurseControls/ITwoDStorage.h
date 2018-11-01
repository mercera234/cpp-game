#pragma once
#include "Storable.h"

template <class T>
struct ITwoDStorage : public Storable
{
	virtual int getSize() = 0;
	virtual int getRows() = 0;
	virtual int getCols() = 0;
	
	/*Set the 2d dimensions of storage. If resizing from a previous size, retains the layout of the individual data elements.*/
	virtual void setDimensions(int rows, int cols) = 0;

	/*same as setDimensions but uses a fillValue to initialize all new elements.
	If being used for the first time, then the data will be filled with fillValue.
	If being resized from earlier, then the data outside the perimeter of the original size with get the fillvalue.*/
	virtual void setDimensions(int rows, int cols, const T fillValue) = 0;
	virtual void fill(const T datum) = 0; //fill all elements in storage with T
	
	//getters/setters
	virtual T getDatum(int element) = 0;
	virtual T getDatum(int y, int x) = 0;

	virtual bool setDatum(int element, const T &datum) = 0;
	virtual bool setDatum(int y, int x, const T &datum) = 0;	
};

