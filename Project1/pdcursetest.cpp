#include "TUI.h"
#include "CursesPanel.h"
#include "Menu.h"
#include "MapEditor.h"
#include "CursesAttributeController.h"
#include "RGBControl.h"
#include <list>
#include "ControlManager.h"
#include "Actor.h"
#include "Map.h"
#include "TextField.h"
#include "DataPkg.h"
#include <fstream>
#include "Palette.h"
#include "TextLabel.h"
#include "Frame.h"
#include "Table.h"
#include <sstream>
#include <dirent.h>
#include "FileChooser.h"
#include "ScrollBar.h"
#include "Highlighter.h"
#include <ctime>
#include "BWFilter.h"
#include "MapEffectFilterPattern.h"
#include "2DStorage.h"


void mockFightTest()
{
	short totalRows = 23;
	short totalCols = 51;
	resize_term(totalRows, totalCols);
	WINDOW* frame = newwin(totalRows, totalCols, 0, 0);
	string ruler = "1234567890123456789012345678901234567890123456789012345678901234567890";
	string charName = "Stephanopoulise"; //15 char name max length
	string hpStats = "HP 1000/2000"; //max length of HP
	string mpStats = "MP 1000/2000"; //max length of MP

	int partyIndent = 1;
	int enemyIndent = partyIndent + 16 + 5 + 4;
	int startRow = 1;

	mvwaddnstr(frame, 0, 0, ruler.c_str(), totalCols);
	for (int i = 0; i < 4; i++)
	{
		mvwaddstr(frame, startRow, partyIndent, charName.c_str());
		mvwaddstr(frame, startRow + 1, partyIndent, hpStats.c_str());
		mvwaddstr(frame, startRow + 2, partyIndent, mpStats.c_str());

		startRow += 4;
	}

	startRow = 1;
	for (int i = 0; i < 4; i++)
	{
		mvwaddstr(frame, startRow, enemyIndent, charName.c_str());
		mvwaddstr(frame, startRow + 1, enemyIndent, hpStats.c_str());
		mvwaddstr(frame, startRow + 2, enemyIndent, mpStats.c_str());

		startRow += 4;
	}

	mvwaddstr(frame, 1, partyIndent + 16, "<-");
	mvwaddstr(frame, 5, partyIndent + 16, "-1234");

	mvwaddstr(frame, 1, enemyIndent + 16, "<-");
	mvwaddstr(frame, 5, enemyIndent + 16, "+2000");

	WINDOW* subPanel = newwin(6, totalCols, totalRows - 6, 0);
	box(subPanel, 0, 0);

	wnoutrefresh(frame);
	wnoutrefresh(subPanel);
	doupdate();
	getch();
}





void characterTest()
{



}

void exploreTest()
{
	short totalRows = 23;
	short totalCols = 51;
	resize_term(totalRows, totalCols);
	WINDOW* frame = newwin(totalRows, totalCols, 0, 0);

	//box(frame, 0, 0);
	//                 012345678901234567890123456789012345678901234567890
	string rowType1 = "                       #...#                       ";
	string rowType2 = "########################...########################";
	string rowType3 = "...................................................";
	
	for (int row = 0; row < 9; row++)
	{
		mvwaddstr(frame, row, 0, rowType1.c_str());
	}

	mvwaddstr(frame, 9, 0, rowType2.c_str());
	mvwaddstr(frame, 10, 0, rowType3.c_str());
	mvwaddstr(frame, 11, 0, rowType3.c_str());
	mvwaddstr(frame, 12, 0, rowType2.c_str());

	for (int row = 13; row < totalRows; row++)
	{
		mvwaddstr(frame, row, 0, rowType1.c_str());
	}

	wnoutrefresh(frame);
	doupdate();
	getch();

	WINDOW* msgBox = newwin(6, totalCols, 0, 0);
	box(msgBox, 0, 0);

	int indent = 1;
	mvwaddstr(msgBox, 1, indent, "?????: This is an example of the type of place you may explore.");
	mvwaddstr(msgBox, 3, indent, "Alex: OK. Some more words here, blah blah lbah.");

	wnoutrefresh(msgBox);
	doupdate();
	getch();
}


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


MenuItem* menuDriver(int input, Menu* m)
{
	MenuItem* item = NULL;
	int retval = -1;
	switch (input)
	{
	case KEY_DOWN: retval = m->driver(REQ_DOWN_ITEM); break;
	case KEY_UP: retval = m->driver(REQ_UP_ITEM); break;
	case KEY_LEFT: retval = m->driver(REQ_LEFT_ITEM); break;
	case KEY_RIGHT: retval = m->driver(REQ_RIGHT_ITEM); break;
	case '\n':
	case '\r':
	case KEY_ENTER:
		m->driver(REQ_TOGGLE_ITEM); break;	
		break;
	default: break;
	}

	item = m->getSelectedItem();
	return item;
}

int confirmDriver(int input, Menu* m)
{
	int retval = -1;
	switch (input)
	{
	case KEY_DOWN: retval = m->driver(REQ_DOWN_ITEM); break;
	case KEY_UP: retval = m->driver(REQ_UP_ITEM); break;
	case KEY_LEFT: retval = m->driver(REQ_LEFT_ITEM); break;
	case KEY_RIGHT: retval = m->driver(REQ_RIGHT_ITEM); break;
	case '\n':
	case '\r':
	case KEY_ENTER:
		retval = m->getCrossRefIndex();
		break;
	default: break;
	}
	return retval;
}

void menuTest()
{
	int rows = 12;
	int cols = 1;
//	WINDOW* win = newwin(rows, cols * 18, 1, 1);
	WINDOW* win = newwin(4, (cols) * 18, 1, 1);

	Menu menu(win, rows, cols);
	menu.setItem("012345678901234", "", 0, 'N');
	menu.setItem("TOGGLE WRAP", "", 1, 'L');
	menu.setItem("QUIT", "", 2, 'Q');
	//menu.setItem("3123456789012345", "", 3, 'T');
	menu.setItem("4123456789012345", "", 4, '?');
	menu.setItem("Clear", "", 5, 'N');
	menu.setItem("6123456789012345", "", 6, 'L');
	menu.setItem("7123456789012345", "", 7, 'Q');
	menu.setItem("8123456789012345", "", 8, 'T');
	menu.setItem("9123456789012345", "", 9, '?');
	menu.setItem("a123456789012345", "", 10, 'N');
	menu.setItem("b123456789012345", "", 11, 'L');

	menu.disableItem(8, 0);
	menu.setMarkSide(RIGHT_MARK);
	bool wrap = true;

	bool usingMenu = true;
	while (usingMenu)
	{
		menu.draw();
		doupdate();
		int input = getch();

		MenuItem* item = menuDriver(input, &menu);

		if (item == NULL)
		{
			continue;
		}

		switch(item->index)
		{

		case 2:
			usingMenu = false;
			break;
		case 1:
			menu.setWrapAround(wrap = !wrap); break;
		case 5:
			menu.clear();
			break;
		default: 

			mvaddch(7, 30, (chtype)item->crossref);
			break;
		}
		item->itemChosen = false;
	}
}


void resizeTest()
{
	int x = getmaxx(stdscr);
	int y = getmaxy(stdscr);

	resize_term(y / 2, x / 2);

	doupdate();
	getch();
}

void mapEditorTest()
{
	/*Good lesson learned here. Always ensure that your terminal is sized to contain all windows that it renders or else window creation routines will return null*/
	resize_term(30, 150);


	MapEditor me;
	//me.loadMap();

	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	bool editing = true;
	while (editing) //simulate input/process/update loop
	{
		me.draw();

		doupdate();
		int wc = getch();

		editing = me.processInput(wc);
	}
}


