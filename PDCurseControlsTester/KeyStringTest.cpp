#include "CppUnitTest.h"
#include "key_strings.h"
#include "TUI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(KeyStringTest)
	{
		TEST_METHOD(outOfBoundsKey)
		{
			std::string k = getStringFromKey(-1);

			Assert::IsTrue(k.empty());
		}

		TEST_METHOD(getCtrlKey)
		{
			std::string k = getStringFromKey(0);

			Assert::IsTrue(k.compare("CTRL_AT") == 0);
		}

		TEST_METHOD(getAlphaCtrlKey)
		{
			std::string k = getStringFromKey(1);

			Assert::IsTrue(k.compare("CTRL_A") == 0);
		}

		TEST_METHOD(getSpaceKey)
		{
			std::string k = getStringFromKey(' ');

			Assert::IsTrue(k.compare("SPACE") == 0);
		}

		TEST_METHOD(getNonVerboseSpaceKey)
		{
			std::string k = getStringFromKey(' ', false);

			Assert::IsTrue(k.compare(" ") == 0);
		}

		TEST_METHOD(getAsciiKey)
		{
			std::string k = getStringFromKey('a');

			Assert::IsTrue(k.compare("a") == 0);
		}

		TEST_METHOD(getDirKey)
		{
			std::string k = getStringFromKey(KEY_DOWN);

			Assert::IsTrue(k.compare("DOWN") == 0);
		}

		TEST_METHOD(getAlphaKeyFromString)
		{
			int k = getKeyFromString("b");

			Assert::AreEqual((int)'b',k);
		}

		TEST_METHOD(getKeyFromLowercaseString)
		{
			int k = getKeyFromString("space");

			Assert::AreEqual((int)' ', k);
		}
	};
}