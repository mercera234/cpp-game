#include <limits>
#include <iostream>
#include <iomanip>
//#include <dirent.h>
#include <string>
#include "WadDatabase.h"
#include "DataPkg.h"
#include "KeyComparators.h"
//#include "DataPkgFactory.h"


WadDatabase::WadDatabase() : actors(stringCompare)
{
	totalLumpSize = 0;
}

/*
reads data in from wad and prints it out
*/
void WadDatabase::print()
{
	std::cout << "---Header---" << std::endl;
	std::string tag(header.tag, 4);
	std::cout << tag << ' ' << "Dir offset: " << std::hex << std::showbase << header.dirOffset << std::endl;

	std::cout << "---Directory---" << std::endl;

	std::list<WadDirEntry*>::iterator entryIt = directory.begin();
	std::list<Lump*>::iterator lumpIt = lumps.begin();
	
	//undo formatting from previous
	std::cout << std::dec << std::noshowbase;

	for (unsigned int i = 0; i < directory.size(); i++)
	{
		WadDirEntry* entry = *entryIt;
		Lump* lump = *lumpIt;

		//output offset of entry
		std::cout << "0x" << std::setfill('0') << std::setw(8) << std::right << std::hex << entry->offset << ' ';
		std::cout << std::setw(8) << std::setfill(' ') << std::left;

		//output type of entry
		std::string lumpName = Lump::getNameFromType(entry->type);
		std::cout << lumpName;

		//output entry name
		std::cout << ' ' << std::setw(8) << std::setfill(' ') << std::left << std::dec << entry->lumpSize << ' ' << lump->sourceFileName << std::endl;;

		entryIt++; lumpIt++;
	}
}


/*
Populates the database object (header, lumps, dirEntries) in memory
using all game data files foundin 'dataDirName'
*/
void WadDatabase::buildWad(std::string& dataDir)
{
	//load every data file into lump list
	FileDirectory dir(dataDir);

	cursorOffset = sizeof(WadHeader);
	compileLumps(dir, LumpType::ACTOR);
	compileLumps(dir, LumpType::MAP);

	//finish populating header
	memcpy_s(header.tag, 8, "IWAD", 4);
	header.dirOffset = sizeof(WadHeader) + totalLumpSize;
	header.lumpCount = lumps.size();
}



void WadDatabase::compileLumps(FileDirectory& dir, LumpType type)
{
	std::string filter = Lump::getFilterFromType(type);
	std::list<dirent> files = dir.getFiles(false, filter);
	
	
	for each (dirent file in files)
	{
		//save lump
		Lump* lump = new Lump();
		
		lump->sourceFileName = dir.getPath() + '\\' + file.d_name;
		int fileSize = (int)dir.getSize(lump->sourceFileName);
		
		//calculate size in datapkg blocks (this may be actor temporary fix)
		int fullBlocks = fileSize / BLOCK_SIZE;
		int partialBlocks = fileSize % BLOCK_SIZE > 0 ? 1 : 0;
		int fileBlocks = fullBlocks + partialBlocks;
		lump->size = fileBlocks * BLOCK_SIZE; //then get actual size

		totalLumpSize += lump->size;
		lumps.push_back(lump);

		//create dir entry
		WadDirEntry* dirEntry = new WadDirEntry();
		dirEntry->type = type;
		dirEntry->lumpSize = lump->size;
		lump->fileOffset = cursorOffset;
		dirEntry->offset = (int)lump->fileOffset;
		cursorOffset += lump->size;
		directory.push_back(dirEntry);
	}

	header.lumpCount = lumps.size();
}


//write header, lumps, directory to single wad data file
bool WadDatabase::save(std::ofstream* saveFile)
{
	//write header
	saveFile->write((char*)&header, sizeof(WadHeader));
	
	std::ifstream is; //stream to read data in from individual lumps
	//write all lumps
	for each (Lump* lump in lumps)
	{
		is.open(lump->sourceFileName);

		DataPkg pkg;
		pkg.load(&is);

		pkg.save(saveFile);
		is.close();
	}

	//write directory
	for each (WadDirEntry* entry in directory)
	{
		saveFile->write((char*)entry, sizeof(WadDirEntry));
	}
	
	
	return true;
}