//void mainMenuTest()
//{
//	short totalRows = 23;
//	short totalCols = 51;
//	resize_term(totalRows, totalCols);
//	init_pair(2, COLOR_WHITE, COLOR_BLUE);
//	
//	//setup character panel
//	WINDOW* frame = newwin(totalRows, 26, 0, 0);
//	WINDOW* charDisplay = derwin(frame, totalRows - 2, 24, 1, 1);
//
//	//setup main menu
//	int mainWidth = 11;
//
//	WINDOW* menuFrame = newwin(10, mainWidth, 0, totalCols - mainWidth);
//	WINDOW* menuWin = derwin(menuFrame, 8, 8, 1, 1);
//
//	Menu* main = new Menu(menuWin, 8, 1);
//
//	main->setColor(2);
//
//	main->setItem("Item", "", 0, 0);
//	main->setItem("Equip", "", 1, 1);
//	main->setItem("Status", "", 2, 2);
//	main->setItem("Skill", "", 3, 3);
//	main->setItem("Config", "", 4, 4);
//	main->setItem("Map", "", 5, 5);
//	main->setItem("Save", "", 6, 6);
//	main->setItem("Quit", "", 7, 7);
//
//	//create hidden confirmation window
//	int confirmFrameRows = 4;
//	int confirmFrameCols = 24;
//	WINDOW* confirmFrame = newwin(confirmFrameRows, confirmFrameCols, (totalRows - confirmFrameRows) / 2, (totalCols - confirmFrameCols) / 2);
//	string confirmMsg = "Quit - are you sure?";
//	WINDOW* confirmMenuWin = derwin(confirmFrame, 1, confirmFrameCols - 2, 2, 1);
//
//	Menu* confirmMenu = new Menu(confirmMenuWin, 1, 2);
//
//	confirmMenu->setColor(2);
//	confirmMenu->setMaxNameLength(6);
//	confirmMenu->setItem("No", "", 0, 2);
//	confirmMenu->setItem("Yes", "", 1, 3);
//	
//
//
//	bool inMenu = true;
//
//	bool displayConfirm = false;
//
//	while (inMenu)
//	{
//		//draw charDisplay
//		wattron(frame, COLOR_PAIR(2));
//		box(frame, 0, 0);
//		wbkgd(frame, ' ' | COLOR_PAIR(2));
//		wattron(charDisplay, COLOR_PAIR(2));
//		string fakeCharStr = "abcdeabcdeabcde";
//		string lvStr = "Level 14";
//		string hpStr = "HP 200/225";
//		string mpStr = "MP 45/50";
//		for (int i = 0; i < 4; i++)
//		{
//			int row = i * 5;
//			mvwaddstr(charDisplay, row, 0, fakeCharStr.c_str());
//			mvwaddstr(charDisplay, row + 1, 0, lvStr.c_str());
//			mvwaddstr(charDisplay, row + 2, 0, hpStr.c_str());
//			mvwaddstr(charDisplay, row + 3, 0, mpStr.c_str());
//		}
//
//		wnoutrefresh(frame);
//		wnoutrefresh(charDisplay);
//
//		//draw menu
//		box(menuFrame, 0, 0);
//		wnoutrefresh(menuFrame);
//		main->draw();
//
//		//if Confirmation Dialog is here then draw it
//		if(displayConfirm)
//		{
//			box(confirmFrame, 0, 0);
//			mvwaddstr(confirmFrame, 1, 1, confirmMsg.c_str());
//			wnoutrefresh(confirmFrame);
//			confirmMenu->draw();
//		}
//
//		doupdate();
//		int c = getch();
//
//		//handle input
//		MenuResponse* resp;
//		if (displayConfirm)
//		{
//			resp = menuDriver(c, confirmMenu);
//
//			if(resp->itemChosen)
//			{
//				switch (resp->crossref)
//				{
//				case 2: displayConfirm = false;  break; //no
//				case 3: inMenu = false; break; //yes
//				}
//			}
//		}
//		else
//		{
//			resp = menuDriver(c, main);
//			if (resp->itemChosen)
//			{
//				switch (resp->index)
//				{
//				case -2: inMenu = false; break;
//				case 7: displayConfirm = true;  break;
//				}
//			}
//		}
//
//		
//
//
//		
//	}
//
//	
//}


void testPanel()
{
	WINDOW* panelWin[3];
	PANEL* panel[3];
	init_pair(1, COLOR_WHITE | 0x8, COLOR_BLUE);

	panelWin[0] = newwin(5, 25, 0, 0);
	panelWin[1] = newwin(5, 25, 2, 16);
	panelWin[2] = newwin(5, 25, 3, 32);

	WINDOW* subWin = derwin(panelWin[0], 3, 23, 1, 1);

	wattron(panelWin[0], COLOR_PAIR(1));
	wattron(panelWin[1], COLOR_PAIR(1));
	wattron(panelWin[2], COLOR_PAIR(1));

	wbkgd(panelWin[0], COLOR_PAIR(1));
	wbkgd(panelWin[1], COLOR_PAIR(1));
	wbkgd(panelWin[2], COLOR_PAIR(1));

	box(panelWin[0], 0, 0);
	box(panelWin[1], 0, 0);
	box(panelWin[2], 0, 0);

	mvwaddch(panelWin[2], 1, 1, 'C');
	mvwaddch(panelWin[1], 1, 1, 'B');
	mvwaddch(subWin, 0, 0, 'A');
	
	
	mvwaddch(panelWin[2], 3, 23, 'C');
	mvwaddch(panelWin[1], 3, 23, 'B');
	mvwaddch(subWin, 2, 22, 'A');
	
	

	

	panel[0] = new_panel(panelWin[0]);
	panel[1] = new_panel(panelWin[1]);
	panel[2] = new_panel(panelWin[2]);
	//wnoutrefresh(subWin);
	update_panels();

	doupdate();
	getch();

	top_panel(panel[1]);
	//wnoutrefresh(subWin);
	update_panels();
	doupdate();
	getch();

	top_panel(panel[0]);
	//wnoutrefresh(subWin);
	update_panels();
	doupdate();
	getch();


}


//void basicMenuPanelTest()
//{
//	//set up menu
//	WINDOW* mainWin = newwin(4, 40, 3, 20);
//	WINDOW* subWin = derwin(mainWin, 1, 38, 2, 1);
//
//	Menu* menu = new Menu(mainWin, subWin, 1, 2);
//	menu->setItem("No", "", 0, 2);
//	menu->setItem("Yes", "", 1, 3);
//	mvwaddstr(mainWin, 1, 2, "Are you sure you want to quit?");
//
//	//setup second menu
//	WINDOW* mainWin2 = newwin(4, 40, 5, 22);
//	WINDOW* subWin2 = derwin(mainWin2, 1, 38, 2, 1);
//
//	Menu* menu2 = new Menu(mainWin2, subWin2, 1, 2);
//	menu2->setItem("No", "", 0, 2);
//	menu2->setItem("Yes", "", 1, 3);
//	mvwaddstr(mainWin2, 1, 2, "Are you really sure you want to quit?");
//
//	//setup third menu
//	WINDOW* mainWin3 = newwin(4, 40, 7, 24);
//	WINDOW* subWin3 = derwin(mainWin3, 1, 38, 2, 1);
//
//	Menu* menu3 = new Menu(mainWin3, subWin3, 1, 2);
//	menu3->setItem("No", "", 0, 2);
//	menu3->setItem("Yes", "", 1, 3);
//	mvwaddstr(mainWin3, 1, 2, "Are you really really sure you want to quit?");
//
//	Menu* mPtrs[3] = { menu, menu2,menu3 };
//
//	//set up panel
//	PANEL* panel = new_panel(mainWin);
//	PANEL* panel2 = new_panel(mainWin2);
//	PANEL* panel3 = new_panel(mainWin3);
//	hide_panel(panel2);
//	hide_panel(panel3);
//
//	PANEL* pPtrs[3] = { panel, panel2, panel3};
//
//	bool inMenu = true;
//
//	int state = 0;
//	menu->draw();
//	menu2->draw();
//	menu3->draw();
//
//	while (inMenu)
//	{
//		
//	/*	for (int i = state; i >= 0; i--)
//		{
//			mPtrs[state]->draw();
//		}*/
//		update_panels();
//		
//		//wnoutrefresh(mainWin);
//		//for (int i = state; i >= 0; i--)
//		//{
//		////	mPtrs[state]->draw();
//		//}
//		
//		doupdate();
//		int c = getch();
//
//		//handle input
//		int retval = confirmDriver(c, mPtrs[state]);
//		if (retval == 3) //yes to quitting
//		{
//			show_panel(pPtrs[++state]);
//		}
//		else if(retval == 2)
//		{
//			hide_panel(pPtrs[state--]);
//			//inMenu = false;
//		}
//			
//	}
//
//	del_panel(panel);
//	del_panel(panel2);
//	del_panel(panel3);
//
//	delwin(mainWin);
//	delwin(mainWin2);
//	delwin(mainWin3);
//
//	delwin(subWin);
//	delwin(subWin2);
//	delwin(subWin3);
//}

//void panelTest3()
//{
//	const int YES = 3;
//	const int NO = 2;
//
//	//setup first menu
//	WINDOW* win = newwin(4, 40, 1, 1);
//	WINDOW* menuWin = derwin(win, 1, 38, 2, 1);
//
//	NewMenu* menu = new NewMenu(menuWin, 1, 2);
//	menu->setItem("No", "", 0, NO);
//	menu->setItem("Yes", "", 1, YES);
//
//	//setup second menu
//	WINDOW* win2 = newwin(4, 40, 2, 10);
//	WINDOW* menuWin2 = derwin(win2, 1, 38, 2, 1);
//
//	NewMenu* menu2 = new NewMenu(menuWin2, 1, 2);
//	menu2->setItem("No", "", 0, NO);
//	menu2->setItem("Yes", "", 1, YES);
//
//
//	PANEL* pan = new_panel(win);
//	PANEL* pan2 = new_panel(win2);
//	hide_panel(pan2);
//
//	bool inMenu = true;
//	NewMenu* activeMenu = menu;
//	PANEL* topPanel = pan;
//	while (inMenu)
//	{
//		//all panels get the opportunity to be drawn if they are on the stack
//		if (panel_hidden(pan))
//		{
//			box(win, 0, 0);
//			mvwaddstr(win, 1, 2, "Are you sure you want to quit?");
//			menu->draw();
//		}
//
//		if (panel_hidden(pan2))
//		{
//			box(win2, 0, 0);
//			mvwaddstr(win2, 1, 2, "Are you really sure you want to quit?");
//			menu2->draw();
//		}
//		
//		update_panels(); //to display panels in proper stacking order
//		doupdate();
//		int c = getch();
//
//		//menu driver handles the activemenu (although this could be redone to use a different driver based on what the active menu is)
//		int retval = newMenuDriver(c, activeMenu);
//
//		if (retval == YES)
//		{
//			if (activeMenu == menu)
//			{
//				activeMenu = menu2;
//				show_panel(pan2);
//			}
//			else if (activeMenu == menu2)
//			{
//				inMenu = false;
//			}
//		}
//		else if (retval == NO)
//		{
//			if (activeMenu == menu)
//			{
//				inMenu = false;
//			}
//			else if (activeMenu == menu2)
//			{
//				activeMenu = menu;
//				hide_panel(pan2);
//			}
//		}
//		
//			
//
//
//
//	}
//	
//}


void RGBMenuDriver(Menu* menu, int input)
{
	switch (input)
	{
	case KEY_LEFT: break;
	case KEY_RIGHT:  break;
	case KEY_UP: menu->driver(REQ_UP_ITEM); break;
	case KEY_DOWN: menu->driver(REQ_DOWN_ITEM); break;
	default: break;
	}


	return;
}

