#pragma once
#include <algorithm>
#include <vector>

template <class T>
class TwoDStorage
{
private:
	unsigned int rows;
	unsigned int cols;
	std::vector<T> data;
public:
	TwoDStorage() {};
	TwoDStorage(unsigned int rows, unsigned int cols);

	void setDimensions(unsigned int rows, unsigned int cols);

	void fill(T datum); //fill all elements in storage with T
	void copyFrom(TwoDStorage<T>& fromStorage);

	//getters/setters
	T getDatum(unsigned int element);
	T getDatum(unsigned int y, unsigned int x);
	
	unsigned int getSize() { return data.size(); }
	unsigned int getRows() { return rows; }
	unsigned int getCols() { return cols; }

	bool setDatum(unsigned int element, const T &datum);
	bool setDatum(unsigned int y, unsigned int x, const T &datum);

	std::vector<T>& getData() { return data; }
};

template <class T>
TwoDStorage<T>::TwoDStorage(unsigned int rows, unsigned int cols)
{
	setDimensions(rows, cols);
}

template <class T>
void TwoDStorage<T>::setDimensions(unsigned int rows, unsigned int cols)
{
	this->rows = rows;
	this->cols = cols;

	int size = rows * cols;
	data.resize(size);
}

//TODO alter the next 2 methods to return T* so that nullptr can be returned instead of throwing an exception
template <class T>
T TwoDStorage<T>::getDatum(unsigned int y, unsigned int x)
{
	/*if (y >= rows || x >= cols)
		throw new exception;*/

	return getDatum(y * cols + x);
}

template <class T>
T TwoDStorage<T>::getDatum(unsigned int element)
{
	/*if (element >= size)
		throw new exception;*/

	return data[element];
}


template <class T>
bool TwoDStorage<T>::setDatum(unsigned int y, unsigned int x, const T &datum)
{
	if (y >= rows || x >= cols)
		return false;
	
	return setDatum(y * cols + x, datum);
}

template <class T>
bool TwoDStorage<T>::setDatum(unsigned int element, const T &datum)
{
	if (element >= data.capacity())
		return false;

	data[element] = datum;
	return true;
}

template <class T>
void TwoDStorage<T>::fill(T datum)
{
	std::fill(data.begin(), data.end(), datum);
}

/*
copyFrom is not the same as a copy constructor. It doesn't make an exact duplicate. 
It copies as much data from fromStorage as can fit in toStorage without exceeding toStorages dimensions
*/
template <class T>
void TwoDStorage<T>::copyFrom(TwoDStorage<T>& fromStorage)
{
	int copyRows = std::min(rows, fromStorage.getRows()); //copy only what will fit in the current data storage
	int copyCols = std::min(cols, fromStorage.getCols());

	for (int row = 0; row < copyRows; row++)
	{
		for (int col = 0; col < copyCols; col++)
		{
			setDatum(row, col, fromStorage.getDatum(row, col));
		}
	}
}


