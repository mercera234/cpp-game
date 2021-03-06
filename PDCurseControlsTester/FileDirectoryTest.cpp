#include "CppUnitTest.h"
#include "FileDirectory.h"
#include "MockFileDirectory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(FileDirectoryTest)
	{
		TEST_METHOD(defaultConstructorTest)
		{
			std::string path = "fake path";
			FileDirectory dir;
			dir.setPath(path);

			int retval = path.compare(dir.getPath());
			Assert::AreEqual(0, retval);
		}

		TEST_METHOD(getPathTest)
		{
			std::string path = "fake path";
			FileDirectory dir(path);

			int retval = path.compare(dir.getPath());
			Assert::AreEqual(0, retval);
		}

		TEST_METHOD(getFilesTest)
		{
			MockFileDirectory dir;

			std::list<dirent> retFiles = dir.getFiles(); //mock method creates on file sets type to DT_REG
			dirent entry = retFiles.front(); 
			
			Assert::AreEqual(DT_REG, entry.d_type);
		}

		TEST_METHOD(addDirToPathTest)
		{
			std::string path = "C:\\This is a fake path";
			FileDirectory dir(path);

			dir.addToPath("subfolder");
			
			Assert::AreEqual(0, dir.getPath().compare("C:\\This is a fake path\\subfolder"));
		}

		TEST_METHOD(removeDirFromPathTest)
		{
			std::string path = "C:\\This-is_a-fake~path\\subfolder";
			FileDirectory dir(path);

			dir.removeLastFromPath();

			Assert::AreEqual(0, dir.getPath().compare("C:\\This-is_a-fake~path"));
		}
	};
}