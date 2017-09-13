#include "stdafx.h"
#include "CppUnitTest.h"
#include "FileDirectory.h"
#include "MockFileDirectory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(FileDirectoryTest)
	{
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
	};
}