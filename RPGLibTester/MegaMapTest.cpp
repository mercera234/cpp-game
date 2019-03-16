#include "CppUnitTest.h"
#include "MegaMap.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{
	TEST_CLASS(MegaMapTest)
	{
		MegaMap map;

		//test map functions without initialization
		TEST_METHOD(nullMapTest)
		{
			Assert::AreEqual(0, map.getUnitPos().y);
			Assert::AreEqual(0, map.getUnitPos().x);
		}

		TEST_METHOD(getRealHeightTest)
		{
			int rows = 3;
			map.setDimensions(rows, 10);

			Image img;
			int unitHeight = 13;
			img.setDimensions(rows, 10);
			map.setUnitHeight(unitHeight);
			map.setLayerImage(0, img);

			Assert::AreEqual(rows * unitHeight, map.getRealHeight());
		}

		TEST_METHOD(getRealWidthTest)
		{
			Image img;
			int cols = 11;
			map.setDimensions(5, cols);
			
			int unitWidth = 13;
			img.setDimensions(5, cols);
			map.setUnitWidth(unitWidth);
			map.setLayerImage(0, img);

			Assert::AreEqual(cols * unitWidth, map.getRealWidth());
		}

		TEST_METHOD(getUnitPosTest)
		{
			Image img;
			img.setDimensions(4, 4);
			map.setUnitWidth(20);
			map.setUnitHeight(12);
			map.setDimensions(4, 4);
			map.setLayerImage(0, img);

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
			map.setDimensions(1, 1);
			img.setDimensions(1, 1);
			int mapId = -1;
			img.setTile(0, 0, mapId);
			map.setUnitWidth(20);
			map.setUnitHeight(12);
			map.setLayerImage(0, img);

			int y = 0;
			int x = 0;

			map.setCursor(&y, &x);
			
			Assert::AreEqual(mapId, map.getCurrMapRoomId());
		}

		TEST_METHOD(getCurrMapRoomIdZAxisTest)
		{
			map.setDimensions(1, 1, 2);
			Image img, img2;
			
			img.setDimensions(1, 1);
			img2.setDimensions(1, 1);
			img.setTile(0, 0, 8);
			img2.setTile(0, 0, 43);
			map.setUnitWidth(20);
			map.setUnitHeight(12);
			map.setLayerImage(0, img);
			map.setLayerImage(1, img2);

			int y = 0;
			int x = 0;

			map.setCursor(&y, &x);
			map.setFloorIndex(0);
			map.changeLayer(1);

			Assert::AreEqual(43, map.getCurrMapRoomId());
		}

		TEST_METHOD(getMapRoomPosTest)
		{
			Image img;
			map.setDimensions(2, 2);
			img.setDimensions(2, 2);
			int mapId = 91;
			img.setTile(0, 1, mapId); //map 91 is in the second column of the 4x4 map
			img.setTile(1, 1, mapId);

			int unitWidth = 5;
			map.setUnitWidth(unitWidth);
			map.setUnitHeight(2);
			map.setLayerImage(0, img);

			int y = 3;
			int x = 7;

			map.setCursor(&y, &x);

			Pos roomPos = map.getMapRoomPos();
			Assert::AreEqual(y, roomPos.y);
			Assert::AreEqual(x - unitWidth, roomPos.x);
		}

		TEST_METHOD(getNullMapRoomPosTest)
		{
			Image img;
			map.setDimensions(2, 1);
			img.setDimensions(2, 1);
			int mapId = -1;
			img.setTile(0, 0, mapId); //no real room loaded
			img.setTile(1, 0, mapId);

			int unitLength = 6;
			map.setUnitWidth(unitLength);
			map.setUnitHeight(unitLength);
			map.setLayerImage(0, img);

			int y = 9;
			int x = 2;

			map.setCursor(&y, &x);

			Pos roomPos = map.getMapRoomPos();
			Assert::AreEqual(y - unitLength, roomPos.y);
			Assert::AreEqual(x , roomPos.x);
		}

		TEST_METHOD(getDepthTest)
		{
			int row = 1;
			int col = 2;
			int depth = 2;
			map.setDimensions(row, col, depth);
		
			Assert::AreEqual(depth, map.getDepth());
		}

		TEST_METHOD(getFloorIndexTest)
		{
			map.setDimensions(1, 1, 2);

			Pos cursor(0, 0); //set to 0,0 1F
			map.setCursor(&cursor.y, &cursor.x);

			Assert::AreEqual(0, map.getFloorIndex());
		}

		TEST_METHOD(setFloorTest)
		{
			map.setDimensions(1, 1, 6);
			map.setGroundFloor(3);

			map.setFloor("2F");

			Assert::AreEqual(1, map.getFloorIndex());
		}

		TEST_METHOD(setBelowFloorTest)
		{
			map.setDimensions(1, 1, 6);
			map.setGroundFloor(3);

			map.setFloor("B2");

			Assert::AreEqual(-2, map.getFloorIndex());
		}

		TEST_METHOD(changeFloorTest)
		{
			map.setDimensions(1, 1, 2);

			Pos cursor(0, 0); //set to 0,0 1F
			map.setCursor(&cursor.y, &cursor.x);
			map.changeLayer(1);

			Assert::AreEqual(1, map.getFloorIndex());
		}

		TEST_METHOD(changeFloorSubTest)
		{
			map.setDimensions(1, 1, 2);
			map.setGroundFloor(1);
			Pos cursor(0, 0); //set to 0,0 B1 since ground level is at 1
			map.setCursor(&cursor.y, &cursor.x);
			map.setFloorIndex(0);
			map.changeLayer(-1);

			Assert::AreEqual(-1, map.getFloorIndex());
		}

		TEST_METHOD(changeFloorOutOfBoundsTest)
		{
			map.setDimensions(1, 1, 2);

			Pos cursor(0, 0); //set to 0,0 1F
			map.setCursor(&cursor.y, &cursor.x);
			map.changeLayer(2); //go up 2 floors beyond top of megamap

			Assert::AreEqual(0, map.getFloorIndex()); //still 0 since the layer change fails
		}

		TEST_METHOD(getPositiveFloorLabel)
		{
			map.setDimensions(1, 1, 2);
			Pos cursor(0, 0); 
			map.setCursor(&cursor.y, &cursor.x);

			Assert::IsTrue(map.getFloorLabel().compare("1F") == 0);
		}

		TEST_METHOD(getPositiveFloorWithOffsetLabel)
		{
			map.setDimensions(1, 1, 9);
			Pos cursor(0, 0); 
			map.setCursor(&cursor.y, &cursor.x);
			map.setGroundFloor(4);
			map.setFloorIndex(4);

			Assert::IsTrue(map.getFloorLabel().compare("5F") == 0);
		}

		TEST_METHOD(getNegativeFloorWithOffsetLabel)
		{
			map.setDimensions(1, 1, 9);
			Pos cursor(0, 0);
			map.setCursor(&cursor.y, &cursor.x);
			map.setGroundFloor(4);
			map.setFloorIndex(-3);

			Assert::IsTrue(map.getFloorLabel().compare("B3") == 0);
		}

		TEST_METHOD(visitTest)
		{			
			Image img;

			map.setDimensions(1, 1);
			img.setDimensions(1, 1);
			img.setTile(0, 0, 64);

			map.setLayerImage(0, img);
			Pos cursor(0, 0);

			map.setCursor(&cursor.y, &cursor.x);
			map.visitArea();

			auto& autoMap = map.getAutoMap();
			
			Assert::AreEqual((int)' ' | setBkgdColor(COLOR_CYAN), autoMap[0].getTile(0, 0));
		}

		TEST_METHOD(visitMapKnownTest)
		{
			Image img;

			map.setDimensions(2, 2);
			img.setDimensions(2, 2);
			img.setTile(0, 0, 64);
			img.setTile(1, 0, 64);
			img.setTile(0, 1, 64);
			img.setTile(1, 1, 64);

			map.setLayerImage(0, img);
			Pos cursor(1, 1);

			map.setCursor(&cursor.y, &cursor.x);
			map.visitArea();

			auto& autoMap = map.getAutoMap();

			chtype known = ' ' | setBkgdColor(COLOR_BLUE);
			chtype visited = ' ' | setBkgdColor(COLOR_CYAN);

			Assert::AreEqual(known, autoMap[0].getTile(0, 0));
			Assert::AreEqual(known, autoMap[0].getTile(0, 1));
			Assert::AreEqual(known, autoMap[0].getTile(1, 0));
			Assert::AreEqual(visited, autoMap[0].getTile(1, 1));
		}

		TEST_METHOD(visitIsolatedMapTest)
		{
			Image img;
			int dim = 4;
			map.setDimensions(dim, dim);
			img.setDimensions(dim, dim);
			img.getTileMap().fill(-1);

			for (int row = 1; row < 3; row++)
			{
				for (int col = 1; col < 3; col++)
				{
					img.setTile(row, col, 64);
				}
			}

			map.setLayerImage(0, img);
			Pos cursor(2, 2);

			map.setCursor(&cursor.y, &cursor.x);
			map.visitArea();

			auto& autoMap = map.getAutoMap();

			chtype unknown = ' ';
			chtype known = ' ' | setBkgdColor(COLOR_BLUE);
			chtype visited = ' ' | setBkgdColor(COLOR_CYAN);

			Assert::AreEqual(unknown, autoMap[0].getTile(0, 0));
			Assert::AreEqual(known, autoMap[0].getTile(1, 1));
			Assert::AreEqual(visited, autoMap[0].getTile(2, 2));
			Assert::AreEqual(unknown, autoMap[0].getTile(3, 3));
		}
		
	};
}