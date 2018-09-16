#include "CppUnitTest.h"
#include "FormField.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PDCurseControlsTester
{
	TEST_CLASS(FormFieldTest)
	{
		TUI tui;

		TEST_METHOD_INITIALIZE(startTUI)
		{
			tui.init();
		}

		TEST_METHOD_CLEANUP(stopTUI)
		{
			tui.shutdown();
		}

		TEST_METHOD(ctorTest)
		{
			FormField field("Field:");

			Assert::IsTrue(field.getLabel().getText().compare("Field:") == 0);
		}

		TEST_METHOD(setFocusTest)
		{
			FormField field;
			field.setFocus(true);

			Assert::IsTrue(field.getField().isFocused());
		}

		TEST_METHOD(setContentLabelTest) //label and text are inline
		{
			FormField field;
			int lblY, lblX;
			lblY = 0;
			lblX = 0;
			field.setContent(newwin(1, 5, lblY, lblX), "Word", 5, 0, 5);
			WINDOW* win = field.getWindow();

			Assert::AreEqual(lblY, getbegy(win));
			Assert::AreEqual(lblX, getbegx(win));
			Assert::AreEqual(1, getmaxy(win));
			Assert::AreEqual(10, getmaxx(win));
		}

		TEST_METHOD(setContentLabelTest2) //label is up and to the left of text
		{
			FormField field;
			int lblY, lblX;
			lblY = 1;
			lblX = 1;
			field.setContent(newwin(1, 7, lblY, lblX), "", 16, 2, 4);
			WINDOW* win = field.getWindow();

			Assert::AreEqual(lblY, getbegy(win));
			Assert::AreEqual(lblX, getbegx(win));
			Assert::AreEqual(2, getmaxy(win));
			Assert::AreEqual(19, getmaxx(win));
		}
	};
}