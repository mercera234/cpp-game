#pragma once
#include <string>
#include <dirent.h>
#include <list>

const std::string curDir = ".";

class FileDirectory
{
private:
	std::string path;
	bool checkFilterMatch(const std::string& fileName, const std::string& filter);
public:
	static std::streampos getSize(const std::string& fileName);
	
	FileDirectory(const std::string& path);

	//Returns files in the path located by the path member variable
	//We couldn't use dirent* because the readdir function always returns the same pointer when reading through a directory!
	std::list<dirent> getFiles(bool includeDirectories = false, const std::string& filter = "");
	
	//getters/setters
	std::string getPath() { return path; }
	void setPath(const std::string& newPath) { path = newPath; }
	void addDirToPath(const std::string& newDir);
	void removeDirFromPath();
};