#include "TUI.h"
#include "curses.h"
#include "GameStateManager.h"
#include "state_machine_tests.h"
//#include "TestState.h"
#include "MockTitle.h"

void gameStateManagerTest2()
{
	//WINDOW* menuWin = newwin(4, 25, 1, 1);

	//GameStateManager stateMngr;
	//GameState* titleScreen = new TitleScreenState(menuWin);
	//stateMngr.push(titleScreen);

	//bool playing = true;

	//while (playing)
	//{
	//	//draw
	//	stateMngr.draw();
	//	doupdate();

	//	//input
	//	int input = getch();

	//	//process
	//	switch (input)
	//	{
	//	case KEY_ESC: playing = false; break;
	//	default:
	//		playing = stateMngr.processInput(input);
	//		break;
	//	}
	//}

}


void gameStateManagerEmptyTest()
{
	GameStateManager stateMngr;

	bool playing = true;

	while (playing) //loop should only run once
	{
		//draw
		stateMngr.draw();
		doupdate();

		//input
		int input = getch();

		//process
		switch (input)
		{
		case KEY_ESC: playing = false; break;
		default:
			playing = stateMngr.processInput(input);
			break;
		}
	}

}
//
//void gameStateManagerTest()
//{
//	GameStateManager stateMngr;
//
//	stateMngr.setState(TestState::getInstance());
//
//	bool playing = true;
//
//	while (playing)
//	{
//		//draw
//		stateMngr.draw();
//		doupdate();
//
//		//input
//		int input = getch();
//
//		//process
//		switch (input)
//		{
//		case KEY_ESC: playing = false; break;
//		default:
//			playing = stateMngr.processInput(input);
//			break;
//		}
//	}
//
//}


void gameStateManagerFullTest()
{
	GameStateManager stateMngr;

	stateMngr.setState(MockTitle::getInstance());

	bool playing = true;

	while (playing)
	{
		//draw
		stateMngr.draw();
		doupdate();

		//input
		int input = getch();

		//process
		switch (input)
		{
		case KEY_ESC: playing = false; break;
		default:
			playing = stateMngr.processInput(input);
			break;
		}
	}

}