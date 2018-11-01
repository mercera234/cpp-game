#pragma once
#include <fstream> 
#include <iostream>
/*
An Abstract class for saving and loading data
*/
class Storable
{
public:
	virtual void save(std::ofstream& saveFile) = 0; 
	virtual void load(std::ifstream& loadFile) = 0;
};