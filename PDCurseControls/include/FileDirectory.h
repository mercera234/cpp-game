#pragma once
#include <string>
#include <dirent.h>
#include <list>

class FileDirectory
{
private:
	std::string path;
	bool checkFilterMatch(std::string fileName, std::string filter);
public:
	static std::streampos getSize(std::string& fileName);
	
	FileDirectory(const std::string& path);

	//Returns files in the path located by the path member variable
	//We couldn't use dirent* because the readdir function always returns the same pointer when reading through a directory!
	std::list<dirent> getFiles(bool includeDirectories = false, const std::string& filter = "");
	
	
	std::string getPath() { return path; }
};