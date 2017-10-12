#pragma once
#include <fstream>
#include <string>
#include <list>
#include <functional>
#include <map>
#include "Storable.h"
#include "FileDirectory.h"
#include "Lump.h"
#include "ActorDefRepository.h"
#include "MapRepository.h"


struct WadHeader
{
	char tag[8];
	int dirOffset;
	int lumpCount;
};

struct WadDirEntry
{
	int offset; //location of lump in wad
	int lumpSize;
	LumpType type;
};


class WadDatabase
{
private:	
	WadHeader header;

	
	int cursorOffset;
	int totalLumpSize; //should be maintained as lumps are created

	//should always be a 1-to-1 ratio of lumps to directory entries
	std::list<Lump*> lumps; 
	std::list<WadDirEntry*> directory;

	void compileLumps(FileDirectory& dir, LumpType type);
	
public:
	WadDatabase();
	void print();
	
	void buildWad(std::string& dataDir); //creates wad database using all data files found in data directory

	bool save(std::ofstream* saveFile);
	bool load(std::ifstream* loadFile);

	
	//not sure if I should have this method here!!!
	void getActorRepository(std::map<std::string, ActorDef*, std::function<bool(std::string, std::string)>>& actorRepo, std::ifstream& wadFile);
	void getMapRepository(std::map<unsigned short, Map*>& mapRepo, std::ifstream& wadFile);
};