void RGBTest()
{
	WINDOW* win = newwin(3, 3, 1, 3);
	Menu* rgbMenu = new Menu(win, 3, 1);

	rgbMenu->setItem("R", "", 0, 0);
	rgbMenu->setItem("G", "", 1, 1);
	rgbMenu->setItem("B", "", 2, 2);

	char* mark = new char[3];
	mark[0] = '*'; mark[1] = '*'; mark[2] = 0;

	bool redOn = false;
	bool greenOn = false;
	bool blueOn = false;

	bool inMenu = true;
	int offColumn = 7;
	int onColumn = 12;

	int currRow = 1;

	int color = 0;
	chtype c_red = COLOR_PAIR(COLOR_RED);
	chtype c_green = COLOR_PAIR(COLOR_GREEN);
	chtype c_blue = COLOR_PAIR(COLOR_BLUE);
	chtype attrs = 0x0; //combined attributes
	
	while (inMenu)
	{
		//draw stdscr
		clear();
		

		chtype x = getattrs(stdscr);

		mvaddstr(0, 7, "OFF  ON");
		
		int redColumn, greenColumn, blueColumn;
		redColumn = redOn ? onColumn : offColumn;
		greenColumn = greenOn ? onColumn : offColumn;
		blueColumn = blueOn ? onColumn : offColumn;

		mvaddstr(1, redColumn, mark);
		mvaddstr(2, greenColumn, mark);
		mvaddstr(3, blueColumn, mark);

		wnoutrefresh(stdscr);

		//draw menu
		rgbMenu->draw();
		doupdate();
		//input
		int c = getch();

		if (c == 'q')
			break;

		
		
		//process
		RGBMenuDriver(rgbMenu, c);
		currRow = rgbMenu->getCurrentIndex() + 1;

		chtype oneAttr = 0x0; //single attribute
		if (c == KEY_RIGHT)
		{
			switch (currRow)
			{
			case 1: 
				if (redOn == false)
				{
					redOn = true;
					oneAttr = c_red;
				}
				break;
			case 2: 
				if (greenOn == false)
				{
					greenOn = true;
					oneAttr = c_green;
				}
				break;
			case 3: 
				if (blueOn == false)
				{
					blueOn = true;
					oneAttr = c_blue;
				}
				break; 
			}
			attrs |= oneAttr;
			attron(attrs); //attron doesn't work right! I have to pass in all desired attributes and can't set one at a time
		}
		else if (c == KEY_LEFT)
		{
			switch (currRow)
			{
			case 1:
				if (redOn)
				{
					redOn = false;
					oneAttr = c_red;
				}
				break;
			case 2:
				if (greenOn)
				{
					greenOn = false;
					oneAttr = c_green;
				}
					break;
			case 3:
				if (blueOn)
				{
					blueOn = false;
					oneAttr = c_blue;
				}
					break;
			}
			attrs &= (oneAttr ^ 0xffffffff);
			attroff(oneAttr);
		}

	}


}

void RGBTest2()
{
	WINDOW* win = newwin(4, 3, 1, 3);
	Menu* rgbMenu = new Menu(win, 4, 1);

	rgbMenu->setItem("R", "", 0, 0);
	rgbMenu->setItem("G", "", 1, 1);
	rgbMenu->setItem("B", "", 2, 2);
	rgbMenu->setItem("!", "", 3, 3);

	char* mark = new char[3];
	mark[0] = '*'; mark[1] = '*'; mark[2] = 0;

	RGBControl control;

	bool inMenu = true;
	int offColumn = 7;
	int onColumn = 12;

	int currRow = 1;

	while (inMenu)
	{
		//do processing
		int redColumn = control.isColorOn(COLOR_RED) ? onColumn : offColumn;
		int greenColumn = control.isColorOn(COLOR_GREEN) ? onColumn : offColumn;
		int blueColumn = control.isColorOn(COLOR_BLUE) ? onColumn : offColumn;
		int boldColumn = control.isBoldOn() ? onColumn : offColumn;

		//draw stdscr
		clear();

		chtype colorAttrs = control.getColors();
		if (colorAttrs > A_BOLD) //at least 1 color is set
			attron(colorAttrs);
		else //no colors are set
			attroff(0xff00000); 

		if (control.isBoldOn())
			attron(A_BOLD);
		else
			attroff(A_BOLD);
		
		chtype y = getattrs(stdscr);

		mvaddstr(0, 7, "OFF  ON");
		mvaddstr(1, redColumn, mark);
		mvaddstr(2, greenColumn, mark);
		mvaddstr(3, blueColumn, mark);
		mvaddstr(4, boldColumn, mark);

		wnoutrefresh(stdscr);

		//draw menu
		rgbMenu->draw();
		doupdate();
		//input
		int c = getch();

		if (c == 'q')
			break;

		//process
		RGBMenuDriver(rgbMenu, c);
		currRow = rgbMenu->getCurrentIndex() + 1;

		bool position;
		int color;
		if (c == KEY_RIGHT)
		{
			position = ON_POSITION;
			switch (currRow)
			{
			case 1: color = COLOR_RED; break;
			case 2: color = COLOR_GREEN; break;
			case 3: color = COLOR_BLUE; break;
			case 4: color = A_BOLD; break;
			}
			control.pullSwitch(color, position);
		}
		else if (c == KEY_LEFT)
		{
			position = OFF_POSITION;
			switch (currRow)
			{
			case 1: color = COLOR_RED; break;
			case 2: color = COLOR_GREEN; break;
			case 3: color = COLOR_BLUE; break;
			case 4: color = A_BOLD; break;
			}
			control.pullSwitch(color, position);
		}
		
	}
}


void AttrControllerTest()
{
	WINDOW* win = newwin(4, 3, 1, 3);
	Menu* menu = new Menu(win, 4, 1);

	menu->setItem("R", "", 0, 0);
	menu->setItem("G", "", 1, 1);
	menu->setItem("B", "", 2, 2);
	menu->setItem("!", "", 3, 3);

	char* mark = new char[3];
	mark[0] = '*'; mark[1] = '*'; mark[2] = 0;

	CursesAttributeController control;

	bool inMenu = true;
	int offColumn = 7;
	int onColumn = 12;

	int currRow = 1;

	while (inMenu)
	{
		//do processing
		int redColumn = control.isColorStored(COLOR_RED) ? onColumn : offColumn;
		int greenColumn = control.isColorStored(COLOR_GREEN) ? onColumn : offColumn;
		int blueColumn = control.isColorStored(COLOR_BLUE) ? onColumn : offColumn;
		int boldColumn = control.isAttrStored(A_BOLD) ? onColumn : offColumn;

		//draw stdscr
		clear();

		//set colors
		chtype colorAttrs = control.getColors();
		if (control.getColors() > 0) //at least 1 color is set
			attron(colorAttrs);
		else //no colors are set
			attroff(A_COLOR);

		//set attributes
		chtype attrs = control.getAttrs();
		if (attrs > 0)
			attron(attrs);
		else
			attroff(A_ATTR_ONLY);

		chtype y = getattrs(stdscr);

		mvaddstr(0, 7, "OFF  ON");
		mvaddstr(1, redColumn, mark);
		mvaddstr(2, greenColumn, mark);
		mvaddstr(3, blueColumn, mark);
		mvaddstr(4, boldColumn, mark);

		wnoutrefresh(stdscr);

		//draw menu
		menu->draw();
		doupdate();
		//input
		int c = getch();

		if (c == 'q')
			break;

		//process
		RGBMenuDriver(menu, c);
		currRow = menu->getCurrentIndex() + 1;

		int color;
		if (c == KEY_RIGHT)
		{
			switch (currRow)
			{
			case 1: color = COLOR_RED; control.storeColor(color); break;
			case 2: color = COLOR_GREEN; control.storeColor(color); break;
			case 3: color = COLOR_BLUE; control.storeColor(color); break;
			case 4: color = A_BOLD; control.storeAttribute(A_BOLD); break;
			}
			
		}
		else if (c == KEY_LEFT)
		{			
			switch (currRow)
			{
			case 1: color = COLOR_RED; control.freeColor(color); break;
			case 2: color = COLOR_GREEN; control.freeColor(color); break;
			case 3: color = COLOR_BLUE; control.freeColor(color); break;
			case 4: color = A_BOLD; control.freeAttribute(A_BOLD); break;
			}
		}

	}
}

