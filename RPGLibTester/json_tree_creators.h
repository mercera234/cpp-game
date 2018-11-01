#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

/*Methods for creating JSON objects similar to what is to be read in from a file.*/


void createActorJSONtree(boost::property_tree::ptree& actorsNode);
void createItemJSONtree(boost::property_tree::ptree& itemsNode);
void createMapRoomJSONtree(boost::property_tree::ptree& roomsNode);
void createEnemyPoolJSONtree(boost::property_tree::ptree& poolsNode);
void createMapJSONtree(boost::property_tree::ptree& mapsNode);