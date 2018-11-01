#pragma once
#include <fstream>
#include "FileDirectory.h"
#include "Storable.h"

class FilePath
{
protected:
	FileDirectory workingDir;
	std::string fileName = "";
	Storable* object = nullptr;
	std::ios_base::openmode writeMode = std::ios::binary | std::ios::trunc;
public:
	FilePath();
	FilePath(const std::string& workingDirIn, Storable* objectIn = nullptr);

	/*open folder relative to the current working dir
	currently doesn't accept .. folder*/
	void openFolder(const std::string& dirName);

	void setPath(const std::string& filePath);
	std::string& getPath();

	/*Save using a pre-initialized ofstream.*/
	int save(std::ofstream& saveFile);
	
	/*Load using a pre-initialized ofstream.*/
	int load(std::ifstream& loadFile);

	/*Save to the specified file. 
	If isFullPath is false, then the file name will be appended to the working dir path.
	If isFullPath is true, then the path of the file will replace workingdir*/
	int save(const std::string& saveFile, bool isFullPath = false);

	/*Save using a pre-initialized ofstream.
	If isFullPath is false, then the file name will be appended to the working dir path.
	If isFullPath is true, then the path of the file will replace workingdir*/
	int load(const std::string& loadFile, bool isFullPath = false);

	/*Save directly to fileName in working dir.*/
	int save();

	/*Load directly from fileName in working dir.*/
	int load();

	//setters/getters

	void setFileName(const std::string& fileNameIn) { fileName = fileNameIn; }
	const std::string& getFileName() { return fileName; }



	void setWriteMode(std::ios_base::openmode writeModeIn) { writeMode = writeModeIn; }
	std::ios_base::openmode getWriteMode() { return writeMode; }

	void setObject(Storable* objectIn) { object = objectIn; }
	Storable* getObject() { return object; }
};

