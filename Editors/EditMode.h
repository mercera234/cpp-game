#pragma once
#include <iostream>
#include "MapRoom.h"

const std::string DEF_MAP_EXTENSION = ".map";
const std::string DEF_IMAGE_EXTENSION = ".img";
const std::string DEF_FILENAME = "<Untitled>";

class EditMode
{
protected:
	static std::string dialogDefPath;
	static std::string loadedFileName;
	static bool modified;
public:
	std::string extensionFilter;
	std::string modeName;
	
	EditMode();
	~EditMode();
	
	/*Save using the default path and loaded file name*/
	void save();


	virtual void save(const std::string& fileName) = 0;
	virtual void load(const std::string& fileName) = 0;
	virtual void createNew() = 0;

	void resetFileName();
	void setModified(bool modified); //all routines to perform when current mode has been modified from original state
	void storeLastOpened(const std::string& path);

	//getters/setters
	void setFileName(const std::string& fileNameIn) { loadedFileName = fileNameIn; }
	std::string getFileName() { return loadedFileName; }
	std::string getFileNameModified();
	std::string getDefaultFilePath() { return dialogDefPath; }
	void setDefaultFilePath(const std::string& path) { dialogDefPath = path; }
	bool isModified() { return modified; }

	/*Returns true if a save have occurred, which changes the file name from the default*/
	bool hasSaved();
};

class MapRoomEditMode : public EditMode
{
private:
	
public:
	MapRoom mapRoom;
	MapRoomEditMode()
	{
		modeName = "MAP ROOM";
		extensionFilter = DEF_MAP_EXTENSION;
	}

	void save(const std::string& fileName);
	void load(const std::string& fileName);
	void createNew();
};

class ImageEditMode : public EditMode
{
private:
public:
	Image* image;
	ImageEditMode()
	{
		modeName = "IMAGE";
		extensionFilter = DEF_IMAGE_EXTENSION;
	}

	void save(const std::string& fileName);
	void load(const std::string& fileName);
	void createNew();
};