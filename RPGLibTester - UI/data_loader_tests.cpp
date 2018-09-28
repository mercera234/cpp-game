#include "data_loader_tests.h"
#include "data_loaders.h"

void jsonTest()
{
	ResourceManager rm;
	loadDataFile("data\\jsontest.txt", rm);
	system("pause");

}