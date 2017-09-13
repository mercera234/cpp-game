#pragma once
#include "FileDirectory.h"

class MockFileDirectory : public FileDirectory
{
private:
public:
	//MockFileDirectory(const std::string& path);
	MockFileDirectory();
	std::list<dirent> getFiles(bool includeDirectories = false, const std::string& filter = "");
};
