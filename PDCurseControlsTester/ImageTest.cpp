#include "CppUnitTest.h"
#include "Image.h"
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(ImageTest)
	{
		TEST_METHOD(constructorTest)
		{
			Image img;
			Assert::IsTrue(img.isBordered());
		}

		TEST_METHOD(drawWithoutWinTest)
		{
			Image img;

			bool exceptionOccurred = false;
			try
			{
				img.draw();
			}
			catch (std::exception e)//const char* msg)
			{
				exceptionOccurred = true;
			}
			
			Assert::IsFalse(exceptionOccurred);
		}


		TEST_METHOD(setDimensionsSimpleTest)
		{
			Image img;
			int rows = 1;
			int cols = 1;
			img.setDimensions(rows, cols);
			Assert::AreEqual(rows * cols, (int)img.getTotalTiles());
		}

		TEST_METHOD(setDimensionsNormalTest)
		{
			Image img;
			int rows = 10;
			int cols = 30;
			img.setDimensions(rows, cols);
			Assert::AreEqual(rows * cols, (int)img.getTotalTiles());
		}

		TEST_METHOD(resetTest)
		{
			Image img;
			int rows = 1;
			int cols = 2;
			img.setDimensions(rows, cols);

			TwoDStorage<chtype>* map = img.getTileMap();
			map->setDatum(0, 1, 'A');

			img.reset(); //will clear the A

			Assert::IsTrue(' ' == map->getDatum(0,1));
		}

		TEST_METHOD(setTileTest)
		{
			Image img;
			int rows = 1;
			int cols = 2;
			img.setDimensions(rows, cols);

			chtype c = 'A';
			img.setTile(0, 1, c);

			Assert::IsTrue(c == img.getTile(0, 1));
		}
	};
}