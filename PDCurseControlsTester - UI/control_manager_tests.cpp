#include "ControlManager.h"
#include "Frame.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "TextField.h"
#include "Palette.h"
#include "ControlManageApp.h"
#include "SimpleControlCommand.h"

void controlManagerTest()
{
	WINDOW* win = newwin(2, 20, 1, 1);
	GridMenu m1(win, 2, 1);

	m1.setItem(new LineItem("Yes", 0, 0));
	m1.setItem(new LineItem("No", 1, 1));
	m1.setCurrentItem(0);

	m1.post(true);

	Palette palette(4, 4, "CM Test", 7, 1);
	
	for (int i = 0; i < 7; i++)
	{
		chtype c = ' ' | i << BKGDCOLOR_OFFSET;
		palette.setItem(colorNames[i], c, i);
	}
	palette.setFocusable(false);
	palette.setCurrentItem(0);
	palette.post(true);

	TextField field1(15, 1, 30);

	ControlManageApp app;

	SimpleControlCommand<ControlManageApp> cmd;
	cmd.setReceiver(&app);
	cmd.setAction(&ControlManageApp::callBackTest2);

	SimpleControlCommand<ControlManageApp> cmd2;
	cmd2.setReceiver(&app);
	cmd2.setAction(&ControlManageApp::textCallback);

	SimpleControlCommand<ControlManageApp> cmd3;
	cmd3.setReceiver(&app);
	cmd3.setAction(&ControlManageApp::callBackTest);

	SimpleControlCommand<ControlManageApp> cmd4;
	cmd4.setReceiver(&app);
	cmd4.setAction(&ControlManageApp::newCallback);

	SimpleControlCommand<ControlManageApp> cmd5;
	cmd5.setReceiver(&app);
	cmd5.setAction(&ControlManageApp::quitCallback);


	ControlManager cm;
	cm.registerControl(&palette, MOUSE_LISTENER, &cmd);
	cm.registerControl(&field1, MOUSE_LISTENER | KEY_LISTENER, &cmd2);
	cm.registerControl(&m1, MOUSE_LISTENER | KEY_LISTENER, &cmd3);
	cm.registerShortcutKey(CTRL_N, &cmd4);
	cm.registerShortcutKey(KEY_ESC, &cmd5);

	cm.setFocus(&field1);

	bool testing = true;
	while (testing)
	{
		wnoutrefresh(stdscr);
		cm.draw();

		doupdate();
		int c = getch();

		clear();
		if (cm.handleInput(c) > 0)
			testing = false;
	}
	cm.shutdown();

}
