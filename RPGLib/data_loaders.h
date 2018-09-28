#pragma once
#include "ResourceManager.h"

/*Load Actors from text file. Returns the count of Actors loaded.*/
//int loadActorsFromTextFile(std::ifstream& textFile, ResourceManager& rm);

/*Load maps both from text file and data files.
The location of the map image files is included in the textfile.*/
int loadMapRoomsFromTextFile(std::ifstream& textFile, ResourceManager& rm);

/*Load Maps from text file. Returns the count of Maps loaded.*/
int loadMapsFromTextFile(std::ifstream& textFile, ResourceManager& rm);

/*Load items from text file. Returns the count of Items loaded.*/
int loadItemsFromTextFile(std::ifstream& textFile, ResourceManager& rm);

/*Load entries from configuration file.
The configuration file is human readable and should be modifiable in-game and on the file system.*/
int loadConfigurationFile(std::ifstream& textFile, ResourceManager& rm);

void loadDataFile(const std::string& jsonFile, ResourceManager& rm);