void newColorTest()
{
	char* text = "Here is text";

	int maxy = getmaxy(stdscr);

	
	
	for (int pair = 0; pair < 16; pair++)
	{
		chtype attr = 0x00010000;

		mvprintw(0, 0, "Color Pair: 0x%x", pair);
		for (int i = 0; i < 16; i++)
		{
			chtype color = COLOR_PAIR(pair);
			chtype c = attr | color;
			attron(c);
			mvprintw(i + 1, 0, "%d) %s: 0x%x", i, text, c);
			attroff(c);
			attr <<= 1;
			//attr &= 0xfffffffe;
		}
		refresh();
		doupdate();
		getch();
	}
	
	//reverse colors
	init_pair(1, COLOR_BLACK, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_BLACK, COLOR_CYAN);
	init_pair(4, COLOR_BLACK, COLOR_RED);
	init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(6, COLOR_BLACK, COLOR_YELLOW);
	init_pair(7, COLOR_BLACK, COLOR_WHITE); //identical to no color pair used

	for (int i = 8; i < COLORS; i++)
	{
		init_pair(i, COLOR_BLACK, i);
	}

	for (int pair = 0; pair < 16; pair++)
	{
		chtype attr = 0x00010000;

		mvprintw(0, 0, "Color Pair: 0x%x", pair);
		for (int i = 0; i < 16; i++)
		{
			chtype color = COLOR_PAIR(pair);
			chtype c = attr | color;
			attron(c);
			mvprintw(i + 1, 0, "%d) %s: 0x%x", i, text, c);
			attroff(c);
			attr <<= 1;
			//attr &= 0xfffffffe;
		}
		refresh();
		doupdate();
		getch();
	}

	//mix colors
	init_pair(1, COLOR_GREEN, COLOR_BLUE);
	init_pair(2, COLOR_RED, COLOR_GREEN);
	init_pair(3, COLOR_YELLOW, COLOR_CYAN);
	init_pair(4, COLOR_MAGENTA, COLOR_RED);
	init_pair(5, COLOR_BLUE, COLOR_MAGENTA);
	init_pair(6, COLOR_CYAN, COLOR_YELLOW);
	init_pair(7, COLOR_YELLOW, COLOR_WHITE); //identical to no color pair used

	for (int i = 8; i < COLORS; i++)
	{
		init_pair(i, COLOR_RED, i);
	}

	for (int pair = 0; pair < 16; pair++)
	{
		chtype attr = 0x00010000;

		mvprintw(0, 0, "Color Pair: 0x%x", pair);
		for (int i = 0; i < 16; i++)
		{
			chtype color = COLOR_PAIR(pair);
			chtype c = attr | color;
			attron(c);
			mvprintw(i + 1, 0, "%d) %s: 0x%x", i, text, c);
			attroff(c);
			attr <<= 1;
			//attr &= 0xfffffffe;
		}
		refresh();
		doupdate();
		getch();
	}
}


void attrTest()
{
	chtype red = 0x04000000;
	chtype green = 0x02000000;
	attron(red | A_BOLD); //COLOR_PAIR(COLOR_RED));
	mvaddstr(0, 0, "dsaljksdfl");
	doupdate();
	getch();

	attron(green); // COLOR_PAIR(COLOR_GREEN));
	mvaddstr(1, 0, "DSLDSL");

	doupdate();
	getch();

	attron(green | red); // COLOR_PAIR(COLOR_GREEN));
	mvaddstr(2, 0, "DSLDSL");

	doupdate();
	getch();
}

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

void colorPairTest()
{
	int pair = 0;
	for (int bgkd = 0; bgkd < 16; bgkd++)
	{
		for (int text = 0; text < 16; text++)
		{
			init_pair(pair++, text, bgkd);
		}
	}

	int row = 0;
	for (int i = 0; i < 256; i++)
	{
		attron(COLOR_PAIR(i));
		chtype attrs = getattrs(stdscr);
		mvprintw(row++, 0, "%d) Color test: 0x%x", i, attrs);

		if (row % 16 == 0)
		{
			row = 0;
			doupdate();
			getch();
		}
	
	}
}

void modalCallback(void* caller, void* ptr, int input)
{
	Frame* f = (Frame*)ptr;
	Menu* m = (Menu*)f->getControl();

	switch (input)
	{
	case KEY_LEFT: m->driver(REQ_LEFT_ITEM);  break;
	case KEY_RIGHT: m->driver(REQ_RIGHT_ITEM);  break;
		//case KEY_UP: m->driver(REQ_UP_ITEM);  break;
		//case KEY_DOWN: m->driver(REQ_DOWN_ITEM);  break;
	case '\r': m->driver(REQ_TOGGLE_ITEM); break;
	}

	MenuItem* item = m->getSelectedItem();
	if (item != NULL)
	{
		ControlManager* cm = f->getControlManager();
		if (item->index == 0) //YES
		{
			//exit(0); //works, but probably not good to use in a game with multiple states
			cm->prepareForShutdown();
		}
		else if (item->index == 1) //no remove modal window
		{
			//cm = m->getControlManager();
			cm->popControl();
		}
	}

}


void callBackTest(void* caller, void* ptr, int input)
{
	Menu* m = (Menu*)ptr;

	switch (input)
	{
	case KEY_LEFT: m->driver(REQ_LEFT_ITEM);  break;
	case KEY_RIGHT: m->driver(REQ_RIGHT_ITEM);  break;
	case KEY_UP: m->driver(REQ_UP_ITEM);  break;
	case KEY_DOWN: m->driver(REQ_DOWN_ITEM);  break;
	case '\r': m->driver(REQ_TOGGLE_ITEM); break;
	}

	MenuItem* item = m->getSelectedItem();
	if (item != NULL)
	{
		if (item->index == 0) //YES
		{
			//create modal menu

			WINDOW* win = newwin(4, 40, 6, 40);
			WINDOW* dWin = derwin(win, 1, 38, 2, 1);
			Menu* modal = new Menu(dWin, 1, 2);
			modal->setModal(true);
			modal->setItem("Yes", "", 0, 0);
			modal->setItem("No", "", 1, 1);
			
			Frame* f = new Frame(win, modal);
			f->setText("Are you sure you want to quit?", 1, 1);
			ControlManager* cm = m->getControlManager();
			cm->registerControl(f, KEY_LISTENER, modalCallback);
			cm->setFocus(f);
			
		}
		item->itemChosen = false;
	}

}



void callBackTest2(void* caller, void* ptr, int input)
{
	Palette* p = (Palette*)ptr;
	int colorY, colorX;
	MEVENT event;
	nc_getmouse(&event);
	p->translateCoords(event.y, event.x, colorY, colorX);
	p->pickItem(colorY, colorX);
}

void newCallback(void* caller, void* ptr, int input)
{
	mvaddstr(25, 25, "new callback called");
}

void quitCallback(void* caller, void* ptr, int input)
{
	ControlManager* cm = (ControlManager*)ptr;
	cm->prepareForShutdown();
}

void controlManagerTest()
{
	WINDOW* win = newwin(2, 20, 1, 1);
	Menu* m1 = new Menu(win, 2, 1);

	m1->setItem("Yes", "", 0, 0);
	m1->setItem("No", "", 1, 1);

	int rows;
	int cols;
	rows = cols = 4;
	Palette* palette = new Palette(rows, cols, 7, 1);
	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		chtype c = ' ' | (i << 28) & A_COLOR;
		int x = i % cols;
		int y = i / cols;
		palette->setItem(TUI::colorNames[i], c, y, x);
	}


	ControlManager* cm = new ControlManager(NULL);
	cm->registerControl(m1, MOUSE_LISTENER | KEY_LISTENER, callBackTest);
	cm->registerControl(palette, MOUSE_LISTENER, callBackTest2);
	cm->registerShortcutKey(CTRL_N, newCallback);
	cm->registerShortcutKey(CTRL_Q, quitCallback);

	bool inMenus = true;
	while (inMenus)
	{
		clear();
		wnoutrefresh(stdscr);
		cm->draw();
		
		
		doupdate();
		int c = getch();

		inMenus = cm->handleInput(c);
	}

}


void commandTest()
{
	WINDOW* win = newwin(2, 20, 1, 1);
	Menu* m1 = new Menu(win, 2, 1);

	m1->setItem("Yes", "", 0, 0);
	m1->setItem("No", "", 1, 1);

	int rows;
	int cols;
	rows = cols = 4;
	Palette* palette = new Palette(rows, cols, 7, 1);
	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		chtype c = ' ' | (i << 28) & A_COLOR;
		int x = i % cols;
		int y = i / cols;
		palette->setItem(TUI::colorNames[i], c, y, x);
	}


	ControlManager* cm = new ControlManager(NULL);
	cm->registerControl(m1, MOUSE_LISTENER | KEY_LISTENER, callBackTest);
	cm->registerControl(palette, MOUSE_LISTENER, callBackTest2);
	cm->registerShortcutKey(CTRL_N, newCallback);
	cm->registerShortcutKey(CTRL_Q, quitCallback);

	bool inMenus = true;
	while (inMenus)
	{
		clear();
		wnoutrefresh(stdscr);
		cm->draw();


		doupdate();
		int c = getch();

		inMenus = cm->handleInput(c);
	}

}



void templateTest()
{

	list<Menu*> aList;



	WINDOW* win = newwin(2, 20, 1, 1);
	Menu* m1 = new Menu(win, 2, 1);

	m1->setItem("Item A", "", 0, 5);
	m1->setItem("Item B", "", 1, 7);

	aList.push_back(m1);

	int x = 3;
}


void mouseTest()
{
	MEVENT event;
	mousemask(ALL_MOUSE_EVENTS, NULL);


	bool a = true;
	while (a)
	{
		int c = getch();
		switch (c)
		{
		case KEY_MOUSE:

			nc_getmouse(&event);
			if (event.bstate == BUTTON1_CLICKED)
			{
				mvaddstr(event.y, event.x, "BUTTON1_CLICKED"); //left click
			}
			else if (event.bstate == BUTTON2_CLICKED)
			{
				mvaddstr(event.y, event.x, "BUTTON2_CLICKED"); //wheel click
			}
			else if (event.bstate == BUTTON3_CLICKED)
			{
				mvaddstr(event.y, event.x, "BUTTON3_CLICKED"); //right click
			}

			//mvaddstr(0, 0, "Mouse");
			break;
		default: 
			mvaddstr(0, 0, "Keyboard");
			break;
		}

		doupdate();
	}
	
}


