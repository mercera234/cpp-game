#include "CppUnitTest.h"
#include "DialogBuilder.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "TextBoard.h"
#include "MultiControl.h"
#include "actor_helper.h"
#include "ItemBrowser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(DialogBuilderTest)
	{
		TUI tui;
		DialogBuilder builder;
		ResourceManager rm;
		Actor a;
		
		TEST_METHOD_INITIALIZE(start)
		{
			builder.setRm(&rm);
			initDefaultActor(a);
		}


		TEST_METHOD(buildMainMenuTest)
		{
			DialogWindow win;
			builder.buildMainMenu(win, Rect(0, 0, Pos(10, 10)));

			GridMenu* menu = static_cast<GridMenu*>(win.getControl());
			int itemsCreated = menu->getMaxItems();
			Assert::AreEqual(itemsCreated, 8);

			LineItem* item = static_cast<LineItem*>(menu->getItem(2, 1));

			Assert::AreEqual(item->name.c_str(), "Save");
		}

		TEST_METHOD(buildPlayerMenuTest)
		{
			DialogWindow win;
			builder.buildPlayerMenu(win, Rect(0, 0, Pos(10, 10)));

			GridMenu* menu = static_cast<GridMenu*>(win.getControl());
			int itemsCreated = menu->getMaxItems();
			Assert::AreEqual(itemsCreated, 4);
		}

		TEST_METHOD(buildMainMenuBodyTest)
		{
			DialogWindow win;
			builder.buildMainMenuBody(win, Rect(0, 0, Pos(10, 10)));

			TextBoard* board = dynamic_cast<TextBoard*>(win.getControl());
			
			Assert::AreEqual(4, board->getPieceCount());
		}

		TEST_METHOD(buildDescTest)
		{
			DialogWindow win;
			builder.buildDesc(win, Rect(0, 0, Pos(10, 10)));

			TextBoard* board = dynamic_cast<TextBoard*>(win.getControl());

			Assert::AreEqual(3, board->getPieceCount());
		}

		TEST_METHOD(buildInventoryTest)
		{
			DialogWindow win;
			builder.buildInventory(win, Rect(4, 3, Pos(11, 5)));

			Assert::IsTrue(dynamic_cast<ItemBrowser*>(win.getControl()) != nullptr);
		}

		/*TEST_METHOD(buildEquipDialogTest)
		{
			DialogWindow win;
			builder.buildEquipDialog(win, Rect(0, 0, Pos(10, 10)), a);

			MultiControl* mControl = dynamic_cast<MultiControl*>(win.getControl());
			GridMenu* menu = (GridMenu*)mControl->getControl("Equip Menu");

			Assert::AreEqual(6, (int)menu->getMaxItems());
		}*/
	};
}