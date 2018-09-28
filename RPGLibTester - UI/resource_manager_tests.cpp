#include "resource_manager_tests.h"
#include "ResourceManager.h"
#include "data_loaders.h"

void loadMapsFromTextFile()
{
	ResourceManager rm;

	std::ifstream is;
	is.open("data\\maps.txt");

	int count = loadMapsFromTextFile(is, rm);

	std::cout << "Total: " << count << std::endl;
	system("pause");
}

void loadActorsFromTextFile()
{
	/*ResourceManager rm;

	std::ifstream is;
	is.open("data\\actors.txt");

	int count = loadActorsFromTextFile(is, rm);

	std::cout << "Total: " << count << std::endl;
	system("pause");*/
}

void loadItemsFromTextFile()
{
	/*ResourceManager rm;

	std::ifstream is;
	is.open("data\\items.txt");

	int count = loadItemsFromTextFile(is, rm);

	std::cout << "Total: " << count << std::endl;
	system("pause");*/
}

void loadConfigurationTextFile()
{
	ResourceManager rm;

	std::ifstream is;
	is.open("data\\configuration.ini");

	int count = loadConfigurationFile(is, rm);

	std::cout << "Total: " << count << std::endl;
	system("pause");
}