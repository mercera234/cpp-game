#include "CppUnitTest.h"
#include "MegaMap.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(MegaMapTest)
	{
		MegaMap map;

		TEST_METHOD(getRealHeightTest)
		{
			Image img;
			int rows = 3;
			int unitHeight = 13;
			img.setDimensions(rows, 10);
			map.setUnitHeight(unitHeight);
			map.setMapRoomLayout(img);

			Assert::AreEqual(rows * unitHeight, map.getRealHeight());
		}

		TEST_METHOD(getRealWidthTest)
		{
			Image img;
			int cols = 11;
			int unitWidth = 13;
			img.setDimensions(5, cols);
			map.setUnitWidth(unitWidth);
			map.setMapRoomLayout(img);

			Assert::AreEqual(cols * unitWidth, map.getRealWidth());
		}

		TEST_METHOD(getUnitPosTest)
		{
			Image img;
			img.setDimensions(4, 4);
			map.setUnitWidth(20);
			map.setUnitHeight(12);
			map.setMapRoomLayout(img);

			int y = 30;
			int x = 45;

			map.setCursor(&y, &x);

			Pos unit = map.getUnitPos();
			Assert::AreEqual(2, unit.y);
			Assert::AreEqual(2, unit.x);
		}

		TEST_METHOD(getCurrMapRoomIdTest)
		{
			Image img;
			img.setDimensions(1, 1);
			int mapId = 57;
			img.setTile(0, 0, mapId);
			map.setUnitWidth(20);
			map.setUnitHeight(12);
			map.setMapRoomLayout(img);

			int y = 0;
			int x = 0;

			map.setCursor(&y, &x);
			
			Assert::AreEqual(mapId, map.getCurrMapRoomId());
		}

		TEST_METHOD(getMapRoomPosTest)
		{
			Image img;
			img.setDimensions(2, 2);
			int mapId = 91;
			img.setTile(0, 1, mapId); //map 91 is in the second column of the 4x4 map
			img.setTile(1, 1, mapId);

			int unitWidth = 5;
			map.setUnitWidth(unitWidth);
			map.setUnitHeight(2);
			map.setMapRoomLayout(img);

			int y = 3;
			int x = 7;

			map.setCursor(&y, &x);

			Pos roomPos = map.getMapRoomPos();
			Assert::AreEqual(y, roomPos.y);
			Assert::AreEqual(x - unitWidth, roomPos.x);
		}



	};
}