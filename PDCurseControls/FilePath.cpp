#include "FilePath.h"

FilePath::FilePath()
{
	//setup filedirectory to default working dir path
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	std::string dir = buf;
	
	workingDir.setPath(dir);
}

FilePath::FilePath(const std::string& workingDirIn, Storable* objectIn)
{
	workingDir.setPath(workingDirIn);
	setObject(objectIn);
}

void FilePath::openFolder(const std::string& dirName)
{
	workingDir.addToPath(dirName);
}

void FilePath::setPath(const std::string& filePath)
{
	workingDir.setPath(filePath);
}

std::string& FilePath::getPath()
{
	return workingDir.getPath();
}

int FilePath::save(const std::string& saveFile, bool isFullPath)
{
	if (isFullPath)
	{
		decomposePath(saveFile, workingDir.getPath(), fileName);	
	}
	else
		fileName = saveFile;

	return save();
}

int FilePath::load(const std::string& loadFile, bool isFullPath)
{
	if (isFullPath)
	{
		decomposePath(loadFile, workingDir.getPath(), fileName);
	}
	else
		fileName = loadFile;
	return load();
}

int FilePath::save(std::ofstream& saveFile)
{
	if (object == nullptr)
		return -1;

	std::streampos startPos = saveFile.tellp();
	object->save(saveFile);
	std::streampos endPos = saveFile.tellp();
	return (int)(endPos - startPos);
}

int FilePath::load(std::ifstream& loadFile)
{
	if (object == nullptr)
		return -1;

	std::streampos startPos = loadFile.tellg();
	object->load(loadFile);
	std::streampos endPos = loadFile.tellg();
	return (int)(endPos - startPos);
}

int FilePath::save()
{
	std::ofstream os(workingDir.getPath() + '\\' + fileName, writeMode);
	return save(os);
}

int FilePath::load()
{
	std::ifstream is(workingDir.getPath() + '\\' + fileName, std::ios::binary);
	return load(is);
}