#include "MockFileDirectory.h"

MockFileDirectory::MockFileDirectory() : FileDirectory("FakePath")
{
	
}


std::list<dirent> MockFileDirectory::getFiles(bool includeDirectories, const std::string& filter)
{
	std::list<dirent> files;
	dirent entry;
	entry.d_type = DT_REG;
	files.push_back(entry);
	return files;
}