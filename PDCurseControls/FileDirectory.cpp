#include "FileDirectory.h"
#include <fstream>

FileDirectory::FileDirectory()
{

}

FileDirectory::FileDirectory(const std::string& path)
{
	setPath(path);
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


bool FileDirectory::checkFilterMatch(const std::string& fileName, const std::string& filter)
{
	if (filter.empty())
		return true; //include all files when filter isn't set

	std::string checkStr = fileName.substr(fileName.length() - filter.length());
	if (checkStr.compare(filter) != 0)
		return false;

	return true;
}

void FileDirectory::addToPath(const std::string& newDir)
{
	path.append('\\' + newDir);
}

void FileDirectory::removeLastFromPath()
{
	path.erase(path.find_last_of('\\'));	
}

void addToFilePath(std::string& filePath, const std::string& pathElement)
{
	filePath.append('\\' + pathElement);
}

void removeFromFilePath(std::string& filePath, int levels)
{
	for (int i = 0; i < levels; i++)
	{
		filePath.erase(filePath.find_last_of('\\'));
	}
}

bool validatePath(const std::string& filePath)
{
	std::ifstream is(filePath);
	if (is.is_open() == false)
	{
		return false;
	}

	is.close();
	return true;
}

bool validateDir(const std::string& dirPath)
{
	DIR* dir = opendir(dirPath.c_str());
	if (dir == nullptr)
	{
		return false;
	}
	closedir(dir);
	return true;
}

void decomposePath(const std::string& filePath, std::string& dirPath, std::string& fileName)
{
	//break up path into working dir and file name
	dirPath = filePath;
	removeFromFilePath(dirPath);

	fileName = getFileFromPath(filePath);
}

std::string getFileFromPath(const std::string& filePath)
{
	return filePath.substr(filePath.find_last_of('\\'));
}


/*
Returns size of fileName
*/
std::streampos FileDirectory::getSize(const std::string& fileName) //static
{
	std::ifstream is (fileName, std::ios::binary | std::ios::ate);
	std::streampos size = is.tellg();
	is.close();
	return size;
}