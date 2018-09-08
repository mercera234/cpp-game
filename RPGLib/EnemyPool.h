#pragma once
#include <vector>

struct EnemyGroup
{
	std::vector<std::string> enemyNames;
	int weight; //weighted chance of encounter
};


class EnemyPool
{
	std::vector<EnemyGroup> groups;
public:
	EnemyPool();
	~EnemyPool();


	void setGroups(std::vector<EnemyGroup>& configurationsIn) { groups = configurationsIn; }
	std::vector<EnemyGroup>& getGroups() { return groups; }

	EnemyGroup getRandomGroup();
};

