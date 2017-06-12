#pragma once
#include <map>

template <class T>
class Repository
{
private:
public:
	virtual void add(T& obj) = 0; //add one object of type T
	virtual T* find(unsigned short id) = 0;
};


