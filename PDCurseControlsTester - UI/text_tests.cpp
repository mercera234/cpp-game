#include "text_tests.h"
#include "TextField.h"
#include "TextLabel.h"
#include "FormField.h"

void textFieldtest()
{
	TextField field(15, 1, 1);
	field.setText("Default text");
	field.setColor(COLOR_BLUE, COLOR_MAGENTA_BOLD);

	bool playing = true;
	while (playing)
	{
		curs_set(CURSOR_NORMAL);
		field.draw();
		field.setCursorFocus();
		doupdate();
		int c = getch();

		switch (c)
		{
		case KEY_ESC:
			playing = false;
			break;
		case CTRL_N:
			field.clear();
			break;
		case '\r':
		case '\n':
		case KEY_ENTER:
		{
			std::string text = field.getText();
			mvaddstr(3, 1, text.c_str());
		}
		break;
		default:
			field.inputChar(c);
		}
	}

}

void textLabelTest()
{
	WINDOW* win = newwin(1, 10, 2, 2);
	TextLabel* label = new TextLabel(win, "12345678901");

	label->draw();
	doupdate();
	getch();
}

void formFieldTest()
{
	TextLabel* lbl = new TextLabel(newwin(1, 10, 1, 1), "Testlabel");
	TextField* field = new TextField(16, 2, 1);
	FormField* fField = new FormField(lbl, field, 0, NULL);

	bool playing = true;
	while (playing)
	{
		fField->draw();
		field->setCursorFocus();
		doupdate();
		int c = getch();

		if (c == KEY_ESC)
		{
			playing = false;
		}
		else
			field->inputChar(c);
	}
}