#pragma once

template <class T>
class _2DStorage
{
private:
	T* data;
	unsigned int rows;
	unsigned int cols;
	unsigned int size;
public:
	_2DStorage(int rows, int cols);
	~_2DStorage();
	T* getData() { return data; }
	T getDatum(int y, int x);
	T getDatum(int element);
	unsigned int getSize() { return size; }

	//for objects
	bool setDatum(int y, int x, T* datum);
	bool setDatum(int element, T* datum);

	//for primitive types
	bool setDatum(int y, int x, T datum);
	bool setDatum(int element, T datum);
};

template <class T>
_2DStorage<T>::_2DStorage(int rows, int cols)
{
	this->rows = rows;
	this->cols = cols;

	size = rows * cols;
	data = new T[size];
}

template <class T>
T _2DStorage<T>::getDatum(int y, int x)
{
	if (y < 0 || x < 0 || y >= rows || x >= cols)
		return NULL;

	return getDatum(y * cols + x);
}

template <class T>
T _2DStorage<T>::getDatum(int element)
{
	if (element < 0 || element >= size)
		return NULL;

	return data[element];
}

//for objects
template <class T>
bool _2DStorage<T>::setDatum(int y, int x, T* datum)
{
	if (y < 0 || x < 0 || y >= rows || x >= cols)
		return false;

	return setDatum(y * cols + x, datum);
}

template <class T>
bool _2DStorage<T>::setDatum(int element, T* datum)
{
	if (element < 0 || element >= size)
		return false;

	data[element] = datum;
	return true;
}

//for primitives
template <class T>
bool _2DStorage<T>::setDatum(int y, int x, T datum)
{
	if (y < 0 || x < 0 || y >= rows || x >= cols)
		return false;

	return setDatum(y * cols + x, datum);
}

template <class T>
bool _2DStorage<T>::setDatum(int element, T datum)
{
	if (element < 0 || element >= size)
		return false;

	data[element] = datum;
	return true;
}

template <class T>
_2DStorage<T>::~_2DStorage()
{
	delete data;
	rows = cols = size = 0;
}