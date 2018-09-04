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
	};
}