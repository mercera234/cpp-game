#pragma once
#include <string>
#include <dirent.h>
#include <list>

const std::string curDir = ".";

/*Add another dir or file name to path. The item type is not specified.*/
void addToFilePath(std::string& filePath, const std::string& pathElement);

/*Remove the last (levels) file name or dir from path.*/
void removeFromFilePath(std::string& filePath, int levels = 1);

/*Get just the fileName from the provided path.
Assumes that the path provided ends in a file name and not a directory.*/
std::string getFileFromPath(const std::string& filePath);

void decomposePath(const std::string& filePath, std::string& dirPath, std::string& fileName);

/*Validate that the filePath is on harddrive.
This is meant for checking a file and not a directory.
This doesn't verify the syntax of the path, because it doesn't need to.*/
bool validatePath(const std::string& filePath);

bool validateDir(const std::string& dirPath);

class FileDirectory
{
private:
	std::string path;
	bool checkFilterMatch(const std::string& fileName, const std::string& filter);
	
public:
	static std::streampos getSize(const std::string& fileName);
	
	FileDirectory();
	FileDirectory(const std::string& path);

	//Returns files in the path located by the path member variable
	//We couldn't use dirent* because the readdir function always returns the same pointer when reading through a directory!
	std::list<dirent> getFiles(bool includeDirectories = false, const std::string& filter = "");
	
	void addToPath(const std::string& newDir);
	void removeLastFromPath();

	//getters/setters
	std::string& getPath() { return path; }
	void setPath(const std::string& newPath) { path = newPath; }
};