#include "ActorEditor.h"
#include "TUI.h"
#include <string>
#include <dirent.h>

ActorEditor::ActorEditor()
{
	fieldMargin = 22;
	extensionFilter = DEF_ACTOR_EXTENSION;

	fields = new list<FormField*>();

	ActorDef* def = new ActorDef();
	actor = new Actor();
	actor->def = def;

	//setup filename (this should eventually be moved to program that sets up master editor
	fileName = DEF_FILENAME;
	WINDOW* textWin = newwin(1, 15, 0, 1);
	fileNameLbl = new TextLabel(textWin, fileName);

	//setup default file path for opening/saving files
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	dialogDefPath = buf;


	cm = new ControlManager(this);

	setupFields();
	cm->registerControl(fileNameLbl, 0, 0);

	cm->registerShortcutKey(KEY_ESC, globalCallback);
	cm->registerShortcutKey(CTRL_S, globalCallback);
	cm->registerShortcutKey(CTRL_A, globalCallback);
	cm->registerShortcutKey(CTRL_N, globalCallback);
	cm->registerShortcutKey(CTRL_O, globalCallback);
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
	clear();
	wnoutrefresh(stdscr);
	cm->draw();
}


void ActorEditor::createNew()
{
	//clear text from all fields
	for (list<FormField*>::iterator it = fields->begin(); it != fields->end(); it++)
	{
		FormField* ff = (FormField*)*it;
		ff->clear();
	}
	cm->setFocus(fields->front());

	fileName = DEF_FILENAME;
//	updateFileNameLabel();
	setModified(false);
}


bool ActorEditor::processInput(int input)
{
	cm->handleInput(input);
	return cm->isActive();
}


void ActorEditor::load(string fileName)
{
	//load the actor from file
	ifstream is(fileName, std::ios::binary);
	actor->def->load(is);
	is.close();

	//populate each field with actor data
	for (list<FormField*>::iterator it = fields->begin(); it != fields->end(); it++)
	{
		FormField* ff = (FormField*)*it;
		loadField(ff);
	}
}

void ActorEditor::loadField(FormField* ff)
{
	TextField* field = (TextField*)ff->getField();
	int type = ff->getType();
	void* data = ff->getData();
	string text = field->getText();

	switch (type)
	{
	case T_STRING:
	{
		string* d = (string*)data;
		field->setText(*d);
	} break;
	case T_CHAR:
	{
		char* c = (char*)data;
		string c2 = c;
		field->setText(c2);
	} break;
	case T_INTEGER:
	{
		int* i = (int*)data;
		field->setText(*i);
	} break;
	case T_SHORT:
	{
		short* s = (short*)data;
		field->setText(*s);
	} break;
	case T_FLOAT:
	{
		float* f = (float*)data;
		field->setText(*f);
	} break;
	}


}


void ActorEditor::save(string fileName)
{
	//extract data from fields and put in the Actor
	for (list<FormField*>::iterator it = fields->begin(); it != fields->end(); it++)
	{
		FormField* ff = (FormField*)*it;
		saveField(ff);
	}

	//save the Actor to file
	ofstream os(fileName, ios::binary | ios::trunc);
	actor->def->save(os);
	os.close();
}


void ActorEditor::saveField(FormField* ff)
{
	TextField* field = (TextField*)ff->getField();
	int type = ff->getType();
	void* data = ff->getData();
	string text = field->getText();

	switch (type)
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
	case KEY_BTAB:
	case '\t':
		//does nothing here, but cycles choice in the control manager. We don't want this routed as input to the field
		break;
	default:
		field->inputChar(input);
		setModified(true);
		break;
	}



}