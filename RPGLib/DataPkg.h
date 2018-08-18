#pragma once
#include <iostream>
#include <vector>
#include "Storable.h"

const int BLOCK_SIZE = 64;

/*
A class for storing a series of fixed-sized blocks of data in memory.
Any data type can be stored in the blocks.
*/
class DataPkg
{
private:
	std::vector<char*> blocks;

	std::streampos readOffset; //the current position in the data blocks to read data from. Initialized to 0 automatically, maybe because it is a streampos datatype.
	std::streampos appendOffset; //the current position in the data blocks to append new data
	
	void createNewBlock();
	void writeDataToBlocks(const void* datum, int size, int offset, int startBlock, int endBlock);
	void readDataFromBlocks(const void* datum, int size, int offset, int startBlock, int endBlock);

	void writeBlock(const void* datum, int datumOffset, int size, int block, int blockOffset);
	void readBlock(const void* datum, int datumOffset, int size, int block, int blockOffset);
	void blockOp(bool op, const void* datum, int datumOffset, int size, int block, int blockOffset);

public:
	DataPkg();
	~DataPkg();

	//Writes data to blocks at any offset. More blocks will be allocated if necessary.
	void writeData(const void* datum, int size, int offset);

	//Reads data from blocks at any offset. Will read nothing if beyond the end of the blocks.
	void readData(const void* datum, int size, int offset);

	//Appends size of datum to appendOffset location in data blocks. More blocks will be allocated if necessary.
	void appendData(const void* datum, int size);

	//Reads data starting at readOffset
	void readNextData(const void* datum, int size);

	//Shift readOffset relative to current location. 
	void shiftReadOffset(int offset);

	//Move readOffset to offset. 
	void setReadOffset(int offset) { readOffset = offset; }

	int getSize() { return blocks.size() * BLOCK_SIZE; }

	int save(std::ofstream* saveFile);
	int load(std::ifstream* loadFile);
	int load(std::ifstream* loadFile, unsigned int nblocks);
};