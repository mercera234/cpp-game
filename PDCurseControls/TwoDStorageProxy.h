#pragma once
#include "TwoDStorage.h"
#include "FilePath.h"

template <class T>
class TwoDStorageProxy : public ITwoDStorage<T>
{
private:
	TwoDStorage<T> real;
	FilePath* filePath = nullptr;
	int extentRows = 0;
	int extentCols = 0;

	/*Loads the real storage object. This will be done using the specified fileName, or if it is invalid, the extant will be used.
	Returns the number of bytes loaded. Will be 0 if nothing was loaded, even if data is widened to meet extant.*/
	int loadReal();
public:
	TwoDStorageProxy() {}
	TwoDStorageProxy(int newRows, int newCols);
	
	void setDimensions(int rows, int cols); //override
	

	//methods that must trigger loading of data

	void setDimensions(int rows, int cols, const T fillValue); //override
	void fill(const T datum); //override
	
	T getDatum(int element); //override
	T getDatum(int y, int x); //override
	
	bool setDatum(int element, const T &datum); //override 
	bool setDatum(int y, int x, const T &datum); //override

	void save(std::ofstream& saveFile); //override
	void load(std::ifstream& loadFile); //override

	void hideData(bool saveData = false);


	//setters/getters

	int getSize() { return extentRows * extentCols; } //override
	int getRealSize() { return real.getSize(); }
	int getRows() { return extentRows; } //override
	int getCols() { return extentCols; } //override

	void setFilePath(FilePath* filePathIn) { filePath = filePathIn; }
	FilePath* getFilePath() { return filePath; }
	//TwoDStorage<T>& getReal() { return real; }
};


template <class T>
TwoDStorageProxy<T>::TwoDStorageProxy(int newRows, int newCols)
{
	setDimensions(newRows, newCols);
}

template <class T>
int TwoDStorageProxy<T>::loadReal()
{
	int dataLoaded = 0;
	if (getRealSize() == 0) //actual data is not loaded
	{
		if (filePath != nullptr)
		{
			filePath->setObject(&real); //reset the object to load so any other object is not read in by mistake
			dataLoaded = filePath->load();
			setDimensions(real.getRows(), real.getCols());
		}
			
		if (dataLoaded <= 0)
		{
			//if file can't be loaded for whatever reason, then widen the storage to match the extent
			real.setDimensions(extentRows, extentCols);
		}
	}
	return dataLoaded;
}


template <class T>
void TwoDStorageProxy<T>::save(std::ofstream& saveFile)
{
	real.save(saveFile);
}

template <class T>
void TwoDStorageProxy<T>::load(std::ifstream& loadFile)
{
	real.load(loadFile);
	setDimensions(real.getRows(), real.getCols());
}

template <class T>
void TwoDStorageProxy<T>::setDimensions(int rows, int cols)
{
	//set extent only
	this->extentRows = rows;
	this->extentCols = cols;
}

template <class T>
void TwoDStorageProxy<T>::hideData(bool saveData)
{
	if (saveData)
	{
		if (filePath == nullptr)
			return;

		filePath->save();
	}
	
	real.clearData(); //erase the data, but the extant is not touched
}

template <class T>
void TwoDStorageProxy<T>::setDimensions(int newRows, int newCols, const T fillValue)
{
	loadReal();
		
	setDimensions(newRows, newCols);
	real.setDimensions(extentRows, extentCols, fillValue);
}


template <class T>
void TwoDStorageProxy<T>::fill(const T datum)
{
	loadReal();
	
	real.fill(datum);
}

template <class T>
T TwoDStorageProxy<T>::getDatum(int element)
{
	loadReal();

	return real.getDatum(element);
}


template <class T>
T TwoDStorageProxy<T>::getDatum(int y, int x)
{
	loadReal();

	return real.getDatum(y, x);
}

template <class T>
bool TwoDStorageProxy<T>::setDatum(int element, const T &datum)
{
	loadReal();

	return real.setDatum(element, datum);
}

template <class T>
bool TwoDStorageProxy<T>::setDatum(int y, int x, const T &datum)
{
	loadReal();

	return real.setDatum(y, x, datum);
}

