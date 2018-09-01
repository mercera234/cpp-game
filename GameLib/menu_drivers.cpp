#include "menu_drivers.h"
#include "GameInput.h"
#include "AbstractMenu.h"

MenuItem* menuDriver(int input, AbstractMenu* m) //static
{
	MenuItem* item = nullptr;
	int retval = -1;
	switch (input)
	{
	case GameInput::DOWN_INPUT: retval = m->driver(REQ_DOWN_ITEM); break;
	case GameInput::UP_INPUT: retval = m->driver(REQ_UP_ITEM); break;
	case GameInput::LEFT_INPUT: retval = m->driver(REQ_LEFT_ITEM); break;
	case GameInput::RIGHT_INPUT: retval = m->driver(REQ_RIGHT_ITEM); break;
		/*case KEY_PPAGE: m->driver(REQ_SCR_DPAGE); break;
		case KEY_NPAGE: m->driver(REQ_SCR_UPAGE); break;*/
	case GameInput::OK_INPUT:
		item = m->getCurrentItem();
		break;
	default: break;
	}

	return item;
}