#include "json_tree_creators.h"
#include <list>


void createActorJSONtree(boost::property_tree::ptree& actorsNode)
{
	boost::property_tree::ptree actorNode;
	actorNode.put("name", "BigBug");
	actorNode.put("symbol", "B");
	actorNode.put("color", "White");
	actorNode.put("level", 1);
	actorNode.put("exp", 2);
	actorNode.put("money", 2);
	actorNode.put("hp", 7);
	actorNode.put("mp", 0);
	actorNode.put("strength", 2);
	actorNode.put("defense", 0);
	actorNode.put("intelligence", 0);
	actorNode.put("will", 0);
	actorNode.put("agility", 0);

	boost::property_tree::ptree actorNode2;
	actorNode2.put("name", "Hero");
	actorNode2.put("symbol", "@");
	actorNode2.put("color", "White");
	actorNode2.put("level", 1);
	actorNode2.put("exp", 2);
	actorNode2.put("money", 2);
	actorNode2.put("hp", 7);
	actorNode2.put("mp", 0);
	actorNode2.put("strength", 2);
	actorNode2.put("defense", 0);
	actorNode2.put("intelligence", 0);
	actorNode2.put("will", 0);
	actorNode2.put("agility", 0);

	actorsNode.push_back(std::make_pair("", actorNode));
	actorsNode.push_back(std::make_pair("", actorNode2));
}

void createItemJSONtree(boost::property_tree::ptree& itemsNode)
{
	boost::property_tree::ptree itemNode;
	itemNode.put("name", "Megalixir");
	itemNode.put("cost", 100000);
	itemNode.put("targetset", "side single");
	itemNode.put("targetside", "self");
	itemNode.put("description", "Restores all HP and MP");

	boost::property_tree::ptree statsNode;

	boost::property_tree::ptree hpNode;
	hpNode.put("stat", "hp");
	hpNode.put("value", 9999);

	boost::property_tree::ptree mpNode;
	mpNode.put("stat", "mp");
	mpNode.put("value", 999);

	statsNode.push_back(std::make_pair("", hpNode));
	statsNode.push_back(std::make_pair("", mpNode));

	itemNode.add_child("stats", statsNode);
	
	boost::property_tree::ptree itemNode2;
	itemNode2.put("name", "Potion");
	itemNode2.put("cost", 10);
	itemNode2.put("targetset", "single");
	itemNode2.put("targetside", "any");
	itemNode2.put("stat", "hp");
	itemNode2.put("value", 25);
	itemNode2.put("description", "Restores 25 HP");

	boost::property_tree::ptree usableNode;
	usableNode.push_back(std::make_pair("", itemNode));
	usableNode.push_back(std::make_pair("", itemNode2));

	boost::property_tree::ptree equipNode;

	equipNode.put("name", "Knife");
	equipNode.put("cost", 20);
	equipNode.put("part", "weapon");
	equipNode.put("description", "for stabbing...");
	equipNode.put("stat", "strength");
	equipNode.put("value", 25);

	boost::property_tree::ptree equipmentNode;
	equipmentNode.push_back(std::make_pair("", equipNode));

	itemsNode.push_back(std::make_pair("usable", usableNode));
	itemsNode.push_back(std::make_pair("equipment", equipmentNode));
	
}

void createMapRoomJSONtree(boost::property_tree::ptree& roomsNode)
{
	boost::property_tree::ptree roomNode;
	roomNode.put("name", "Crypt");
	roomNode.put("brightness", false);

	boost::property_tree::ptree roomNode2;
	roomNode2.put("name", "Island");
	roomNode2.put("random encounters", false);


	boost::property_tree::ptree roomNode3;
	roomNode3.put("name", "Labyrinth");

	boost::property_tree::ptree thingNode;
	thingNode.put("name", "Gold$");
	thingNode.put("quantity", 3);
	thingNode.put("y", 13);
	thingNode.put("x", 33);

	boost::property_tree::ptree thingsNode;
	thingsNode.push_back(std::make_pair("", thingNode));

	roomNode3.add_child("things", thingsNode);


	roomsNode.push_back(std::make_pair("", roomNode));
	roomsNode.push_back(std::make_pair("", roomNode2));
	roomsNode.push_back(std::make_pair("", roomNode3));
}

void createEnemyPoolJSONtree(boost::property_tree::ptree& poolsNode)
{
	std::list<std::string> roomNames = { "Labyrinth", "Labyrinth2", "Labyrinth3" };

	boost::property_tree::ptree roomsNode;
	for each (auto roomName in roomNames)
	{
		boost::property_tree::ptree roomNode;
		roomNode.put("", roomName);
		roomsNode.push_back(std::make_pair("", roomNode));
	}

	boost::property_tree::ptree enemiesNode;
	std::list<std::string> enemyNames = { "Toadie", "Toadie" };

	for each (auto enemyName in enemyNames)
	{
		boost::property_tree::ptree enemyNode;
		enemyNode.put("", enemyName);
		enemiesNode.push_back(std::make_pair("", enemyNode));
	}

	boost::property_tree::ptree groupNode;
	groupNode.put("weight", 1);
	groupNode.push_back(std::make_pair("enemies", enemiesNode));

	boost::property_tree::ptree groupsNode;
	groupsNode.push_back(std::make_pair("", groupNode));

	boost::property_tree::ptree poolNode;
	poolNode.push_back(std::make_pair("rooms", roomsNode));
	poolNode.push_back(std::make_pair("groups", groupsNode));
	poolsNode.push_back(std::make_pair("", poolNode));
}

void createMapJSONtree(boost::property_tree::ptree& mapsNode)
{
	boost::property_tree::ptree mapNode;

	mapNode.put("name", "TestRegion");
	mapNode.put("rows", 2);
	mapNode.put("cols", 3);
	mapNode.put("floors", 2);
	mapNode.put("ground floor", 1);
	int mapIds[] = {
		3, 3, -1,
		2, 1, -1,
		4, 4, -1,
		4, 4, -1
	};

	boost::property_tree::ptree idsNode;
	for each (int id in mapIds)
	{
		boost::property_tree::ptree idNode;
		idNode.put("", id);
		idsNode.push_back(std::make_pair("", idNode));
	}
	mapNode.push_back(std::make_pair("data", idsNode));

	boost::property_tree::ptree crNode;
	crNode.put("name", "Labyrinth");
	crNode.put("index", 1);

	boost::property_tree::ptree crNode2;
	crNode2.put("name", "Labyrinth2");
	crNode2.put("index", 2);

	boost::property_tree::ptree crNode3;
	crNode3.put("name", "Labyrinth3");
	crNode3.put("index", 3);

	boost::property_tree::ptree crossRefsNode;
	crossRefsNode.push_back(std::make_pair("", crNode));
	crossRefsNode.push_back(std::make_pair("", crNode2));
	crossRefsNode.push_back(std::make_pair("", crNode3));
	mapNode.push_back(std::make_pair("crossRef", crossRefsNode));

	mapsNode.push_back(std::make_pair("", mapNode));
}