void colorPaletteTest()
{
	//setup color palette
	/*int rows;
	int cols;
	rows = cols = 4;
	Palette palette(rows, cols, 1, 1);
	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		chtype c = ' ' | (i << 28) & A_COLOR;
		int x = i % cols;
		int y = i / cols;
		palette.setItem(TUI::colorNames[i], c, y, x);
	}

	MEVENT event;
	mousemask(ALL_MOUSE_EVENTS, NULL);

	bool in = true;
	while (in)
	{
		palette.draw();

		doupdate();

		int c = getch();

		switch (c)
		{
		case KEY_MOUSE:
			nc_getmouse(&event);
			if (event.bstate == BUTTON1_CLICKED)
			{
				if (MouseHelper::isInWindow(palette.getWindow(), event.y, event.x))
				{
					int colorY, colorX;
					MouseHelper::translateCoords(palette.getWindow(), event.y, event.x, colorY, colorX);

					PaletteItem* item = palette.pickItem(colorY, colorX);
					chtype color = item->icon;
					mvaddch(7, 7, 'A' | color);
				}


			}
			break;
		}
		wnoutrefresh(stdscr);
	}*/
}


void filterPaletteTest()
{
	//setup color palette
	/*int rows = 2;
	int cols = 4;
	
	Palette palette(rows, cols, 1, 1);
	
	palette.setItem("Obstr", 'O', 0, 0);
	palette.setItem("Jumpable", 'J', 0, 1);
	palette.setItem("Const Dmg", 'd', 0, 2);
	palette.setItem("Inc Dmg", 'D', 0, 3);
	palette.setItem("Ailment", 'A', 1, 0);
	palette.setItem("Save", 'S', 1, 1);
	palette.setItem("Exit", 'E', 1, 2);
	


	MEVENT event;
	mousemask(ALL_MOUSE_EVENTS, NULL);

	bool in = true;
	while (in)
	{
		palette.draw();

		doupdate();

		int c = getch();

		switch (c)
		{
		case KEY_MOUSE:
			nc_getmouse(&event);
			if (event.bstate == BUTTON1_CLICKED)
			{
				if (MouseHelper::isInWindow(palette.getWindow(), event.y, event.x))
				{
					int toolY, toolX;
					MouseHelper::translateCoords(palette.getWindow(), event.y, event.x, toolY, toolX);

					palette.pickItem(toolY, toolX);
				}


			}
			break;
		}
		wnoutrefresh(stdscr);
	}*/
}


Actor* createActor()
{
	ActorDef* def = new ActorDef();
	def->name = "Test guy";
	def->symbol = (chtype) '@' | 0x0e000000;
	def->level = 1;
	def->exp = 0;
	def->gold = 50;
	def->maxHp = 25;
	def->maxMp = 10;
	def->strength = 8;
	def->defense = 5;
	def->agility = 4;
	def->accuracy = .9f;
	def->luck = .05f;

	Actor* actor = new Actor();
	actor->def = def;
	actor->currHp = def->maxHp;
	actor->defIndex = 1;
	actor->prevX = -1;
	actor->prevY = -1;
	actor->x = 30;
	actor->y = 5;
	actor->type = 0; //not used yet
	return actor;
}

Actor* createNPCActor()
{
	ActorDef* def = new ActorDef();
	def->name = "NPC1";
	def->symbol = (chtype) '&' | 0x0e000000;
	def->level = 5;
	def->exp = 0;
	def->gold = 0;
	def->maxHp = 1;
	def->maxMp = 1;
	def->strength = 1;
	def->defense = 1;
	def->agility = 1;
	def->accuracy = .9f;
	def->luck = .05f;

	Actor* actor = new Actor();
	actor->def = def;
	actor->currHp = def->maxHp;
	actor->defIndex = 1;
	actor->prevX = -1;
	actor->prevY = -1;
	actor->x = 40;
	actor->y = 4;
	actor->type = 0; //not used yet
	return actor;
}

Actor* createEnemyActor()
{
	ActorDef* def = new ActorDef();
	def->name = "Toadie";
	def->symbol = (chtype) 't' | (COLOR_GREEN << 24);
	def->level = 1;
	def->exp = 0;
	def->gold = 3;
	def->maxHp = 1;
	def->maxMp = 0;
	def->strength = 1;
	def->defense = 1;
	def->agility = 2;
	def->accuracy = .9f;
	def->luck = .01f;

	Actor* actor = new Actor();
	actor->def = def;
	actor->currHp = def->maxHp;
	actor->defIndex = 2;
	actor->prevX = -1;
	actor->prevY = -1;
	actor->x = 40;
	actor->y = 5;
	actor->type = 2; //not used yet
	return actor;
}

/*Checks for directional key by checking hex values*/
bool isDirKey(int input)
{
	return (input >= KEY_DOWN && input <= KEY_RIGHT);
}



void simpleActorTest()
{
	Actor* player1 = createActor();
	Actor* testNPC = createNPCActor();

	bool playing = true;
	string* dialogue = NULL;
	while (playing)
	{
		clear();
		mvaddch(player1->y, player1->x, player1->def->symbol);
		mvaddch(testNPC->y, testNPC->x, testNPC->def->symbol);

		if (dialogue != NULL)
		{
			mvaddstr(1, 1, "Hey");
		}

		refresh();
		doupdate();
		int c = getch();

		if (isDirKey(c))
		{
			short* axis = NULL;
			short stepAmt;
			switch (c)
			{
			case KEY_UP: 
				axis = &player1->y;
				stepAmt = -1;
				break;
			case KEY_DOWN:
				axis = &player1->y; 
				stepAmt = 1;
				break;
			case KEY_LEFT: 
				axis = &player1->x;
				stepAmt = -1; 
				break;
			case KEY_RIGHT: 
				axis = &player1->x;
				stepAmt = 1;
				break;
			default: playing = false;  break;
			}

			*axis += stepAmt;

			if (player1->y == testNPC->y && player1->x == testNPC->x)
			{
				*axis -= stepAmt; //reverse the step 
				//setup dialogue
				dialogue = new string("Hey");
			}
			else
				dialogue = NULL;


		}
		

	}

}

void simpleFightTest()
{
	//setup main window
	short totalRows = 23;
	short totalCols = 51;
	resize_term(totalRows, totalCols);
	WINDOW* frame = newwin(totalRows, totalCols, 0, 0);
	string ruler = "1234567890123456789012345678901234567890123456789012345678901234567890";
	char* hpDispTemplate = "HP %+4u/%-4u";

	
	//setup actors
	Actor* p1 = createActor();
	Actor* e1 = createEnemyActor();

	//setup player display
	TextLabel* playerName = new TextLabel(newwin(1, 15, 0, 0), p1->def->name);
	
	ostringstream oss; 
	oss << "HP " << p1->currHp << '/' << p1->def->maxHp;
	TextLabel* playerHP = new TextLabel(newwin(1, 15, 0, 0), oss.str());
	
	Controllable** pCells = new Controllable*[3];
	pCells[0] = playerName;
	pCells[1] = playerHP;
	pCells[2] = NULL;

	Table* playerStats = new Table(3, 1, 0, 0, pCells);

	//setup enemy display
	TextLabel* enemyName = new TextLabel(newwin(1, 15, 0, 0), e1->def->name);
	
	ostringstream oss2;
	oss2 << "HP " << e1->currHp << '/' << e1->def->maxHp;
	TextLabel* enemyHP = new TextLabel(newwin(1, 15, 0, 0), oss2.str());

	Controllable** eCells = new Controllable*[3];
	eCells[0] = enemyName;
	eCells[1] = enemyHP;
	eCells[2] = NULL;

	Table* enemyStats = new Table(3, 1, 0, 0, eCells);

	//set up final table
	Controllable** cells = new Controllable*[8];
	for (int i = 0; i < 8; i++)
	{
		cells[i] = NULL;
	}
	cells[2] = playerStats;
	cells[3] = enemyStats;

	Table* fightGrid = new Table(4, 2, 1, 1, cells);
	for (int i = 0; i < 4; i++)
	{
		fightGrid->setRowHeight(i, 4);
	}
	for (int i = 0; i < 2; i++)
	{
		fightGrid->setColWidth(i, 25);
	}
	

	//build display window
	WINDOW* subPanel = newwin(6, totalCols, totalRows - 6, 0);
	WINDOW* subWin = derwin(subPanel, 2, 18, 1, 1);

	Menu* fightMenu = new Menu(subWin, 2, 1);
	Frame* mFrame = new Frame(subPanel, fightMenu);

	fightMenu->setItem("Attack", "", 0, 0);
	fightMenu->setItem("Run", "", 1, 1);


	//setup additional vars
	int startRow = 1;
	short damage;

	bool fighting = true;
	while (fighting)
	{
		//draw
		//render
		startRow = 1;
		//wclear(frame);
		mvwaddnstr(frame, 0, 0, ruler.c_str(), totalCols);

		box(subPanel, 0, 0);

		wnoutrefresh(frame);
		fightGrid->draw();

		wnoutrefresh(subPanel);
		fightMenu->draw();
		doupdate();



		//get input
		int c = getch();

		//process
		int retval;
		switch (c)
		{
		case '\n':
		case '\r':
		case KEY_ENTER:
			retval = fightMenu->driver(c);
			if (retval == 0) //attack chosen
			{
				damage = p1->def->strength;
				e1->currHp -= damage;
				if (e1->currHp < 0)
					e1->currHp = 0;

				ostringstream oss2;
				oss2 << "HP " << e1->currHp << '/' << e1->def->maxHp;
				enemyHP->setText(oss2.str());
			}
			else if (retval == 1)
			{
				fighting = false; //break out
			}

			break;
		case KEY_DOWN: fightMenu->driver(REQ_DOWN_ITEM); break;
		case KEY_UP: fightMenu->driver(REQ_UP_ITEM); break;

		}

	}

}

