#include "CppUnitTest.h"
#include "GameData.h"
#include "json_tree_creators.h"
#include <limits>
#include <exception>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(GameDataTest)
	{
		TUI tui;
		GameData data;

		TEST_METHOD(getNullResourcesTest)
		{
			Assert::AreEqual(1, (int)data.getActors().size());
			Assert::AreEqual(1, (int)data.getRooms().size());
			Assert::AreEqual(1, (int)data.getMaps().size());
		}

		TEST_METHOD(getNullResourcesActorContentTest)
		{
			Actor& a = data.getActor(nullName);

			Assert::AreEqual((char)nullSymbol, (char)a.symbol);
		}

		TEST_METHOD(getNullResourcesMapContentTest)
		{
			MapRoom& m = data.getRoom(nullId);
			ITwoDStorage<chtype>& tileMap = m.getDisplay().getTileMap();
			chtype c = tileMap.getDatum(4, 8);

			Assert::AreEqual('!', (char)c);
		}

		TEST_METHOD(getNextIdTest)
		{
			data.resetNextId();
			Assert::AreEqual(0, data.getNextId()); //without being called before, next id will be 0
		}

		TEST_METHOD(storeIntTest)
		{
			BoundInt someInt(0, 100, 36);
			Assert::IsTrue(data.storeIntData("Gold", someInt));
		}

		TEST_METHOD(retrieveUnstoredInt)
		{
			BoundInt& x = data.retrieveIntData("anything");	
			Assert::AreEqual(0, x.getCurr());
		}

		TEST_METHOD(storeRetrieveIntTest)
		{
			BoundInt value(0, INT_MAX, 34578665);
			std::string key = "Val";
			data.storeIntData(key, value);

			BoundInt& getVal = data.retrieveIntData(key);
			
			Assert::AreEqual(value.getCurr(), getVal.getCurr());
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

		TEST_METHOD(loadActorsTest)
		{
			boost::property_tree::ptree actorsNode;
			createActorJSONtree(actorsNode);

			data.loadActors(actorsNode);
			Assert::AreEqual(3, (int)data.getActors().size()); //3 = 2 actors + 1 null actor
		}

		TEST_METHOD(loadItemsTest)
		{
			boost::property_tree::ptree itemsNode;
			createItemJSONtree(itemsNode);

			data.loadItems(itemsNode);
			Assert::AreEqual(4, (int)data.getItems().size()); //4 = 3 items + 1 null item 
		}


		//save this test elsewhere, for testing passing derived class into a map for a base class
		//struct ATest
		//{
		//	int x;
		//};

		//struct BTest : public ATest
		//{
		//	double y;
		//};

		//TEST_METHOD(mapLoadTest)
		//{
		//	std::map<int, ATest> t;

		//	BTest b;
		//	b.x = 478;
		//	b.y = 179.346;
		//	t.insert(std::make_pair(1, b));

		//	BTest& bref = (BTest&)t[1];

		//	Assert::AreEqual(179, (int)bref.y);
		//}

		TEST_METHOD(loadRoomsTest)
		{
			boost::property_tree::ptree roomsNode;
			createMapRoomJSONtree(roomsNode);

			data.loadRooms(roomsNode);
			Assert::AreEqual(4, (int)data.getRooms().size()); //= 3 rooms + 1 null room
		}

		TEST_METHOD(loadEnemyPoolsTest)
		{
			boost::property_tree::ptree poolsNode;
			createEnemyPoolJSONtree(poolsNode);

			data.loadEnemyPools(poolsNode);
			Assert::AreEqual(4, (int)data.getEnemyPools().size());//= 3 pools + 1 null pool
		}

		TEST_METHOD(loadMapsTest)
		{
			boost::property_tree::ptree mapsNode;
			createMapJSONtree(mapsNode);

			data.loadMaps(mapsNode);
			Assert::AreEqual(2, (int)data.getMaps().size()); //1 map loaded, 1 null map
		}

		TEST_METHOD(loadNullMapsTest)
		{
			boost::property_tree::ptree mapsNode;
			createMapJSONtree(mapsNode);

			data.loadMaps(mapsNode);
			MegaMap& map = data.getMap("TestRegion");
			Image& img = map.getMapRoomLayout()[0];

			Assert::AreEqual(-1, (int)img.getTile(0, 2)); //1 map loaded, 1 null map
		}


	};
}