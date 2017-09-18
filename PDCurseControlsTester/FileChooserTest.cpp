#include "CppUnitTest.h"
#include "FileChooser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(FileChooserTest)
	{
		TEST_METHOD(constructorTest)
		{
			FileDialogType type = FileDialogType::OPEN_DIALOG;
			FileChooser fc("", type);

			Assert::IsTrue(type == fc.getType());
		}
	};
}