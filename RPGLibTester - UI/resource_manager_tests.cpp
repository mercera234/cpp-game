#include "resource_manager_tests.h"
#include "ResourceManager.h"


void loadMapsFromTextFile()
{
	ResourceManager rm;

	std::ifstream is;
	is.open("data\\maps.txt");

	int count = rm.loadMapsFromTextFile(is);

	std::cout << "Total: " << count << std::endl;
	system("pause");
}

void loadActorsFromTextFile()
{
	ResourceManager rm;

	std::ifstream is;
	is.open("data\\actors.txt");

	int count = rm.loadActorsFromTextFile(is);

	std::cout << "Total: " << count << std::endl;
	system("pause");
}

void loadItemsFromTextFile()
{
	ResourceManager rm;

	std::ifstream is;
	is.open("data\\items.txt");

	int count = rm.loadItemsFromTextFile(is);

	std::cout << "Total: " << count << std::endl;
	system("pause");
}

void loadConfigurationTextFile()
{
	ResourceManager rm;

	std::ifstream is;
	is.open("data\\configuration.ini");

	int count = rm.loadConfigurationFile(is);

	std::cout << "Total: " << count << std::endl;
	system("pause");
}