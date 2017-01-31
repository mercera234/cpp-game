#pragma once
#include <iostream>
using namespace std;

class DataPkg
{
private:
	void* dataBuf;
	unsigned int size;

public:
	void* create(int size);
	void free();
	bool save(string fileName);
	bool load(string fileName);
};