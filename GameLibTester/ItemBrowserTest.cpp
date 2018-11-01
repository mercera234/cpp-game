#include "CppUnitTest.h"
#include "ItemBrowser.h"
#include "GameItem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(ItemBrowserTest)
	{
		TEST_METHOD(getDescriptionNoItemsTest)
		{
			ItemBrowser inventory;
			std::list<std::string> lines;
			inventory.getDescription(lines);

			Assert::AreEqual(0, (int)lines.size());
		}

		TEST_METHOD(getDescription2NoItemsTest)
		{
			ItemBrowser inventory;
			Assert::AreEqual("", inventory.getDescription().c_str());
		}

		TEST_METHOD(getDescriptionTest)
		{
			GameItem knife;
			knife.name = "knife";
			knife.description = "A blade for cutting";

			OwnedItem item;
			item.item = &knife;
			typedef std::vector<OwnedItem*> Inventory;
			Inventory items = { &item };

			ItemBrowser inventory;
			inventory.setItems(items);

			std::list<std::string> lines;
			inventory.getDescription(lines);

			Assert::IsTrue(knife.description == lines.front());
		}

		TEST_METHOD(getDescription2Test)
		{
			GameItem knife;
			knife.name = "knife";
			knife.description = "A blade for cutting";

			OwnedItem item;
			item.item = &knife;
			typedef std::vector<OwnedItem*> Inventory;
			Inventory items = { &item };

			ItemBrowser inventory;
			inventory.setItems(items);

			Assert::AreEqual(knife.description.c_str(), inventory.getDescription().c_str());
		}
	};
}