#include "CppUnitTest.h"
#include "DataPkg.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RPGLibTester
{		
	TEST_CLASS(DataPkgTest)
	{
	public:
		
		TEST_METHOD(appendDataOneByteOnlyTest)
		{
			DataPkg pkg;

			char c = 'A';
			pkg.appendData(&c, 1);

			char r;
			pkg.readData(&r, 1, 0);

			Assert::AreEqual('A', r);
		}


		TEST_METHOD(appendDataFullBlockTest)
		{
			DataPkg pkg;

			int size = BLOCK_SIZE;
			char* testArr = new char[size];
			memset(testArr, 0x77, size);

			pkg.appendData(testArr, size);

			char r;
			pkg.readData(&r, 1, BLOCK_SIZE - 1); //minus one or else we would go over

			Assert::AreEqual((char)0x77, r);
		}


		TEST_METHOD(appendDataOverOneBlockTest)
		{
			DataPkg pkg;

			int size = BLOCK_SIZE + 1;
			char* testArr = new char[size];
			memset(testArr, 'l', size);

			pkg.appendData(testArr, size);

			Assert::AreEqual(BLOCK_SIZE * 2, pkg.getSize());
		}

		TEST_METHOD(writeDataTest)
		{
			DataPkg pkg;

			char c = 4;
			pkg.writeData(&c, 1, 15); //beyond append offset which should be 0

			char r;
			pkg.readData(&r, 1, 15);

			Assert::AreEqual(c, r);
		}

		TEST_METHOD(writeDataCreatesNewBlocksTest)
		{
			DataPkg pkg;

			char c = 4;
			pkg.writeData(&c, 1, BLOCK_SIZE * 2); //at first index of 3rd block

			Assert::AreEqual(BLOCK_SIZE * 3, pkg.getSize());
		}

		TEST_METHOD(readNextDataTest)
		{
			DataPkg pkg;

			char* data = "abcdefghijklmnopqrstuvwxyz";
			pkg.appendData(data, 26); 

			char c;
			pkg.readNextData(&c, 1);

			Assert::AreEqual('a', c);
		}

		TEST_METHOD(readNextDataAfterReadTest)
		{
			DataPkg pkg;

			char* data = "abcdefghijklmnopqrstuvwxyz";
			pkg.appendData(data, 26);

			int bufsize = 10;
			char* buf = new char[bufsize];
			pkg.readNextData(buf, bufsize);//this should read just past the j

			char c;
			pkg.readNextData(&c, 1);

			Assert::AreEqual('k', c);
		}

		TEST_METHOD(shiftReadOffsetTest)
		{
			DataPkg pkg;

			char* data = "abcdefghijklmnopqrstuvwxyz";
			pkg.appendData(data, 26);

			int bufsize = 10;
			char* buf = new char[bufsize];
			pkg.readNextData(buf, bufsize);//this should read just past the j

			pkg.shiftReadOffset(-3);

			char c;
			pkg.readNextData(&c, 1);

			Assert::AreEqual('h', c);
		}

		TEST_METHOD(setReadOffsetTest)
		{
			DataPkg pkg;

			char* data = "abcdefghijklmnopqrstuvwxyz";
			pkg.appendData(data, 26);

			pkg.setReadOffset(24);

			char c;
			pkg.readNextData(&c, 1);

			Assert::AreEqual('y', c);
		}
	};
}