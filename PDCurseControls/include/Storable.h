#pragma once
#include <fstream> 
#include <iostream>
/*
An Abstract class for saving and loading data
*/
class Storable
{
public:
	//return value should be # of bytes written. -1 indicates error
	virtual int save(std::ofstream& saveFile) = 0; 

	//return value should be # of bytes read. -1 indicates error
	virtual int load(std::ifstream& loadFile) = 0;
};