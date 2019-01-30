#include "game_data_tests.h"
#include "GameData.h"


void jsonTest()
{
	GameData data;
	data.loadDataFile("data\\json_test.txt");
	system("pause");
}


