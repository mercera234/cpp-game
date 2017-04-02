#pragma once
#include <fstream> 
/*
An Abstract class for saving and loading data
*/
class Storable
{
public:
	virtual bool save(ofstream* saveFile) = 0;
	virtual bool load(ifstream* loadFile) = 0;
};