void realMapTest()
{
	/*int rows = 8;
	int cols = 8;

	WINDOW* frame = newwin(rows + 2, cols + 2, 1, 1);
	WINDOW* win = derwin(frame, rows, cols, 1, 1);

	Map m("A map", rows, cols, win);

	int y = 0;
	int x = 0;
	m.setPosition(y, x);

	bool playing = true;

	while (playing)
	{
		box(frame, 0, 0);
		wnoutrefresh(frame);
		m.draw();
		doupdate();
		int c = getch();

		switch (c)
		{
		case KEY_UP: y--;  break;
		case KEY_DOWN: y++; break;
		case KEY_LEFT: x--; break;
		case KEY_RIGHT: x++; break;
		default: playing = false;
		}
		m.setPosition(y, x);
	}

	*/
}

void textFieldtest()
{
	WINDOW* textWin = newwin(1, 15, 1, 1);
	TextField field(textWin);
	field.setText("Default text");
	
	bool playing = true;
	while (playing)
	{
		curs_set(0);
		field.draw();
		field.setFocus();
		doupdate();
		int c = getch();

		field.inputChar(c);
	}

}

bool saveActorDef(string fileName, ActorDef* def)
{
	ofstream file;

	file.open(fileName, ios::trunc | ios::binary);
	if (file.is_open() == false)
		return false;

	//save only the necessary pieces of data
	file.write((char*)def->name.c_str(), def->name.length());
	file.write((char*)&def->symbol, sizeof(int));
	file.write((char*)&def->level, sizeof(short));
	file.write((char*)&def->exp, sizeof(int));
	file.write((char*)&def->gold, sizeof(int));
	file.write((char*)&def->maxHp, sizeof(int));
	file.write((char*)&def->maxMp, sizeof(int));
	file.write((char*)&def->strength, sizeof(short));
	file.write((char*)&def->defense, sizeof(short));
	file.write((char*)&def->agility, sizeof(short));
	file.write((char*)&def->accuracy, sizeof(int));
	file.write((char*)&def->luck, sizeof(int));

	//file.write((char*)dataBuf, size);
	file.close();

	return true;
}


void dataPkgTest()
{
	DataPkg pkg;

	ActorDef* def = new ActorDef();// (ActorDef*) pkg.create(sizeof(ActorDef));
	def->name = "P1Test";
	def->symbol = (chtype) '@' | 0x0e000000;
	def->level = 1;
	def->exp = 0;
	def->gold = 50;
	def->maxHp = 25;
	def->maxMp = 10;
	def->strength = 8;
	def->defense = 5;
	def->agility = 4;
	def->accuracy = .9f;
	def->luck = .05f;
	
//	pkg.save("Testpkg.bin");

	//pkg.free();

	saveActorDef("Actor.bin", def);
}

void textLabelTest()
{
	WINDOW* win = newwin(1, 10, 2, 2);
	TextLabel* label = new TextLabel(win, "12345678901");

	label->draw();
	doupdate();
	getch();
}

Menu* m1driver(Menu* m, int input) //simulated callback for menu
{
	WINDOW* win2 = newwin(1, 40, 1, 1);
	Menu* m2 = new Menu(win2, 1, 2);

	m2->setItem("Yes2", "", 0, 0);
	m2->setItem("No2", "", 1, 1);
	return m2;
}

void m2driver(Menu* m, int input) //simulated callback for menu
{


}


void menuTest2()
{
	WINDOW* win1 = newwin(1, 40, 1, 1);
	Menu* m1 = new Menu(win1, 1, 2);

	m1->setItem("Yes", "", 0, 0);
	m1->setItem("No", "", 1, 1);

	Menu* menus[2];
	menus[0] = m1;
	int totalMenus = 1;
	Menu* activeMenu = m1;

	bool playing = true;

	while (playing)
	{
		for (int i = 0; i < totalMenus; i++)
		{
			menus[i]->draw(); //draw all potential objects
		}
		doupdate();
		int c = getch();

		if (activeMenu == m1) //this section should be the callback! We call a specific routine based on what the Controllable object is
		{
			switch (c)
			{
			case KEY_LEFT: activeMenu->driver(REQ_LEFT_ITEM); break;
			case KEY_RIGHT: activeMenu->driver(REQ_RIGHT_ITEM); break;
			case '\r': activeMenu->driver(REQ_TOGGLE_ITEM); break;
			}
			
			MenuItem* mi = activeMenu->getSelectedItem();
			if(mi != NULL)
			{
				switch (mi->index)
				{
				case 0: //create pop-up window
					activeMenu = m1driver(activeMenu, c);
					menus[1] = activeMenu;
					totalMenus++;
					mi->itemChosen = false; 
					break;
				case 1: playing = false; //quit on no
					break;
				}
			}
			
		}
		else //callback for second method
		{
			switch (c)
			{
			case KEY_LEFT: activeMenu->driver(REQ_LEFT_ITEM); break;
			case KEY_RIGHT: activeMenu->driver(REQ_RIGHT_ITEM); break;
			case '\r': activeMenu->driver(REQ_TOGGLE_ITEM); break;
			}

			MenuItem* mi = activeMenu->getSelectedItem();
			if (mi != NULL)
			{
				switch (mi->index)
				{
				case 0: //quit on yes
					playing = false;
					break;
				case 1:   //backout on no (the callback that handles this portion can deallocate the memory)
					delete activeMenu;
					activeMenu = menus[0];
					totalMenus--;
					break;
				}			
			}
		}

		
	}


}

void frameTest()
{
	WINDOW* w = newwin(4, 40, 1, 1);
	WINDOW* dw = derwin(w, 1, 38, 2, 1);
	Menu* m = new Menu(dw, 1, 2);
	m->setItem("Yes", "", 0, 0);
	m->setItem("No", "", 1, 1);

	Frame* f = new Frame(w, m);
	mvwaddstr(w, 1, 1, "Are you sure you want to quit?");

	f->draw();
	doupdate();
	getch();

	f->setBorder(BORDER_NONE);
	f->draw();
	doupdate();
	getch();
}

/*
Deprecated test method that uses the incomplete <filesystem> header
*/
//void openFileTest()
//{
//	path fullPath = current_path();
//	fullPath.make_preferred();
//	
//	string openPathStr = fullPath.string();
//
//	TextLabel* openFile = new TextLabel(newwin(1, 10, 0, 0), "Open from:");
//
//	int openPathSpace = 30;
//	TextLabel* openPath = new TextLabel(newwin(1, openPathSpace, 0, 11), openPathStr);
//	
//
//	Menu* diskNavigator = new Menu(newwin(8, 40, 2, 0), 255, 1);
//	diskNavigator->setMaxNameLength(40);
//	
//	bool playing = true;
//	bool newDir = true;
//
//	while (playing)
//	{
//		if (newDir)
//		{
//			diskNavigator->clear();
//			diskNavigator->setItem("..", "", 0, 0);
//
//			//get list of files in current dir to display in Menu
//			int currElement = 1;
//			for (directory_iterator it(fullPath), end; it != end; it++)
//			{
//				directory_entry entry = *it;
//				path entryPath = entry.path();
//				diskNavigator->setItem(entryPath.filename().string(), "", currElement++, currElement);
//			}
//			
//			//truncate path name so we can see as much of the end as possible
//			openPathStr = fullPath.string();
//			if (openPathStr.length() > openPathSpace)
//			{
//				ostringstream oss;
//				oss << "..." << openPathStr.substr(openPathStr.length() - openPathSpace + 3);
//				openPathStr = oss.str();
//			}
//			openPath->setText(openPathStr);
//
//			newDir = false;
//		}
//	
//		
//		mvaddnstr(1, 0, "--------------------------------------------------------------------", 40);
//
//
//		openFile->draw();
//		openPath->draw();
//		diskNavigator->draw();
//		doupdate();
//		int c = getch();
//
//		switch (c)
//		{
//		case KEY_DOWN: diskNavigator->driver(REQ_DOWN_ITEM);   break;
//		case KEY_UP: diskNavigator->driver(REQ_UP_ITEM); break;
//		case 'q': playing = false; break;
//		case '\r':
//			diskNavigator->driver(REQ_TOGGLE_ITEM);
//			break;
//		}
//
//		MenuItem* choice = diskNavigator->getSelectedItem();
//
//		if (choice != NULL)
//		{
//			if (choice->name.compare("..") == 0)
//			{
//				//navigate up one dir
//				fullPath = fullPath.parent_path();
//				newDir = true;
//			}
//			else
//			{
//			/*	ostringstream oss;
//				string name = choice->name;
//				oss << fullPath << '\\' << name << ends;
//
//*/
//
//				//directory_entry entry(oss.str());
//				directory_entry entry(fullPath /= choice->name);
//				file_status fs = entry.status();
//				
//				if (is_directory(fs))
//				{
//					fullPath.append(choice->name);
//					newDir = true;
//				}
//				
//			}
//
//
//		}
//	}
//}

