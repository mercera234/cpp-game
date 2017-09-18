#include "MockStorable.h"

int MockStorable::save(std::ofstream& saveFile)
{
	return 600; //random amount of bytes to return
}


int MockStorable::load(std::ifstream& loadFile)
{
	return 600; //random amount of bytes to return
}