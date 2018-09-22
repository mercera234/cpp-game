#pragma once
#include "TwoDStorage.h"

template <class T>
class TwoDStorageProxy : public TwoDStorage<T>
{
private:
	std::string fileName;

	void checkActual();
public:
	TwoDStorageProxy() {}
	
	void setDimensions(unsigned int rows, unsigned int cols);
	void fill(const T datum); //fill all elements in storage with T
	void setFileName(const std::string& fileNameIn) { fileName = fileNameIn; }
	std::string& getFileName() { return fileName; }
	T getDatum(unsigned int y, unsigned int x);
	bool setDatum(unsigned int y, unsigned int x, const T &datum);
};


template <class T>
void TwoDStorageProxy<T>::checkActual()
{
	if (data.size() == 0)
	{
		std::ifstream is(fileName, std::ios::binary);
		load(is);
		is.close();
	}
}

template <class T>
void TwoDStorageProxy<T>::setDimensions(unsigned int rows, unsigned int cols)
{
	checkActual();

	TwoDStorage::setDimensions(rows, cols);
}

template <class T>
void TwoDStorageProxy<T>::fill(const T datum)
{
	checkActual();
	
	TwoDStorage::fill(datum);
}

template <class T>
T TwoDStorageProxy<T>::getDatum(unsigned int y, unsigned int x)
{
	checkActual();

	return TwoDStorage::getDatum(y, x);
}

template <class T>
bool TwoDStorageProxy<T>::setDatum(unsigned int y, unsigned int x, const T &datum)
{
	checkActual();

	return TwoDStorage::setDatum(y, x, datum);
}