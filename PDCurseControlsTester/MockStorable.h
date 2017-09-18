#pragma once
#include <fstream>
#include "Storable.h"

class MockStorable : public Storable
{
private:
public:
	//Storable overrides
	//return value should be # of bytes written. -1 indicates error
	int save(std::ofstream& saveFile);

	//return value should be # of bytes read. -1 indicates error
	int load(std::ifstream& loadFile);
};