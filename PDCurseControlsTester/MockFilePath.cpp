#include "MockFilePath.h"

MockFilePath::MockFilePath(const std::string& workingDirIn) : FilePath(workingDirIn)
{

}

int MockFilePath::save(std::ofstream& saveFile)
{
	if (object == nullptr)
		return -1;

	object->save(saveFile);

	return 200;
}

int MockFilePath::load(std::ifstream& loadFile)
{
	if (object == nullptr)
		return -1;

	object->load(loadFile);

	return 400;
}

int MockFilePath::save()
{
	std::ofstream os;//stream objects never touch file system in mock class
	return save(os);
}

int MockFilePath::load()
{
	std::ifstream is;
	return load(is);
}
