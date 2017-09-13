#include "FileDirectory.h"
#include <fstream>

FileDirectory::FileDirectory(const std::string& path)
{
	this->path = path;
}

std::list<dirent> FileDirectory::getFiles(bool includeDirectories, const std::string& filter)
{
	DIR* dir = opendir(path.c_str());
	dirent* entry;

	int currElement = 0;
	std::list<dirent> files;

	//create list of entries in directory
	while ((entry = readdir(dir)) != NULL)
	{
		bool filterMatch = false;

		if (entry->d_type == DT_REG)
		{
			filterMatch = checkFilterMatch(entry->d_name, filter);
		}

		if (filterMatch || (includeDirectories && entry->d_type == DT_DIR))
		{
			files.push_back(*entry);
		}
	}

	return files;
}


bool FileDirectory::checkFilterMatch(std::string fileName, std::string filter)
{
	if (filter.empty())
		return true; //include all files when filter isn't set

	std::string checkStr = fileName.substr(fileName.length() - filter.length());
	if (checkStr.compare(filter) != 0)
		return false;

	return true;
}

/*
Returns size of fileName
*/
std::streampos FileDirectory::getSize(std::string& fileName) //static
{
	std::ifstream is (fileName, std::ios::binary | std::ios::ate);
	std::streampos size = is.tellg();
	is.close();
	return size;
}