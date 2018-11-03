#include "EditMode.h"
//#include <dirent.h>
#include <fstream>

std::string EditMode::dialogDefPath;
std::string EditMode::loadedFileName = DEF_FILENAME;
bool EditMode::modified = false;

EditMode::EditMode()
{
	cm.setCaller(this);
}

void EditMode::resetFileName()
{
	loadedFileName = DEF_FILENAME;
}

bool EditMode::hasSaved()
{
	return loadedFileName.compare(DEF_FILENAME) != 0;
}

void EditMode::storeLastOpened(const std::string& path)
{
	int pos = path.find_last_of('\\');
	setFileName(path.substr(pos + 1, path.length()));

	//save path that file was opened/saved from as the start point for next time
	setDefaultFilePath(path.substr(0, pos));
}

void EditMode::setModified(bool mod)
{
	modified = mod;
}

std::string EditMode::getFileNameModified()
{
	return isModified() ? getFileName().substr(0, 14) + "*" : getFileName();
}

void EditMode::createNew()
{
	resetFileName();
	setModified(false);
}

void EditMode::save()
{
	save(dialogDefPath + '\\' + loadedFileName);	
}

EditMode::~EditMode()
{
}


void MapRoomEditMode::save(const std::string& fileName)
{
	std::ofstream os(fileName, std::ios::trunc | std::ios::binary);
	
	mapRoom.save(os);
	setModified(false);
}

void MapRoomEditMode::load(const std::string& fileName)
{
	std::ifstream is(fileName, std::ios::binary);
	if (is.is_open() == false)
		return;

	mapRoom.load(is);
	setModified(false);
}

void MapRoomEditMode::createNew()
{
	mapRoom.reset();
	EditMode::createNew();
}


void ImageEditMode::save(const std::string& fileName)
{
	std::ofstream os(fileName, std::ios::trunc | std::ios::binary);

	image->save(os);
	setModified(false);
}

void ImageEditMode::load(const std::string& fileName)
{
	std::ifstream is(fileName, std::ios::binary);

	image->load(is);
	setModified(false);
}

void ImageEditMode::createNew()
{
	image->reset();
	EditMode::createNew();
}



void MegaMapEditMode::save(const std::string& fileName)
{
	/*std::ofstream os(fileName, std::ios::trunc | std::ios::binary);

	image->save(os);
	setModified(false);*/
}

void MegaMapEditMode::load(const std::string& fileName)
{
	/*std::ifstream is(fileName, std::ios::binary);

	image->load(is);
	setModified(false);*/
}

void MegaMapEditMode::createNew()
{
	megaMap.getMapRoomLayout().clear();
	EditMode::createNew();
}