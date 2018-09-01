#include "CppUnitTest.h"
#include "MockObjectLinkedItem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(ObjectLinkedTest)
	{
		TEST_METHOD(constructorTest)
		{
			int value = 3;
			MockObjectLinkedItem<int> obj(value);

			Assert::AreEqual(value, obj.getCrossRef());
		}

		TEST_METHOD(constructorPtrTest)
		{
			struct A {
				int x = 3;
			};
			A* a = new A;
			MockObjectLinkedItem<A*> obj(a);
			
			Assert::AreEqual(3, obj.getCrossRef()->x);
			delete a;
		}

		TEST_METHOD(constructorRefTest)
		{
			struct A {
				int x = 3;
			};
			A a;
			MockObjectLinkedItem<A*> obj(&a);

			Assert::AreEqual(3, obj.getCrossRef()->x);
		}


	};
}