/*
Load single wad data file into memory lumps in database
*/
bool WadDatabase::load(std::ifstream* loadFile)
{
	//check that file has data
	loadFile->seekg(std::ios::end);
	std::streampos size = loadFile->tellg();

	if (size <= 0)
		return false;

	//return cursor to head
	loadFile->seekg(std::ios::beg);

	loadFile->read((char*)&header, sizeof(WadHeader));
	
	if (strncmp(header.tag, "IWAD", 4) != 0) //verify that this is actor wad file
		return false;

	//dispose of previous lumps/dir entries if present
	lumps.clear();
	directory.clear();

	//read in entire directory
	loadFile->seekg(header.dirOffset);

	for (int i = 0; i < header.lumpCount; i++)
	{
		WadDirEntry* entry = new WadDirEntry();
		loadFile->read((char*)entry, sizeof(WadDirEntry));
		directory.push_back(entry);
	}

	for each (WadDirEntry* entry in directory)
	{
		Lump* lump = new Lump();
		lump->fileOffset = entry->offset;
		lump->size = entry->lumpSize;

		lumps.push_back(lump);
	}

	return true;
}

//void WadDatabase::getActorRepository(std::map<std::string, ActorDef*, std::function<bool(std::string, std::string)>>& mapRepo, std::ifstream& wadFile)
//{
//	//iterate through directory for all actor types
//	for each (WadDirEntry* entry in directory)
//	{
//		if (entry->type != LumpType::ACTOR)
//			continue;
//
//		wadFile.seekg(entry->offset);
//
//		int blocks = entry->lumpSize / BLOCK_SIZE;
//		DataPkg pkg;
//		pkg.load(&wadFile, blocks);
//
//		DataPkgFactory factory;
//		ActorDef* def = factory.getActorDefFromPkg(&pkg);
//
//		std::pair<std::string, ActorDef*> defPair(def->name, def);
//		//pair<std::string, ActorDef*> defPair(aStr, def);
//		mapRepo.insert(defPair);
//	}
//}

void WadDatabase::getMapRepository(std::map<unsigned short, MapRoom*>& mapRepo, std::ifstream& wadFile)
{
	//iterate through directory for all actor types
	for each (WadDirEntry* entry in directory)
	{
		if (entry->type != LumpType::MAP)
			continue;

		wadFile.seekg(entry->offset);

		int blocks = entry->lumpSize / BLOCK_SIZE;
		DataPkg pkg;
		pkg.load(&wadFile, blocks);

	//	DataPkgFactory factory;
		
	//	ActorDef* def = factory.getActorDefFromPkg(&pkg);

		//std::pair<unsigned short, Map*> aMap();//def->name, def);
		////pair<std::string, ActorDef*> defPair(aStr, def);
		//mapRepo.insert(aMap);
	}
}

void WadDatabase::loadActorsFromTextFile(std::ifstream& textFile)
{
	//char lineFirstChar;
	//while ((lineFirstChar = (char)textFile.peek()) != EOF)
	//{
	//	if (lineFirstChar == '#') //skip to next line
	//	{
	//		//see max conflict with windef.h, hence the extra ()s
	//		textFile.ignore((std::numeric_limits<int>::max)(), '\n');
	//		continue;
	//	}

	//	Actor actor;

	//	char symbol;
	//	int level, exp, money, hp, mp, str, def, intl, wil, agi;

	//	textFile >> actor.name >> symbol >> level >> exp >> money >> hp >> mp >> str >> def >> intl >> wil >> agi;

	//	std::replace(actor.name.begin(), actor.name.end(), '_', ' '); //This handles any multi-word names

	//	actor.symbol = symbol;
	//	actor.getStat(StatType::LEVEL).setCurr(level);
	//	actor.getStat(StatType::EXP).setCurr(exp);
	//	actor.money.setCurr(money);

	//	actor.getStat(StatType::HP).setCurrMax(hp);
	//	actor.getStat(StatType::HP).maxOut();
	//	actor.getStat(StatType::MP).setCurrMax(mp);
	//	actor.getStat(StatType::MP).maxOut();

	//	actor.getStat(StatType::STRENGTH).setCurr(str);
	//	actor.getStat(StatType::DEFENSE).setCurr(def);
	//	actor.getStat(StatType::INTELLIGENCE).setCurr(intl);
	//	actor.getStat(StatType::WILL).setCurr(wil);
	//	actor.getStat(StatType::AGILITY).setCurr(agi);

	//	actors.insert(std::make_pair(actor.name, actor));
	//}

}

void WadDatabase::loadGameMapsFromDir(FileDirectory& dir)
{
	//get list of map files
	//std::string filter = "map";
	//std::list<dirent> files = dir.getFiles(false, filter);

	//int id = 0;
	//for each (dirent file in files)
	//{
	//	MapRoom gameMap;
	//	gameMap.load(dir.getPath() + '\\' + file.d_name);

	//	gameMaps.insert(std::make_pair(id++, gameMap)); //id is set automatically not manually
	//}
}