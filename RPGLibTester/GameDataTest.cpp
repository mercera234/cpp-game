#include "CppUnitTest.h"
#include "GameData.h"
#include <limits>
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(GameDataTest)
	{
		GameData data;
		TEST_METHOD(storeIntTest)
		{
			BoundInt someInt(0, 100, 36);
			Assert::IsTrue(data.storeIntData("Gold", someInt));
		}

		TEST_METHOD(retrieveUnstoredInt)
		{
			bool exceptionCaught = false;
			try
			{
				BoundInt& x = data.retrieveIntData("anything");
			}
			catch (std::exception e)
			{
				exceptionCaught = true;
			}
			Assert::IsTrue(exceptionCaught);
		}

		TEST_METHOD(storeRetrieveIntTest)
		{
			BoundInt value(0, INT_MAX, 34578665);
			std::string key = "Val";
			data.storeIntData(key, value);

			BoundInt& getVal = data.retrieveIntData(key);
			
			Assert::AreEqual(value.getCurr(), getVal.getCurr());
		}

		TEST_METHOD(cantStoreDuplicateKeysTest)
		{
			BoundInt someInt(0, 100, 36);
			std::string key = "Value";
			data.storeIntData(key, someInt);

			BoundInt anotherInt(3, 36, 20);
			data.storeIntData(key, anotherInt); //repeat the same store

			Assert::AreEqual(1, (int)data.bIntData.size());
		}

		TEST_METHOD(overwriteDataCheckTest)
		{
			int firstVal = 36;
			BoundInt someInt(0, 100, firstVal);
			std::string key = "Value";
			data.storeIntData(key, someInt);

			int secondVal = 20;
			BoundInt anotherInt(3, 36, secondVal);
			data.storeIntData(key, anotherInt); //repeat the same store

			BoundInt& retInt = data.retrieveIntData(key);

			Assert::AreEqual(firstVal, retInt.getCurr()); //the first value was never overwritten
		}

		TEST_METHOD(clearTest)
		{
			int firstVal = 36;
			BoundInt someInt(0, 100, firstVal);
			std::string key = "Value";
			data.storeIntData(key, someInt);

			data.clearData();

			int secondVal = 20;
			BoundInt anotherInt(3, 36, secondVal);
			data.storeIntData(key, anotherInt); //repeat the same store

			BoundInt& retInt = data.retrieveIntData(key);

			Assert::AreEqual(secondVal, retInt.getCurr()); //the first value was never overwritten
		}
	};
}