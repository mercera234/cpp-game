#include "EnemyPool.h"
#include <numeric>
#include <time.h>

EnemyPool::EnemyPool()
{
	srand((unsigned int)time(nullptr));
}


EnemyPool::~EnemyPool()
{
}


EnemyGroup EnemyPool::getRandomGroup()
{
	//calculate total weight
	int totalWeight = std::accumulate(groups.begin(), groups.end(), 0, 
		[](int accumulator, EnemyGroup& group)
		{
			return accumulator += group.weight;
		}
		);

	int* arr = new int[totalWeight];

	
	int index = 0;
	int vectorIndex = 0;
	int prevWeight = 0;
	for each (EnemyGroup group in groups)
	{
		int currWeight = group.weight;

		for (int i = 0; i < currWeight; i++)
		{
			arr[i + prevWeight] = vectorIndex;
		}
		vectorIndex++;
		prevWeight += currWeight;
	}


	//roll dice 
	int r = rand() % totalWeight;

	int groupChoice = arr[r];

	return groups[groupChoice];
}