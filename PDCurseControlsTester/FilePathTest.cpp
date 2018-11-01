#include "CppUnitTest.h"
#include "MockFilePath.h"
#include "MockStorable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(HardDriveTest)
	{
		std::string fakePath = "C:\\fake path";
		TEST_METHOD(ctor)
		{
			MockFilePath hd(fakePath);
			
			Assert::IsTrue(hd.getPath().compare(fakePath) == 0);
		}

		TEST_METHOD(openFolder)
		{
			MockFilePath hd(fakePath);
			std::string newFolder = "data";
			hd.openFolder(newFolder);

			Assert::IsTrue(hd.getPath().compare(fakePath + '\\' + newFolder) == 0);
		}

		TEST_METHOD(setPath)
		{
			MockFilePath hd(fakePath);
			std::string newPath = "F:\\really fake path\\another dir";
			hd.setPath(newPath);

			Assert::IsTrue(hd.getPath().compare(newPath) == 0);
		}

		TEST_METHOD(save_withStream)
		{
			MockFilePath hd(fakePath);
			hd.setObject(new MockStorable);
			
			std::ofstream os;
			
			Assert::AreEqual(200, hd.save(os)); //fake save value
		}

		TEST_METHOD(save)
		{
			MockFilePath hd(fakePath);
			hd.setObject(new MockStorable);

			Assert::AreEqual(200, hd.save()); //fake save value
		}

		TEST_METHOD(save_noObject)
		{
			MockFilePath hd(fakePath);
		
			Assert::AreEqual(-1, hd.save()); //fake save value
		}

		TEST_METHOD(load_withStream)
		{
			MockFilePath hd(fakePath);
			hd.setObject(new MockStorable);

			std::ifstream is;

			Assert::AreEqual(400, hd.load(is)); //fake save value
		}

		TEST_METHOD(load)
		{
			MockFilePath hd(fakePath);
			hd.setObject(new MockStorable);

			Assert::AreEqual(400, hd.load()); //fake save value
		}

		TEST_METHOD(load_noObject)
		{
			MockFilePath hd(fakePath);

			Assert::AreEqual(-1, hd.load()); //fake save value
		}

		TEST_METHOD(setWriteMode)
		{
			MockFilePath hd(fakePath);
			std::ios_base::openmode mode = std::ios::binary | std::ios::app;
			hd.setWriteMode(mode);

			Assert::AreEqual(mode, hd.getWriteMode()); //fake save value
		}
	};
}