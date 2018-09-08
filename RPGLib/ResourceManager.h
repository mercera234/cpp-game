#pragma once
#include <map>
#include <functional>
#include "Actor.h"
#include "MapRoom.h"
#include "FileDirectory.h"
#include "GameData.h"

//null resources
const int nullId = -1;
const chtype nullSymbol = 'X' | COLOR_RED_BOLD << TEXTCOLOR_OFFSET;
const std::string nullName = "-";

class ResourceManager
{
private:
	int getColor(char c);

	/*Used to assign a new id to a thing. Once assigned, nextId is incremented. */
	int nextId = 0; 

	int getNextId() { return nextId++; }

	/*Replace underscores with spaces.
	Strings have to be stored in text file with underscores or else they will not be parsed correctly.*/
	void fixString(std::string& s);
public:
	std::map<std::string, Actor, std::function<bool(std::string, std::string)> > actors;
	std::map<int, MapRoom> gameMaps;


	GameData theData;

	std::vector<Actor*> playerParty;


	ResourceManager();
	
	void resetNextId() { nextId = 0; }

	/*Load one resource of each type to be used in the absence of other needed resources. 
	These resources are generated by the program and do not live on the hard drive.*/
	void loadNullResources();

	/*Load Actors from text file. Returns the count of Actors loaded.*/
	int loadActorsFromTextFile(std::ifstream& textFile);

	/*Load Maps from separate data files in one directory. Returns the count of Maps loaded.*/
	int loadGameMapsFromDir(FileDirectory& dir);

	/*Load maps both from text file and data files. 
	The location of the map image files is included in the textfile.*/
	int loadMapRoomsFromTextFile(std::ifstream& textFile);

};
