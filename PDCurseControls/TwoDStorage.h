#pragma once
#include <algorithm>
#include <vector>
#include <assert.h>
#include <fstream>
#include "Storable.h"

template <class T>
class TwoDStorage : public Storable
{
protected:
	unsigned int rows = 0;
	unsigned int cols = 0;
	std::vector<T> data;

	//resize the underlying vector but maintain the 2d layout. Use fillValue if the size is larger in either dimension for unintialized data
	void resize(unsigned int newRows, unsigned int newCols, std::vector<T>& copyData);
public:
	TwoDStorage() {};
	TwoDStorage(unsigned int rows, unsigned int cols);

	/*Set the 2d dimensions of storage. If resizing from a previous size, retains the layout of the individual data elements.*/
	void setDimensions(unsigned int rows, unsigned int cols);
	void setDimensions(unsigned int rows, unsigned int cols, const T fillValue); //same as setDimensions but uses a fillValue to initialize all elements
	void fill(const T datum); //fill all elements in storage with T
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

	//Storable override
	int save(std::ofstream& saveFile);
	int load(std::ifstream& loadFile);
};


//Storable override
template <class T>
int TwoDStorage<T>::save(std::ofstream& saveFile)
{
	std::streampos startPos = saveFile.tellp();

	saveFile.write((char*)&rows, sizeof(int))
			.write((char*)&cols, sizeof(int));

	T datum;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		datum = data[i];
		saveFile.write((char*)&datum, sizeof(T));
	}

	std::streampos endPos = saveFile.tellp();

	return (int)(endPos - startPos);
}

template <class T>
int TwoDStorage<T>::load(std::ifstream& loadFile)
{
	std::streampos startPos = loadFile.tellg();

	loadFile.read((char*)&rows, sizeof(int))
			.read((char*)&cols, sizeof(int));

	setDimensions(rows, cols);

	T datum;
	for (unsigned int i = 0; i < data.size(); i++)
	{
		loadFile.read((char*)&datum, sizeof(T));
		setDatum(i, datum);
	}

	std::streampos endPos = loadFile.tellg();

	return (int)(endPos - startPos);
}

template <class T>
TwoDStorage<T>::TwoDStorage(unsigned int rows, unsigned int cols)
{
	setDimensions(rows, cols);
}

template <class T>
void TwoDStorage<T>::setDimensions(unsigned int newRows, unsigned int newCols)
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
void TwoDStorage<T>::setDimensions(unsigned int newRows, unsigned int newCols, const T fillValue)
{
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
void TwoDStorage<T>::resize(unsigned int newRows, unsigned int newCols, std::vector<T>& copyData)
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


//TODO alter the next 2 methods to return T& so that nullptr can be returned instead of throwing an exception
template <class T>
T TwoDStorage<T>::getDatum(unsigned int y, unsigned int x)
{
	assert(y < rows && x < cols);
	
	return getDatum(y * cols + x);
}

template <class T>
T TwoDStorage<T>::getDatum(unsigned int element)
{
	assert(element < data.size());
	
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
void TwoDStorage<T>::fill(const T datum)
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