void openFileTest()
{
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	string fullPath(buf);
	
	TextLabel* openFile = new TextLabel(newwin(1, 10, 0, 0), "Open from:");

	int openPathSpace = 30;
	TextLabel* openPath = new TextLabel(newwin(1, openPathSpace, 0, 11), fullPath);


	Menu* diskNavigator = new Menu(newwin(8, 40, 2, 0), 255, 1);
	diskNavigator->setMaxNameLength(40);

	bool playing = true;
	bool newDir = true;

	while (playing)
	{
		if (newDir)
		{
			diskNavigator->clear();

			//get list of files in current dir to display in Menu
			int currElement = 0;

			DIR* dir = opendir(fullPath.c_str());
			dirent* entry;
			while ((entry = readdir(dir)) != NULL)
			{				
				diskNavigator->setItem(entry->d_name, "", currElement++, entry->d_type);
			}


			//truncate path name so we can see as much of the end as possible
			string openPathStr;
			openPathStr.assign(fullPath);

			if (openPathStr.length() > openPathSpace)
			{
				ostringstream oss;
				oss << "..." << openPathStr.substr(openPathStr.length() - openPathSpace + 3);
				openPathStr = oss.str();
			}
			openPath->setText(openPathStr);

			newDir = false;
		}


		mvaddnstr(1, 0, "--------------------------------------------------------------------", 40);


		openFile->draw();
		openPath->draw();
		diskNavigator->draw();
		doupdate();
		int c = getch();

		switch (c)
		{
		case KEY_DOWN: diskNavigator->driver(REQ_DOWN_ITEM);   break;
		case KEY_UP: diskNavigator->driver(REQ_UP_ITEM); break;
		case 'q': playing = false; break;
		case '\r':
			diskNavigator->driver(REQ_TOGGLE_ITEM);
			break;
		}

		MenuItem* choice = diskNavigator->getSelectedItem();

		if (choice != NULL)
		{
			if (choice->name.compare("..") == 0)
			{
				//navigate up one dir
				int pos = fullPath.find_last_of('\\');
				fullPath = fullPath.substr(0, pos);
				newDir = true;
			}
			else //check if directory was chosen
			{
				if (choice->crossref == DT_DIR) //dir was chosen
				{
					fullPath.append(1, '\\');
					fullPath.append(choice->name);
					newDir = true;
				}
			}
		}
	}
}

void wdirectoryTest()
{
	DIR* dir = opendir(".");
	
	//DIR* dir = opendir("F:\\Users\\Alex Mercer\\Documents\\Visual Studio 2015\\Projects\\Project1\\Project1");
	WDIR* wDir = dir->wdirp;
	
	mvaddwstr(1, 1, wDir->patt);
	
	wnoutrefresh(stdscr);
	doupdate();
	getch();

	wdirent* entry;
	while ((entry = wreaddir(wDir)) != NULL)
	{
		clear();
		mvaddwstr(1, 1, entry->d_name);
		wnoutrefresh(stdscr);
		doupdate();
		getch();
	}
	


}

void directoryTest()
{
	DIR* dir = opendir(".");

	//DIR* dir = opendir("F:\\Users\\Alex Mercer\\Documents\\Visual Studio 2015\\Projects\\Project1\\Project1");

	dirent* entry;

	char buf[256];
	
	while ((entry = readdir(dir)) != NULL)
	{
		clear();
		mvaddstr(1, 1, entry->d_name);
		wnoutrefresh(stdscr);
		doupdate();
		getch();
	}
}

void tableTest()
{
	//make subtable
	TextLabel* t1 = new TextLabel(newwin(1, 10, 0, 0), "Hero");
	TextLabel* t2 = new TextLabel(newwin(1, 10, 0, 0), "HP 0/255");
	TextLabel* t3 = new TextLabel(newwin(1, 10, 0, 0), "MP 20/25");
	Controllable** c1 = new Controllable*[3];
	c1[0] = t1;
	c1[1] = t2;
	c1[2] = t3;
	Table* subtbl = new Table(3, 1, 0, 0, c1);

	TextLabel* tl1 = new TextLabel(newwin(1, 5, 0, 0), "(0,0)");
	Menu* menu = new Menu(newwin(3, 18, 0, 0), 3, 1);
	menu->setItem("012345678901234", "", 0, 'N');
	menu->setItem("11234567890123456", "", 1, 'L');
	menu->setItem("2123456789", "", 2, 'Q');
	TextLabel* tl2 = new TextLabel(newwin(1, 10, 0, 0), "(0,1)");
	//TextLabel* tl3 = new TextLabel(newwin(1, 15, 0, 0), "(1,0)");
	TextLabel* tl4 = new TextLabel(newwin(1, 8, 0, 0), "(1,1)");

	Controllable** c = new Controllable*[8];
	c[0] = tl1;
	c[1] = menu;
	//c[2] = tl3;
	c[2] = NULL;
	c[3] = NULL;
	c[4] = subtbl;
	c[5] = tl4;
	c[6] = NULL;
	c[7] = NULL;

	int y = 1; 
	int x = 1;
	Table* tbl = new Table(4, 2, y, x, c);

	tbl->setRowHeight(1, 2);
	tbl->setRowHeight(3, 4);
	tbl->setColWidth(0, 25);

	bool playing = true;
	while (playing)
	{
		clear();
		wnoutrefresh(stdscr);
		tbl->draw();
		//subtbl->draw();
		doupdate();
		int c = getch();

		
		switch (c)
		{
		case KEY_UP: y--;  break;
		case KEY_DOWN: y++;  break;
		case KEY_LEFT: x--; break;
		case KEY_RIGHT: x++; break;
		case 'q': playing = false; break;
		}
		
		tbl->move(y, x);
	}
}

void derWinTest()
{
	WINDOW* win = newwin(6, 24, 1, 1);

	int y = 2;
	int x = 8;
	WINDOW* dWin = derwin(win, 2, 8, y, x);

	
	bool playing = true;
	while (playing)
	{
		//clear();
		//wnoutrefresh(stdscr);

		mvwaddstr(win, 0, 0, "MAIN WINDOW");
		mvwaddstr(dWin, 0, 0, "SUBSUBSUB");
		//wbkgd(dWin, 'd' | 0x20000000); //why does this work when the derived window is set before the main window?!
		//wbkgd(win, 'w' | 0x40000000);

		wnoutrefresh(win);
		//wnoutrefresh(dWin); //draw and refresh subwin first?!
				
		doupdate();
		//keypad(win, true);
		//keypad(dWin, true);
		int c = getch();

		switch (c)
		{
		case KEY_UP: y--;  break;
		case KEY_DOWN: y++;  break;
		case KEY_LEFT: x--; break;
		case KEY_RIGHT: x++; break;
		case 'q': playing = false; break;
		}
		int d = mvderwin(dWin, 1, 1);
		int e = 5;
	}
}

void wideTest()
{
	int c = sizeof(char);
	int w = sizeof(wchar_t);

	
	wchar_t wc2 = 256;
	wchar_t wc3 = 0xffff;

	int rows = getmaxy(stdscr);
	int currRow = 0;
	for (int i = 0; i <= 65535; i++)
	{
		wchar_t wc = i;
		
		mvwprintw(stdscr, currRow, 0,"%d) %d c:%c s:%hc w:%lc", i, wc, wc, wc, wc);

		currRow++;
		if (currRow > rows)
		{
			wnoutrefresh(stdscr);
			doupdate();
			getch();
			clear();
			currRow = 0;
		}
	}

}


void fileDialogTest(int dialogType)
{
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	string fullPath(buf);

	int height = 14; //height required is a minimum of 7 for enclosing FileChooser, which requires minimum of 5
	int width = 42; //min of 15 required

	WINDOW* main = newwin(height, width, (getmaxy(stdscr) - height) / 2, (getmaxx(stdscr) - width) / 2);
	WINDOW* sub = derwin(main, height - 2, width - 2, 1, 1);

	FileChooser* fd = new FileChooser(sub, fullPath, dialogType, ".map");

	Frame* f = new Frame(main, fd);
	string fileChosen;
	bool playing = true;
	while (playing)
	{
		f->draw();
		doupdate();

		int c = getch();

		//driver here
		switch (c)
		{
		case KEY_DOWN: fd->driver(REQ_DOWN_ITEM);   break;
		case KEY_UP: fd->driver(REQ_UP_ITEM); break;
		case CTRL_Q: playing = false; break;
		case '\r':
			fileChosen = fd->driver(REQ_TOGGLE_ITEM);
			break;
		default:
			fd->driver(c);
			break;
		}

		if (fileChosen.empty() == false)
		{
			//success!
			mvaddstr(0, 0, fileChosen.c_str());
		}

	}
}

void saveDialogTest()
{
	fileDialogTest(SAVE_DIALOG);
}

void openDialogTest()
{
	fileDialogTest(OPEN_DIALOG);
}

void scrollTest()
{
	int rows = 12;
	int cols = 1;
	WINDOW* win = newwin(4, (cols) * 18, 1, 1);

	Menu menu(win, rows, cols);
	menu.setItem("012345678901234", "", 0, 'N');
	menu.setItem("TOGGLE WRAP", "", 1, 'L');
	menu.setItem("QUIT", "", 2, 'Q');
	//menu.setItem("3123456789012345", "", 3, 'T');
	menu.setItem("4123456789012345", "", 4, '?');
	menu.setItem("Clear", "", 5, 'N');
	menu.setItem("6123456789012345", "", 6, 'L');
	menu.setItem("7123456789012345", "", 7, 'Q');
	menu.setItem("8123456789012345", "", 8, 'T');
	menu.setItem("9123456789012345", "", 9, '?');
	menu.setItem("a123456789012345", "", 10, 'N');
	menu.setItem("b123456789012345", "", 11, 'L');

	menu.disableItem(8, 0);
	menu.setMarkSide(RIGHT_MARK);
	bool wrap = true;

	bool usingMenu = true;
	
	ScrollBar* sb = new ScrollBar(&menu);
	
	while (usingMenu)
	{
		//menu.draw();
		sb->draw();
		doupdate();
		int input = getch();

		MenuItem* item = menuDriver(input, &menu);

		if (item == NULL)
		{
			continue;
		}

		switch (item->index)
		{

		case 2:
			usingMenu = false;
			break;
		case 1:
			menu.setWrapAround(wrap = !wrap); break;
		case 5:
			menu.clear();
			break;
		default:

			mvaddch(7, 30, (chtype)item->crossref);
			break;
		}
		item->itemChosen = false;
	}
}

