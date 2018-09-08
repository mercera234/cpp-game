#include "CppUnitTest.h"
#include "EnemyPool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(EnemyPoolTest)
	{
		EnemyPool pool;

		TEST_METHOD(setPoolTest)
		{
			std::vector<EnemyGroup>& config = pool.getGroups();

			EnemyGroup ec1;
			ec1.weight = 1;
			ec1.enemyNames.push_back("A");
			ec1.enemyNames.push_back("B");
			ec1.enemyNames.push_back("C");

			config.push_back(ec1);


		}

		TEST_METHOD(getRandomGroupTest)
		{
			std::vector<EnemyGroup>& config = pool.getGroups();

			EnemyGroup ec1, ec2;
			ec1.weight = 1;
			ec1.enemyNames.push_back("A");

			ec2.weight = 1;
			ec2.enemyNames.push_back("B");

			config.push_back(ec1);
			config.push_back(ec2);
			

			EnemyGroup ec = pool.getRandomGroup();

			Assert::IsTrue(ec.enemyNames.front().compare("A") == 0 || ec.enemyNames.front().compare("B") == 0);
		}

		//TEST_METHOD(getRandomGroupDistributionTest)
		//{
		//	std::vector<EnemyGroup>& config = pool.getGroups();

		//	EnemyGroup ec1, ec2, ec3, ec4;
		//	ec1.weight = 1;
		//	ec2.weight = 2;
		//	ec3.weight = 3;
		//	ec4.weight = 4;

		//	config.push_back(ec1);
		//	config.push_back(ec2);
		//	config.push_back(ec3);
		//	config.push_back(ec4);

		//	int histogram[4];
		//	for (int i = 0; i < 4; i++)
		//		histogram[i] = 0;


		//	for (int i = 0; i < 1000; i++)
		//	{
		//		EnemyGroup ec = pool.getRandomGroup();
		//		histogram[ec.weight - 1]++;
		//	}
		//	
		//	Assert::IsTrue(true);//not sure if we can truly test a percentage distribution method
		//}
	};
}