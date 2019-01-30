#include "menu_drivers.h"
#include "GameInput.h"
#include "AbstractMenu.h"

MenuItem* menuDriver(int input, AbstractMenu* m) //static
{
	MenuItem* item = nullptr;

	switch (input)
	{
	case GameInput::DOWN_INPUT: 
		m->setInput(REQ_DOWN_ITEM);
		m->processInput(); break;
	case GameInput::UP_INPUT: 
		m->setInput(REQ_UP_ITEM);
		m->processInput(); break;
	case GameInput::LEFT_INPUT: 
		m->setInput(REQ_LEFT_ITEM);
		m->processInput(); break;
	case GameInput::RIGHT_INPUT: 
		m->setInput(REQ_RIGHT_ITEM);
		m->processInput(); break;
		/*case KEY_PPAGE: m->driver(REQ_SCR_DPAGE); break;
		case KEY_NPAGE: m->driver(REQ_SCR_UPAGE); break;*/
	case GameInput::OK_INPUT:
		item = m->getCurrentItem();
		break;
	default: break;
	}

	return item;
}