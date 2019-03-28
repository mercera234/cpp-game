#include "TUI.h"
#include "misc_tests.h"
#include "defaults.h"

//GAMELIB UI TESTER
int main()
{
	TUI tui;
	resize_term(gameScreenHeight, gameScreenWidth);


	//mainMenuTest();
	//battleProcessorTest();
	equipTest();

	return 0;
}

