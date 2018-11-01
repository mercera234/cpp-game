#include "game_data_tests.h"
#include "GameData.h"


void jsonTest()
{
	GameData data;
	data.loadDataFile("data\\jsontest.txt");
	system("pause");
}


