#include "CppUnitTest.h"
#include "OwnedItemRecord.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(OwnedItemRecordTest)
	{
		TEST_METHOD(ctorTest)
		{
			GameItem item;
			item.name = "Potion";

			Possession possession;
			possession.item = &item;

			OwnedItemRecord record(&possession, 0);

			Assert::AreEqual((int)&possession, (int)record.getPossession());
		}

		TEST_METHOD(setPossessionTest)
		{
			GameItem item;
			item.name = "Potion";

			Possession possession;
			possession.item = &item;

			OwnedItemRecord record;
			record.setPossession(&possession);
			record.index = 0;

			Assert::AreEqual((int)&possession, (int)record.getPossession());
		}
	};
}