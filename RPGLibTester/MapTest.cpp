#include "CppUnitTest.h"
#include "TUI.h"
#include "Map.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(MapTest)
	{
		TUI tui;

		TEST_METHOD_INITIALIZE(startTUI)
		{
			tui.init();
		}

		TEST_METHOD_CLEANUP(stopTUI)
		{
			tui.shutdown();
		}

		TEST_METHOD(newMapConstructorTest)
		{
			int rows = 3;
			int cols = 5;
			WINDOW* win = newwin(rows + 10, cols + 10, 0, 0); //the window dimensions do not have to match the map

			std::string name = "test map";
			Map theMap(name, rows, cols, win);

			Assert::AreEqual(0, theMap.getName().compare(name));
			Assert::AreEqual(rows, (int)theMap.getTotalRows());
			Assert::AreEqual(cols, (int)theMap.getTotalCols());
			Assert::IsTrue(win == theMap.getWindow());
		}

		TEST_METHOD(resetTest)
		{
			int rows = 1;
			int cols = 1;
			WINDOW* win = newwin(rows + 10, cols + 10, 0, 0); //the window dimensions do not have to match the map

			std::string name = "test map";
			Map theMap(name, rows, cols, win);

			Image* img = theMap.getDisplay();

			TwoDStorage<chtype>* tileMap = img->getTileMap();

			tileMap->fill('A');

			theMap.reset();

			Assert::AreEqual(' ', (char)tileMap->getDatum(0));
		}

		TEST_METHOD(resizeTest)
		{
			int rows = 1;
			int cols = 1;
			WINDOW* win = newwin(rows + 10, cols + 10, 0, 0); //the window dimensions do not have to match the map

			std::string name = "test map";
			Map theMap(name, rows, cols, win);

			theMap.resize(2, 4);

			Assert::AreEqual(2, (int)theMap.getTotalRows());
			Assert::AreEqual(4, (int)theMap.getTotalCols());
		}

		TEST_METHOD(resizeLayersTest)
		{
			int rows = 4;
			int cols = 3;
			WINDOW* win = newwin(rows, cols, 0, 0); //the window dimensions do not have to match the map

			std::string name = "test map";
			Map theMap(name, rows, cols, win);

			int newRows = 3;
			int newCols = 1;
			theMap.resize(newRows, newCols);

			TwoDStorage<chtype>* img = theMap.getDisplay()->getTileMap();
			TwoDStorage<EffectType> tileEffects = theMap.getEffectsLayer();

			Assert::AreEqual(newRows * newCols, (int)img->getSize());
			Assert::AreEqual(newRows * newCols, (int)tileEffects.getSize());
		}

		//new maps set all tile effects to nullptr by default
		TEST_METHOD(newMapTileEffectTest)
		{
			int rows = 1;
			int cols = 1;
			WINDOW* win = newwin(rows + 10, cols + 10, 0, 0); //the window dimensions do not have to match the map

			std::string name = "test map";
			Map theMap(name, rows, cols, win);

			TwoDStorage<EffectType> tileEffects = theMap.getEffectsLayer();
			Assert::IsTrue(EffectType::NONE == tileEffects.getDatum(0));
		}


		//tile effect tests
		TEST_METHOD(noArgConstructorTileEffectTile)
		{
			TileEffect e;

			Assert::IsTrue(EffectType::NONE == e.getType());	
		}

		TEST_METHOD(constructorTileEffectTile)
		{
			EffectType t = EffectType::OBSTR;
			TileEffect e(t);

			Assert::IsTrue(t == e.getType());
		}
	};
}