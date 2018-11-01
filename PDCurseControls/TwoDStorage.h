#pragma once
#include <algorithm>
#include <vector>
#include <assert.h>
#include <fstream>
#include "Storable.h"
#include "ITwoDStorage.h"

template <class T>
class TwoDStorage : public ITwoDStorage<T>
{
protected:
	int rows = 0;
	int cols = 0;
	std::vector<T> data;

	//resize the underlying vector but maintain the 2d layout. Use fillValue if the size is larger in either dimension for unintialized data
	void resize(int newRows, int newCols, std::vector<T>& copyData);
public:
	TwoDStorage() {};
	TwoDStorage(int rows, int cols);

	/*Set the 2d dimensions of storage. Will resize from previous size to match extent and retains the layout of the individual data elements.*/
	void setDimensions(int rows, int cols);

	/*same as setDimensions but uses a fillValue to initialize all new elements. 
	If being used for the first time, then the data will be filled with fillValue.
	If being resized from earlier, then the data outside the perimeter of the original size with get the fillvalue.*/
	void setDimensions(int rows, int cols, const T fillValue); 
	void fill(const T datum); //fill all elements in storage with T
	//void copyFrom(TwoDStorage<T>& fromStorage);

	void clearData();
	
	T getDatum(int element);
	T getDatum(int y, int x);
	
	bool setDatum(int element, const T &datum);
	bool setDatum(int y, int x, const T &datum);

	//Storable override
	void save(std::ofstream& saveFile);
	void load(std::ifstream& loadFile);

	//getters/setters
	int getSize() { return data.size(); }
	int getRows() { return rows; }
	int getCols() { return cols; }
	std::vector<T>& getData() { return data; }
};


template <class T>
TwoDStorage<T>::TwoDStorage(int rows, int cols)
{
	setDimensions(rows, cols);
}

template <class T>
void TwoDStorage<T>::setDimensions(int newRows, int newCols)
{
	if (data.empty())//nothing to save so initialize
	{
		data.resize(newRows * newCols);
	}
	else
	{
		std::vector<T> copyData(newRows * newCols);
		resize(newRows, newCols, copyData);
	}
	rows = newRows;
	cols = newCols;
}

template <class T>
void TwoDStorage<T>::setDimensions(int newRows, int newCols, const T fillValue)
{
	//same as widen, but with fillvalue
	if (data.empty())//nothing to save so initialize
	{
		data.resize(newRows * newCols, fillValue);
	}
	else
	{
		std::vector<T> copyData(newRows * newCols, fillValue);
		resize(newRows, newCols, copyData);
	}
	
	rows = newRows;
	cols = newCols;
}


template <class T>
void TwoDStorage<T>::resize(int newRows, int newCols, std::vector<T>& copyData)
{
	int copyRows = std::min(newRows, rows); //copy only what will fit in the current data storage
	int copyCols = std::min(newCols, cols);

	for (int row = 0; row < copyRows; row++)
	{
		for (int col = 0; col < copyCols; col++)
		{
			int newElement = row * newCols + col;
			copyData[newElement] = getDatum(row, col);
		}
	}

	//now resize data and copy data back from copyData vector
	data = copyData;
}


template <class T>
T TwoDStorage<T>::getDatum(int y, int x)
{
	assert(y < rows && x < cols);
	
	return getDatum(y * cols + x);
}

template <class T>
T TwoDStorage<T>::getDatum(int element)
{
	assert(element < getSize());
	
	return data[element];
}


template <class T>
bool TwoDStorage<T>::setDatum(int y, int x, const T &datum)
{
	if (y >= rows || x >= cols)
		return false;

	return setDatum(y * cols + x, datum);
}

template <class T>
bool TwoDStorage<T>::setDatum(int element, const T &datum)
{
	if (element >= getSize())
		return false;

	data[element] = datum;
	return true;
}

template <class T>
void TwoDStorage<T>::fill(const T datum)
{
	std::fill(data.begin(), data.end(), datum);
}

template <class T>
void TwoDStorage<T>::clearData()
{
	data.clear();
	rows = cols = 0;
}

/*
copyFrom is not the same as a copy constructor. It doesn't make an exact duplicate. 
It copies as much data from fromStorage as can fit in toStorage without exceeding toStorages dimensions
*/
//template <class T>
//void TwoDStorage<T>::copyFrom(TwoDStorage<T>& fromStorage)
//{
//	int copyRows = std::min(rows, fromStorage.getRows()); //copy only what will fit in the current data storage
//	int copyCols = std::min(cols, fromStorage.getCols());
//
//	for (int row = 0; row < copyRows; row++)
//	{
//		for (int col = 0; col < copyCols; col++)
//		{
//			setDatum(row, col, fromStorage.getDatum(row, col));
//		}
//	}
//}


//Storable override
template <class T>
void TwoDStorage<T>::save(std::ofstream& saveFile)
{
	saveFile.write((char*)&rows, sizeof(int))
		.write((char*)&cols, sizeof(int));

	T datum;
	for (int i = 0; i < (int)data.size(); i++)
	{
		datum = data[i];
		saveFile.write((char*)&datum, sizeof(T));
	}
}

template <class T>
void TwoDStorage<T>::load(std::ifstream& loadFile)
{
	int newRows;
	int newCols;
	loadFile.read((char*)&newRows, sizeof(int))
		.read((char*)&newCols, sizeof(int));

	TwoDStorage::setDimensions(newRows, newCols);

	T datum;
	for (int i = 0; i < (int)data.size(); i++)
	{
		loadFile.read((char*)&datum, sizeof(T));
		setDatum(i, datum);
	}
}