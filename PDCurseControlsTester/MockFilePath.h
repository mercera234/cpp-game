#pragma once
#include "FilePath.h"
class MockFilePath : public FilePath
{
public:
	MockFilePath(const std::string& workingDirIn);

	int save(std::ofstream& saveFile);
	int load(std::ifstream& loadFile);
	int save();
	int load();
};

