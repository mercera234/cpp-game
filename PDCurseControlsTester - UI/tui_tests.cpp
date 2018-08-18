#include "tui_tests.h"
#include "TUI.h"

void allPairsChangedTest()
{
	for (int i = 0; i < 16; i++)
	{
		attron(COLOR_PAIR(i));
		chtype attrs = getattrs(stdscr);
		mvprintw(i, 0, "Color test: 0x%x", attrs);
	}

	doupdate();
	getch();
	attroff(A_COLOR);

	init_pair(1, COLOR_WHITE, COLOR_GREEN); //snowy trees
	init_pair(2, COLOR_CYAN, COLOR_BLUE); //water
	init_pair(3, COLOR_YELLOW, COLOR_BLACK); //hay
	init_pair(4, COLOR_GREEN, COLOR_YELLOW); //poison?
	init_pair(5, COLOR_CYAN, COLOR_BLUE); //water again
	init_pair(6, COLOR_RED, COLOR_WHITE); //blood on concrete
	init_pair(7, COLOR_YELLOW, COLOR_RED); //royalty
	init_pair(8, COLOR_RED, COLOR_GREEN); //christmas
	init_pair(9, COLOR_GREEN, COLOR_BLUE); //moss
	init_pair(10, COLOR_BLACK, COLOR_WHITE); //inverted
	init_pair(11, COLOR_YELLOW, COLOR_MAGENTA); //porn club
	init_pair(12, COLOR_GRAY, COLOR_WHITE);
	init_pair(13, COLOR_MAGENTA, COLOR_GRAY);
	init_pair(14, COLOR_GREEN, COLOR_MAGENTA); //snake charmer
	init_pair(15, COLOR_WHITE, COLOR_RED); //standing in blood
	init_pair(16, COLOR_BLACK, COLOR_RED); //shadow
	init_pair(17, COLOR_WHITE, COLOR_GREEN_BOLD); //snowy trees
	init_pair(18, COLOR_CYAN, COLOR_BLUE_BOLD); //water
	init_pair(19, COLOR_YELLOW_BOLD, COLOR_BLACK); //hay
	init_pair(20, COLOR_GREEN, COLOR_YELLOW); //poison?
	init_pair(21, COLOR_CYAN_BOLD, COLOR_BLUE); //water again
	init_pair(22, COLOR_RED, COLOR_WHITE_BOLD); //blood on concrete
	init_pair(23, COLOR_YELLOW, COLOR_RED); //royalty
	init_pair(24, COLOR_RED, COLOR_GREEN); //christmas
	init_pair(25, COLOR_GREEN, COLOR_GRAY);
	init_pair(26, COLOR_BLACK, COLOR_WHITE); //inverted
	init_pair(27, COLOR_YELLOW, COLOR_MAGENTA_BOLD); //porn club
	init_pair(28, COLOR_GRAY_BOLD, COLOR_WHITE); //gray is not a legal color, and thus it uses the default of white on black
	init_pair(29, COLOR_WHITE, COLOR_BLACK);
	init_pair(30, COLOR_GREEN, COLOR_MAGENTA); //snake charmer
	init_pair(31, COLOR_WHITE, COLOR_RED); //standing in blood
	init_pair(32, COLOR_WHITE, COLOR_YELLOW);

	int col = 0;
	for (int i = 0, row = 0; i < 33; i++)
	{
		attron(COLOR_PAIR(i));
		chtype attrs = getattrs(stdscr);
		mvprintw(row++, col, "%d) Color test: 0x%x", i, attrs);

		if (row > 29)
		{
			row = 0;
			col += 32;
		}
	}

	doupdate();
	getch();
	attroff(A_COLOR);

	init_pair(255, COLOR_RED, 9);
	init_pair(0X4b, COLOR_RED, COLOR_BLUE);

	attron(COLOR_PAIR(255));
	chtype attrs = getattrs(stdscr);
	mvprintw(0, 0, "Color test: 0x%x", attrs);

	init_pair(256, COLOR_MAGENTA, COLOR_BLUE); //anything above 255 will be same as 255

	attron(COLOR_PAIR(256));
	attrs = getattrs(stdscr);
	mvprintw(1, 0, "Color test: 0x%x", attrs);

	doupdate();
	getch();

}



void colorStringTest()
{
	attron(COLOR_PAIR(COLOR_BLUE));
	mvprintw(1, 1, "The first string should be blue");
	attroff(COLOR_PAIR(COLOR_BLUE));
	mvprintw(2, 2, "The second string should be white");

	attron(COLOR_PAIR(COLOR_GREEN_BOLD));
	mvprintw(3, 3, "The third string should be green");
	attroff(COLOR_PAIR(COLOR_GREEN_BOLD));

	wnoutrefresh(stdscr);
	doupdate();

	int input = getch();


}


void invisibleAttrTest()
{
	int rows = 10;
	int cols = 10;
	WINDOW* win = newwin(rows, cols, 1, 1);
	int total = rows * cols;

	int smallRows = rows - 4;
	int smallCols = cols - 4;
	WINDOW* background = newwin(smallRows, smallCols, 2, 2);

	for (int i = 0; i < total; i++)
	{
		int row = i / cols;
		int col = i % cols;
		mvwaddch(win, row, col, 'R' | (COLOR_GREEN << TEXTCOLOR_OFFSET) | (COLOR_RED << BKGDCOLOR_OFFSET));
	}

	mvwaddch(background, 3, 3, 'R' | A_INVIS | (COLOR_GREEN << TEXTCOLOR_OFFSET) | (COLOR_RED << BKGDCOLOR_OFFSET));

	wnoutrefresh(win);
	wnoutrefresh(background);

	doupdate();

	int input = getch();
}

void clearOKTest()
{
	
	mvwaddch(stdscr, 1, 1, 'A');
	//clearok(stdscr, true);
	wrefresh(stdscr);
	//doupdate();
	getch();
	clear();
	mvwaddch(stdscr, 2, 2, 'B');
	//clearok(stdscr, true);
	wrefresh(stdscr);
	//doupdate();
	getch();
}