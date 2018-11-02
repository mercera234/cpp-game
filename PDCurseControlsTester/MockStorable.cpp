#include "MockStorable.h"

void MockStorable::save(std::ofstream& saveFile)
{
	saveOccurred = true;
}


void MockStorable::load(std::ifstream& loadFile)
{
	loadOccurred = true;
}