#include "ActorEditor.h"
#include "TUI.h"
#include <string>

ActorEditor::ActorEditor()
{
	fieldMargin = 22;

	fields = new list<FormField*>();

	ActorDef* def = new ActorDef();
	actor = new Actor();
	actor->def = def;

	cm = new ControlManager(this);

	setupFields();

	cm->registerShortcutKey(KEY_ESC, globalCallback);
	cm->registerShortcutKey(CTRL_S, globalCallback);
	cm->setFocus(fields->front());
}


void ActorEditor::setupFields()
{
	ActorDef* def = actor->def;
	int startRow = 1;
	setupField("Name:", 16, startRow++, T_STRING, &(def->name));
	setupField("Symbol:", 1, startRow++, T_CHAR, &(def->symbol));
	setupField("HP:", 6, startRow++, T_INTEGER, &(def->maxHp));
	setupField("MP:", 4, startRow++, T_INTEGER, &(def->maxMp));
	setupField("Exp:", 6, startRow++, T_INTEGER, &(def->exp));
	setupField("Level:", 2, startRow++, T_SHORT, &(def->level));
	setupField("$:", 7, startRow++, T_INTEGER, &(def->money));
	setupField("Strength:", 3, startRow++, T_SHORT, &(def->strength));
	setupField("Defense:", 3, startRow++, T_SHORT, &(def->defense));
	setupField("Intelligence:", 3, startRow++, T_SHORT, &(def->intelligence));
	setupField("Will:", 3, startRow++, T_SHORT, &(def->will));
	setupField("Agility:", 3, startRow++, T_SHORT, &(def->agility));
	setupField("Accuracy:", 5, startRow++, T_FLOAT, &(def->accuracy));
	setupField("Luck:", 5, startRow++, T_FLOAT, &(def->luck));
}

void ActorEditor::setupField(string fieldName, int maxLen, int fieldY, int type, void* data)
{
	int rowHeight = 1;
	WINDOW* lblWin = newwin(rowHeight, fieldName.length() + 1, fieldY, fieldMargin - fieldName.length()); //all will have height of 1
	TextLabel* lbl = new TextLabel(lblWin, fieldName);

	TextField* field = new TextField(maxLen, fieldY, fieldMargin);
	field->setColor(COLOR_BLUE, COLOR_GREEN_BOLD);

	FormField* ffield = new FormField(lbl, field, type, data);
	
	fields->push_back(ffield);
	cm->registerControl(ffield, MOUSE_LISTENER | KEY_LISTENER, textFieldCallback);
}


void ActorEditor::draw()
{
	cm->draw();
}


//void ActorEditor::drawLabel(int row, string lbl)
//{
//	//start from fieldMargin, back 2 beyond : and space, subtract length of string
//	int left = fieldMargin - 2 - lbl.length();
//	mvprintw(row, left, "%s:", lbl.c_str());
//}


bool ActorEditor::processInput(int input)
{
	return cm->handleInput(input);
}

void ActorEditor::processGlobalInput(int input)
{
	//handle preliminary input
	switch (input)
	{
	case KEY_ESC: //quit
		/*if (modified)
		{
			Frame* f = createConfirmDialog();

			cm->registerControl(f, KEY_LISTENER, confirmQuitCallback);
			cm->setFocus(f);
		}
		else
		{*/
			cm->prepareForShutdown();
		//}
		break;
	case CTRL_N:
		/*if (modified)
		{
			Frame* f = createConfirmDialog();

			cm->registerControl(f, KEY_LISTENER, confirmNewCallback);
			cm->setFocus(f);
		}
		else
		{
			newMap();
		}*/
		break;
	case CTRL_O:
		/*if (modified)
		{
			Frame* f = createConfirmDialog();

			cm->registerControl(f, KEY_LISTENER, confirmOpenCallback);
			cm->setFocus(f);
		}
		else
		{
			setupFileDialog(OPEN_DIALOG);
		}*/

		break;
	case CTRL_S:
		//if (modified) //save only if there are changes
		//	setupFileDialog(SAVE_DIALOG);

		//temporary only!!!!
		saveFields();

		ofstream* os = new ofstream("test.atr", ios::binary | ios::trunc);
		actor->def->save(os);
		os->close();
		break;
	}
}


void ActorEditor::globalCallback(void* caller, void* ptr, int input) //static
{
	ActorEditor* ae = (ActorEditor*)caller;
	ae->processGlobalInput(input);
}


void ActorEditor::saveFields()
{
	ActorDef* def = actor->def;

	for (list<FormField*>::iterator it = fields->begin(); it != fields->end(); it++)
	{
		FormField* ff = (FormField*)*it;
		saveField(ff);
	}
}

void ActorEditor::saveField(FormField* ff)
{
	TextField* field = (TextField*)ff->getField();
	int id = ff->getId();
	void* data = ff->getData();
	string text = field->getText();

	switch (id)
	{
	case T_STRING:
	{
		string* d = (string*)data;
		*d = text; 
	} break;
	case T_CHAR:
	{
		char* c = (char*)data;
		*c = text[0]; 
	} break;
	case T_INTEGER:
	{
		int* i = (int*)data;
		*i = stoi(text); 
	} break;
	case T_SHORT:
	{
		short* s = (short*)data;
		*s = (short)stoi(text); 
	} break;
	case T_FLOAT:
	{
		float* f = (float*)data;
		*f = (float)stof(text); 
	} break;
	}
}

void ActorEditor::textFieldCallback(void* caller, void* ptr, int input) //static
{
	ActorEditor* ae = (ActorEditor*)caller;
	FormField* ffield = (FormField*)ptr;
	ae->textFieldDriver((TextField*)ffield->getField(), input);
}


void ActorEditor::textFieldDriver(TextField* field, int input)
{
	switch (input)
	{
	case KEY_MOUSE: //cm->setFocus(field); 
		break;
	case '\t':
		//does nothing here, but cycles choice in the control manager. We don't want this routed as input to the field
		break;
	default:
		field->inputChar(input);
		break;
	}



}