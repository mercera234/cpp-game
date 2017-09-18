#include "stdafx.h"
#include "CppUnitTest.h"
#include <algorithm>
#include "TwoDStorage.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{		
	TEST_CLASS(TwoDStorageTest)
	{
	public:
		TEST_METHOD(setDatumWithElementTest)
		{
			TwoDStorage<int> displayData(5, 3); 

			int element = 0;
			displayData.setDatum(element, 8);
			
			Assert::AreEqual(8, displayData.getDatum(element));
		}

		TEST_METHOD(setDatumWithRowColTest)
		{
			TwoDStorage<int> displayData(5, 3);

			int y = 3;
			int x = 1;
			displayData.setDatum(y, x, 8);

			Assert::AreEqual(8, displayData.getDatum(y,x));
		}

		TEST_METHOD(setDatumWithObjectTest)
		{
			TwoDStorage<std::string> testData(5, 3);

			int element = 0;
			testData.setDatum(element, "word");

			int result = testData.getDatum(element).compare("word");

			Assert::AreEqual(0, result);
		}

		TEST_METHOD(fillTest)
		{
			TwoDStorage<int> displayData(5, 3);
			int fillDatum = 33;
			displayData.fill(fillDatum);

			Assert::AreEqual(fillDatum, displayData.getDatum(14));
		}

		TEST_METHOD(copyTest)
		{
			TwoDStorage<int> displayData(5, 3);
			
			for (unsigned int i = 0; i < displayData.getSize(); i++)
			{
				displayData.setDatum(i, i); //set datum equal to element
			}

			TwoDStorage<int> newData(7, 2);

			newData.copyFrom(displayData);

			//we assert that the last shared element in each object matches in value
			int longestWidth = std::max(displayData.getCols(), newData.getCols());
			int shortWidth = std::min(displayData.getCols(), newData.getCols());
			int longestHeight = std::max(displayData.getRows(), newData.getRows());
			int shortHeight = std::min(displayData.getRows(), newData.getRows());
			
			int lastSharedY = shortHeight - 1;
			int lastSharedX = shortWidth - 1;
			Assert::AreEqual(displayData.getDatum(lastSharedY, lastSharedX), newData.getDatum(lastSharedY, lastSharedX));
		}

		TEST_METHOD(getDataTest)
		{
			TwoDStorage<int> displayData(2, 2);
			int fillDatum = 7;
			displayData.fill(fillDatum);

			std::vector<int>& secondObj = displayData.getData();
			secondObj[0] = 4568;

			Assert::AreEqual(4568, displayData.getDatum(0));
		}

		TEST_METHOD(setDimensionsTest)
		{
			TwoDStorage<int> displayData(3, 4);

			displayData.setDimensions(1, 2);
			Assert::AreEqual(2, (int)displayData.getSize());
		}
	};
}