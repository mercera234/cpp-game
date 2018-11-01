#pragma once
#include <fstream>
#include "Storable.h"

class MockStorable : public Storable
{
private:
public:
	bool saveOccurred = false;
	bool loadOccurred = false;
	//Storable overrides
	void save(std::ofstream& saveFile);
	void load(std::ifstream& loadFile);
};