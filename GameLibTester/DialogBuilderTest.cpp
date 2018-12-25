#include "CppUnitTest.h"
#include "DialogBuilder.h"
#include "GridMenu.h"
#include "LineItem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameLibTester
{
	TEST_CLASS(DialogBuilderTest)
	{
		TUI tui;
		DialogBuilder builder;

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

		//TEST_METHOD(buildBodyTest)
		//{
		//	/*DialogWindow win;
		//	ResourceManager rm;
		//	builder.buildMainMenuBody(win, Rect(0, 0, Pos(10, 10)), rm);


		//	Assert::IsTrue(true);*/
		//	//Assert::AreEqual(itemsCreated, 4);
		//}
	};
}