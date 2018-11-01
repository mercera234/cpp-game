#pragma once
#include "ResourceManager.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

/*Load Actors from text file. Returns the count of Actors loaded.*/
//int loadActorsFromTextFile(std::ifstream& textFile, ResourceManager& rm);

/*Load maps both from text file and data files.
The location of the map image files is included in the textfile.*/
//int loadMapRoomsFromTextFile(std::ifstream& textFile, ResourceManager& rm);

/*Load Maps from text file. Returns the count of Maps loaded.*/
//int loadMapsFromTextFile(std::ifstream& textFile, ResourceManager& rm);

/*Load items from text file. Returns the count of Items loaded.*/
//int loadItemsFromTextFile(std::ifstream& textFile, ResourceManager& rm);

/*Load entries from configuration file.
The configuration file is human readable and should be modifiable in-game and on the file system.*/
int loadConfigurationFile(std::ifstream& textFile, ResourceManager& rm);

void loadDataFile(const std::string& jsonFile, ResourceManager& rm);

void loadItems(boost::property_tree::ptree& tree, ResourceManager& rm);
void loadActors(boost::property_tree::ptree& tree, ResourceManager& rm);
void loadRooms(boost::property_tree::ptree& tree, ResourceManager& rm);
void loadEnemyPools(boost::property_tree::ptree& tree, ResourceManager& rm);
void loadMaps(boost::property_tree::ptree& tree, ResourceManager& rm);
void loadEverything(boost::property_tree::ptree& jsonFile, ResourceManager& rm);