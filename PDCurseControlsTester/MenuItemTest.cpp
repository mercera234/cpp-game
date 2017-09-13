#include "CppUnitTest.h"
#include "MockMenuItem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(MenuItemTest)
	{
		TEST_METHOD(emptyConstructorTest)
		{
			MockMenuItem mmi;

			Assert::IsTrue(0 == mmi.posY && 0 == mmi.posX);
		}

		TEST_METHOD(overloadedConstructorTest)
		{
			MockMenuItem mmi(35, 96);

			Assert::IsTrue(35 == mmi.posY && 96 == mmi.posX);
		}

		TEST_METHOD(setPositionTest)
		{
			MockMenuItem mmi;
			unsigned int y = 7;
			unsigned int x = 9000;
			mmi.setPosition(y, x);

			Assert::IsTrue(y == mmi.posY && x == mmi.posX);
		}

		TEST_METHOD(linkTest)
		{
			MockMenuItem mmi;
			MockMenuItem mmi2;

			mmi.link(Dir::RIGHT, &mmi2);
			
			Assert::IsTrue(mmi2.getLinkedItem(Dir::LEFT) == &mmi);
		}

		TEST_METHOD(unlinkTest)
		{
			MockMenuItem mmi;
			MockMenuItem mmi2;

			mmi.link(Dir::UP, &mmi2);
			mmi2.link(false, Dir::DOWN, &mmi); //unlink

			Assert::IsFalse(mmi.getLinkedItem(Dir::UP) == &mmi2);
		}

		TEST_METHOD(clearLinksTest)
		{
			MockMenuItem mmi;
			MockMenuItem mmiD;

			mmi.link(Dir::DOWN, &mmiD);
			
			mmi.clearLinks();

			Assert::IsFalse(mmi.getLinkedItem(Dir::DOWN) == &mmiD);
		}

		TEST_METHOD(clearTest)
		{
			MockMenuItem mmi;
			
			mmi.selectable = true;
			mmi.selected = true;
			mmi.crossRef = 78;
			
			mmi.index = 0;
			mmi.hidden = true;

			mmi.clear();
			
			Assert::IsTrue(mmi.selectable == false && mmi.selected == false && mmi.crossRef == -1 && mmi.index == -1 && mmi.hidden == false);
		}


	};
}