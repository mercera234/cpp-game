#include "CppUnitTest.h"
#include "Highlighter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(HighlighterTest)
	{
		Image img;
		ITwoDStorage<chtype>* tileMap;
		int rows = 2;
		int cols = 3;

		TEST_METHOD_INITIALIZE(startTUI)
		{
			img.setDimensions(rows, cols);
			tileMap = &img.getTileMap();

			chtype c = 'A';
			for (int i = 0; i < tileMap->getSize(); i++)
			{
				tileMap->setDatum(i, c++);
			}

			//resulting tilemap should look like
			/*
			A B C
			D E F
			*/

		}

		TEST_METHOD_CLEANUP(stopTUI)
		{
			
		}

		TEST_METHOD(setHighlightingTest)
		{
			int y = 1;
			int x = 2;
			Highlighter hl(&img, &y, &x);
			hl.setHighlighting(true);

			Assert::IsTrue(hl.isHighlighting());
		}

		TEST_METHOD(fillTest)
		{
			int y = 0;
			int x = 0;
			Highlighter hl(&img, &y, &x);
			hl.setHighlighting(true);

			y++;
			x++;

			chtype fillChar = '*';
			hl.fill(fillChar);
			Assert::IsTrue(fillChar == tileMap->getDatum(0));
		}


		TEST_METHOD(fillOutOfBoundsTest)
		{
			int y = 0;
			int x = 1;
			Highlighter hl(&img, &y, &x);
			hl.setHighlighting(true);

			x += 10;//way out of bounds to the right

			chtype fillChar = '*';
			hl.fill(fillChar); //this should throw an error if out of bounds isn't handled correctly
			Assert::IsTrue(fillChar == tileMap->getDatum(0,2));
			//Assert::
		}


		TEST_METHOD(eraseTest)
		{
			int y = 0;
			int x = 1;
			Highlighter hl(&img, &y, &x);
			hl.setHighlighting(true);

			y++;

			hl.erase();
			Assert::IsTrue(' ' == tileMap->getDatum(1,1));
		}

		TEST_METHOD(copyPasteTest)
		{
			int y = 0;
			int x = 0;
			Highlighter hl(&img, &y, &x);
			hl.setHighlighting(true);

			x++;
			hl.copy();
			
			hl.setHighlighting(false); //highlighting is turned off before paste

			y = 1;
			hl.paste();
			/*
			Tilemap should now look like
			A B C
			D A B
			*/

			Assert::IsTrue('A' == tileMap->getDatum(1, 1));
			Assert::IsTrue('B' == tileMap->getDatum(1, 2));
		}

		TEST_METHOD(flipTest)
		{
			int y = 0;
			int x = 0;
			Highlighter hl(&img, &y, &x);
			hl.setHighlighting(true);

			y++;

			hl.flip(AXIS_VER);
			/*
			Tilemap should now look like
			D B C
			A E F
			*/
			Assert::IsTrue('D' == tileMap->getDatum(0, 0));
			Assert::IsTrue('A' == tileMap->getDatum(1, 0));
		}
	};
}