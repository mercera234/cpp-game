#include "panel_tests.h"
#include "CursesPanel.h"

void panelTest()
{
	CursesPanel panel(0, 0, 10, 20, "Title");
	init_pair(5, COLOR_WHITE, COLOR_BLUE);
	panel.setColor(5);
	panel.show();
	doupdate();
	getch();

	CursesPanel panel2(5, 0, 20, 6, "Msg Dialogue");
	panel2.setColor(5);
	panel2.show();
	doupdate();
	getch();

	panel.top();
	doupdate();
	getch();

	panel2.top();
	doupdate();
	getch();

	panel2.bottom();
	doupdate();
	getch();
}