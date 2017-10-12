#include "text_tests.h"
#include "TextField.h"
#include "TextLabel.h"

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
