#pragma once
#include <iostream>

enum class LumpType
{
	UNKNOWN,
	MAP,
	ACTOR,
	IMAGE,
	ITEM
};


const std::string actorFileExt = "actr";
const std::string mapFileExt = "map";

struct Lump
{
	//wad data
//	LumpType type;
	std::streampos fileOffset;
	Storable* object;

	//file data
	int size;
	std::string sourceFileName;

	Lump() {};
	
	static std::string getFilterFromType(LumpType type)
	{
		std::string filter;

		switch (type)
		{
		case LumpType::ACTOR: filter = actorFileExt; break;
		case LumpType::MAP: filter = mapFileExt; break;
		}
		return filter;
	}

	static std::string getNameFromType(LumpType type)
	{
		std::string name;

		switch (type)
		{
		case LumpType::ACTOR: name = "Actor"; break;
		case LumpType::MAP: name = "Map"; break;
		}
		return name;
	}
};