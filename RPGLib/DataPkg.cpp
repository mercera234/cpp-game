#include "DataPkg.h"
#include <fstream>

const bool write_Block = true;
const bool read_Block = false;


DataPkg::DataPkg()
{
	createNewBlock();
	appendOffset = 0;
}

void DataPkg::createNewBlock()
{
	char* dataBlock = new char[BLOCK_SIZE];
	memset(dataBlock, 0, BLOCK_SIZE);

	blocks.push_back(dataBlock);
}

void DataPkg::writeData(const void* datum, int size, int offset)
{
	//calculate how many blocks are needed
	unsigned int startBlockNdx = offset / BLOCK_SIZE;
	unsigned int endBlockNdx = (offset + size) / BLOCK_SIZE;
	
	//check if write is within allocated blocks
	if (blocks.size() <= endBlockNdx) //is not big enough
	{
		int blocksToAdd = (endBlockNdx + 1) - blocks.size();
		for (int i = 0; i < blocksToAdd; i++)
		{
			createNewBlock();
		}
	}

	writeDataToBlocks(datum, size, offset, startBlockNdx, endBlockNdx);

	//check if append offset should be modified
	appendOffset = (offset + size > appendOffset) ? offset + size : appendOffset;
	
}

void DataPkg::readData(const void* datum, int size, int offset)
{
	//calculate how many blocks are needed
	unsigned int startBlockNdx = offset / BLOCK_SIZE;
	unsigned int endBlockNdx = (offset + size) / BLOCK_SIZE;

	//check if offset is within the range of blocks
	if (endBlockNdx >= blocks.size())
		return;

	readDataFromBlocks(datum, size, offset, startBlockNdx, endBlockNdx);

	readOffset = offset + size; 
}



void DataPkg::writeDataToBlocks(const void* datum, int size, int offset, int startBlock, int endBlock)
{
	int bytesLeft = size;
	int datumOffset = 0;

	//write to start block
	int blockOffset = offset - (startBlock * BLOCK_SIZE);
	int writeSize = BLOCK_SIZE - blockOffset;
	writeSize = size < writeSize ? size : writeSize;

	writeBlock(datum, datumOffset, writeSize, startBlock, blockOffset);

	bytesLeft -= writeSize;
	datumOffset += writeSize;

	//write to mid blocks
	for (int block = startBlock + 1; block < endBlock; block++)
	{
		writeBlock(datum, datumOffset, BLOCK_SIZE, block, 0);
		bytesLeft -= BLOCK_SIZE;
		datumOffset += BLOCK_SIZE;
	}

	//write to end block
	writeBlock(datum, datumOffset, bytesLeft, endBlock, 0);
}

void DataPkg::readDataFromBlocks(const void* datum, int size, int offset, int startBlock, int endBlock)
{
	int bytesLeft = size;
	int datumOffset = 0;

	//read from start block
	int blockOffset = offset - (startBlock * BLOCK_SIZE);
	int readSize = BLOCK_SIZE - blockOffset;
	readSize = size < readSize ? size : readSize;

	readBlock(datum, datumOffset, readSize, startBlock, blockOffset);

	bytesLeft -= readSize;
	datumOffset += readSize;

	//read from mid blocks
	for (int block = startBlock + 1; block < endBlock; block++)
	{
		readBlock(datum, datumOffset, BLOCK_SIZE, block, 0);
		bytesLeft -= BLOCK_SIZE;
		datumOffset += BLOCK_SIZE;
	}

	//read from end block
	readBlock(datum, datumOffset, bytesLeft, endBlock, 0);
}


void DataPkg::writeBlock(const void* datum, int datumOffset, int size, int block, int blockOffset)
{
	char* dataBlock = blocks[block];
	memcpy_s(dataBlock + blockOffset, size, (char*)datum + datumOffset, size);
}


void DataPkg::readBlock(const void* datum, int datumOffset, int size, int block, int blockOffset)
{
	char* dataBlock = blocks[block];
	memcpy_s((char*)datum + datumOffset, size, dataBlock + blockOffset, size);
}

void DataPkg::blockOp(bool op, const void* datum, int datumOffset, int size, int block, int blockOffset)
{
	char* dataBlock = blocks[block];
								       //write params			   //read params
	void* dest = (op == write_Block) ? dataBlock + blockOffset : (char*)datum + datumOffset;
	void* src = (op == write_Block) ? (char*)datum + datumOffset : dataBlock + blockOffset;

	memcpy_s(dest, size, src, size);
}



void DataPkg::appendData(const void* datum, int size)
{
	writeData(datum, size, appendOffset);
}

void DataPkg::readNextData(const void* datum, int size)
{
	readData(datum, size, readOffset);
}


void DataPkg::shiftReadOffset(int offset)
{
	readOffset += offset;
}


int DataPkg::save(std::ofstream* saveFile)
{
	for each (char* block in blocks)
	{
		saveFile->write(block, BLOCK_SIZE);
	}

	return BLOCK_SIZE * blocks.size();
}

/*
Loads all blocks
*/
int DataPkg::load(std::ifstream* loadFile)
{
	loadFile->seekg(0, loadFile->end);
	int fileSize = (int)loadFile->tellg();
	loadFile->seekg(0, loadFile->beg);
	
	int fullBlocks = fileSize / BLOCK_SIZE;
	int partialBlocks = fileSize % BLOCK_SIZE > 0 ? 1 : 0;
	int fileBlocks = fullBlocks + partialBlocks;

	return load(loadFile, fileBlocks);
}

/*
Load nblocks only
*/
int DataPkg::load(std::ifstream* loadFile, unsigned int nblocks)
{
	for (unsigned int i = 0; i < nblocks; i++)
	{
		if(i >= blocks.size()) //create new blocks as needed
			createNewBlock();

		char* dataBlock = blocks[i];
		loadFile->read(dataBlock, BLOCK_SIZE);
	}

	return BLOCK_SIZE * nblocks;
}



DataPkg::~DataPkg()
{
	//need to 
}