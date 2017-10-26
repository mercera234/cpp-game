#include "TUI.h"
#include "map_ui_tests.h"
#include "movement_tests.h"
#include "state_machine_tests.h"

//RPG LIB UI TESTER
int main()
{
	TUI tui;
	tui.init();
	
	//mapEffectTest();
	//mapEffectFilterTest();
	//mapHighlighterTest();
	//simpleMapTest();
//	realMapTest();
	//freeMovementProcessorTest();
	
	gameStateManagerFullTest();

	tui.shutdown();
		
	return 0;
}