void highlighterTest()
{
	//int height = getmaxy(stdscr);
	//int width = getmaxx(stdscr);

	//short y, x;
	//y = height / 2;
	//x = width / 2;

	//int totalTiles = height * width;
	//chtype* testMap = new chtype[totalTiles];
	//char asciiStart = ' ';
	//char asciiEnd = '~';
	//char asciiPtr = asciiStart;
	//for (int i = 0; i < totalTiles; i++)
	//{
	//	testMap[i] = (chtype)asciiPtr++;

	//	if (asciiPtr >= asciiEnd)
	//		asciiPtr = asciiStart;
	//}

	//Highlighter* hl = new Highlighter(stdscr, testMap, &y, &x);

	//bool playing = true;
	//
	//while (playing)
	//{
	//	clear();

	//	for (int i = 0; i < totalTiles; i++)
	//	{
	//		int tileY = i / width;
	//		int tileX = i % width;
	//		chtype c = testMap[i];
	//		
	//		mvaddch(tileY, tileX, c);
	//	
	//	}

	//	mvaddch(y, x, '@');
	//	wnoutrefresh(stdscr);
	//	hl->draw();

	//	doupdate();
	//	int input = getch();
	//	
	//	switch(input)
	//	{
	//	case KEY_LEFT: x--;	
	//		hl->setHighlighting(false);
	//		break;
	//	case KEY_RIGHT: x++; hl->setHighlighting(false); break;
	//	case KEY_UP: y--; hl->setHighlighting(false); break;
	//	case KEY_DOWN: y++; hl->setHighlighting(false); break;
	//	case KEY_ESC: playing = false; break;
	//	case KEY_SLEFT: hl->setHighlighting(true); x--; break;
	//	case KEY_SRIGHT: hl->setHighlighting(true); x++; break;
	//	case KEY_SUP: hl->setHighlighting(true); y--; break;
	//	case KEY_SDOWN: hl->setHighlighting(true); y++; break;
	//	case CTRL_C: 
	//		hl->copy();
	//		hl->setHighlighting(false);	
	//		break;
	//	case CTRL_V:
	//		hl->paste(y, x);
	//		break;
	//	case KEY_DC: //delete highlit region			
	//		hl->erase();
	//		break;
	//	default: //fill with printable character
	//		hl->fill(input);
	//		break;
	//	}




	//}



}

void mapTest()
{
	WINDOW* viewport = dupwin(stdscr);
		//newwin(10, 15, 1, 1);
	Map* map = new Map("test", 10, 30, viewport);
	
	chtype* layer = map->getDisplayLayer();
	
	short centerY = getmaxy(viewport) / 2;
	short centerX = getmaxx(viewport) / 2;
	short y = -centerY;
	short x = -centerX;
	short curY = 0;
	short curX = 0;

	map->setPosition(y, x);
	
	char asciiStart = ' ';
	char asciiEnd = '~';
	char asciiPtr = asciiStart;
	int totalTiles = 10 * 30;
	for (int i = 0; i < totalTiles; i++)
	{
		layer[i] = (chtype)asciiPtr++;

		if (asciiPtr >= asciiEnd)
			asciiPtr = asciiStart;
	}

	//Highlighter* hl = new Highlighter(viewport, layer, &curY, &curX);
	Highlighter* hl = new Highlighter(map, &curY, &curX);

	//hl->setOffset(map->getUlYPtr(), map->getUlXPtr());

	
	bool playing = true;
	while (playing)
	{
		/*clear();
		wnoutrefresh(stdscr);*/
		wclear(viewport);
		wbkgd(viewport, '%');
		wnoutrefresh(viewport);
		map->draw();
		hl->draw();
		mvwaddch(viewport, centerY, centerX, '@' | 0x0f000000);
		wnoutrefresh(viewport);
		doupdate();
		int input = getch();


		switch (input)
		{
		case KEY_UP: y--; curY--; hl->setHighlighting(false); break;
		case KEY_DOWN: y++; curY++; hl->setHighlighting(false); break;
		case KEY_LEFT: x--; curX--; hl->setHighlighting(false); break;
		case KEY_RIGHT: x++; curX++; hl->setHighlighting(false); break;
		case KEY_SLEFT: hl->setHighlighting(true); x--; curX--; break;
		case KEY_SRIGHT: hl->setHighlighting(true); x++; curX++; break;
		case KEY_SUP: hl->setHighlighting(true); y--; curY--; break;
		case KEY_SDOWN: hl->setHighlighting(true); y++; curY++; break;
		case KEY_ESC: playing = false; break;
		case CTRL_C:
			hl->copy();
			hl->setHighlighting(false);
			break;
		case CTRL_V:
			hl->paste();
			break;
		case KEY_DC: //delete highlit region			
			hl->erase();
			break;
		case CTL_UP:
		case CTL_DOWN:
			hl->flip(HL_VER);
			break;
		case CTL_LEFT:
		case CTL_RIGHT:
			hl->flip(HL_HOR);
			break;
		default: //fill with printable character
			hl->fill(input);
			break;
		}

		map->setPosition(y, x);
	}


}


void getRandomColor()
{
	srand(time(NULL));



}


void filterTest()
{
	WINDOW* viewport = dupwin(stdscr);
	
	Map* map = new Map("test", 10, 30, viewport);

	chtype* layer = map->getDisplayLayer();

	short centerY = getmaxy(viewport) / 2;
	short centerX = getmaxx(viewport) / 2;
	short y = -centerY;
	short x = -centerX;
	short curY = 0;
	short curX = 0;

	map->setPosition(y, x);

	char asciiStart = ' ';
	char asciiEnd = '~';
	char asciiPtr = asciiStart;
	int totalTiles = 10 * 30;
	for (int i = 0; i < totalTiles; i++)
	{
		layer[i] = (chtype)asciiPtr++ | COLOR_PAIR(COLOR_YELLOW);

		if (asciiPtr >= asciiEnd)
			asciiPtr = asciiStart;
	}

	FilterPattern* f = new BWFilter(map);

	bool playing = true;
	bool filterStatus = false;
	while (playing)
	{
		wclear(viewport);
		wbkgd(viewport, '%');
		wnoutrefresh(viewport);
		f->draw();
		
		mvwaddch(viewport, centerY, centerX, '@' | 0x0f000000);
		wnoutrefresh(viewport);
		doupdate();
		int input = getch();


		switch (input)
		{
		case KEY_UP: y--; curY--; break;
		case KEY_DOWN: y++; curY++; break;
		case KEY_LEFT: x--; curX--; break;
		case KEY_RIGHT: x++; curX++; break;
		case KEY_ESC: playing = false; break;
		default: //fill with printable character
			//toggle filter
			f->setEnabled(filterStatus = !filterStatus);
			break;
		}

		map->setPosition(y, x);
	}


}


void mapEffectFilterTest()
{
	WINDOW* viewport = dupwin(stdscr);

	Map* map = new Map("test", 10, 30, viewport);

	chtype* layer = map->getDisplayLayer();
	short* eLayer = map->getEffectsLayer();

	short centerY = getmaxy(viewport) / 2;
	short centerX = getmaxx(viewport) / 2;
	short y = -centerY;
	short x = -centerX;
	short curY = 0;
	short curX = 0;

	map->setPosition(y, x);

	char asciiStart = ' ';
	char asciiEnd = '~';
	char asciiPtr = asciiStart;
	int totalTiles = 10 * 30;
	for (int i = 0; i < totalTiles; i++)
	{
		int y = i / map->getWidth();
		int x = i % map->getWidth();

		if (x < 10)
			layer[i] = (chtype)asciiPtr++ | COLOR_PAIR(COLOR_YELLOW);
		else if(x >= 10 && x < 20)
			layer[i] = (chtype)asciiPtr++ | (COLOR_YELLOW << 28); //yellow background
		else if(x < 30)
			layer[i] = (chtype)asciiPtr++ | (COLOR_YELLOW_BOLD << 28); //yellow background

		if (asciiPtr >= asciiEnd)
			asciiPtr = asciiStart;

		//give frame obstructed effect
		if (y == 0 || x == 0 || y == map->getHeight() - 1 || x == map->getWidth() - 1)
			eLayer[i] = E_OBSTR;
	}

	MapEffectFilterPattern* f = new MapEffectFilterPattern(map);
	f->setEffectColorPair(COLOR_CYAN, E_OBSTR);

	bool playing = true;
	bool filterStatus = false;
	while (playing)
	{
		wclear(viewport);
		wbkgd(viewport, '%');
		wnoutrefresh(viewport);
		f->draw();

		mvwaddch(viewport, centerY, centerX, '@' | 0x0f000000);
		wnoutrefresh(viewport);
		doupdate();
		int input = getch();


		switch (input)
		{
		case KEY_UP: y--; curY--; break;
		case KEY_DOWN: y++; curY++; break;
		case KEY_LEFT: x--; curX--; break;
		case KEY_RIGHT: x++; curX++; break;
		case KEY_ESC: playing = false; break;
		default: //fill with printable character
				 //toggle filter
			f->setEnabled(filterStatus = !filterStatus);
			break;
		}

		map->setPosition(y, x);
	}


}

void storageTest()
{
	_2DStorage<int>* displayData = new _2DStorage<int>(5, 3);

	displayData->setDatum(0, 8);
	displayData->setDatum(3, 1, 9);


	int a = displayData->getDatum(3, 1);

	int* b = displayData->getData();

	system("pause");
}


void simpleMapTest()
{

}


int main()
{
	TUI* tui = new TUI();
	tui->init();
	//curs_set(0);
	//menuTest();
	//menuTest2();
	//fileDialogTest();
	//scrollTest();
	storageTest();
	//mapEditorTest();
//	filterTest();
	//mapEffectFilterTest();
//	tableTest();
	//mapTest();
	//highlighterTest();
	tui->shutdown();
	delete tui;
	return 0;
}

