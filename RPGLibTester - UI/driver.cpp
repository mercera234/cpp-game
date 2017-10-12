#include "TUI.h"
#include "map_ui_tests.h"

//RPG LIB UI TESTER
int main()
{
	TUI tui;
	tui.init();
	
	//mapEffectTest();
	mapEffectFilterTest();

	tui.shutdown();
		
	return 0;
}

