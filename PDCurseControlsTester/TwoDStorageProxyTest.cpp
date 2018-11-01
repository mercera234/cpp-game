#include "CppUnitTest.h"
#include "TwoDStorageProxy.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(TwoDStorageProxyTest)
	{
		
		TEST_METHOD(ctorTest)
		{
			int rows = 3;
			int cols = 6;
			TwoDStorageProxy<int> proxy(rows, cols);

			Assert::AreEqual(rows, proxy.getRows());
			Assert::AreEqual(cols, proxy.getCols());
		}

		TEST_METHOD(setDimensionsTest)
		{
			int rows = 3000;
			int cols = 600;
			TwoDStorageProxy<int> proxy;
			proxy.setDimensions(rows, cols);

			Assert::AreEqual(rows * cols, proxy.getSize());
			Assert::AreEqual(0, proxy.getRealSize());
		}

		TEST_METHOD(setDatumTest)
		{
			int rows = 10;
			int cols = 6;
			TwoDStorageProxy<int> proxy(rows, cols);
			int size = proxy.getSize();

			int datum = 2;
			int lastNdx = rows * cols - 1;
			proxy.setDatum(lastNdx, datum); //this will enlarge the data vector to meet the extent

			Assert::AreEqual(size, proxy.getSize()); //size will appear unchanged
			Assert::AreEqual(datum, proxy.getDatum(lastNdx)); //size will appear unchanged
		}

		TEST_METHOD(hideDataTest)
		{
			int rows = 3000;
			int cols = 600;
			TwoDStorageProxy<int> proxy;
			proxy.setDimensions(rows, cols);
			proxy.fill(4);//should fill the data
			proxy.hideData();

			Assert::AreEqual(0, proxy.getRealSize());
		}
	};
}