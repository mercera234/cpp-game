#include "DataPkg.h"
#include <fstream>


void* DataPkg::create(int size)
{
	dataBuf = (void*) new char*[this->size = size];
	memset(dataBuf, 0, size);
	return dataBuf;
}

void DataPkg::free()
{
	size = 0;
	delete dataBuf;
}

bool DataPkg::save(string fileName)
{
	ofstream file;

	file.open(fileName, ios::trunc | ios::binary);
	if (file.is_open() == false)
		return false;

	file.write((char*)dataBuf, size);
	file.close();

	return true;
}

bool DataPkg::load(string fileName)
{
	return true;
}