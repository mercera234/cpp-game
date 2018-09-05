#include "TUI.h"
#include "map_ui_tests.h"
#include "movement_tests.h"
#include "state_machine_tests.h"
#include "music_player_test.h"
#include "inventory_tests.h"
#include "misc_tests.h"

//RPG LIB UI TESTER
int main()
{
	TUI tui;
	tui.init();
	

	mapEffectFilterTest();
	
	tui.shutdown();
		
	return 0;
}

