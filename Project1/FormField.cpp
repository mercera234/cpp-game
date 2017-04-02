#include "FormField.h"

FormField::FormField(TextLabel* label, Controllable* field, int type, void* data)
{
	this->label = label;
	this->field = field;
	this->type = type;
	this->data = data;
}

void FormField::setFocus()
{
	field->setFocus();
}
//FormField::FormField(string label, unsigned short length, void* data, int y, int x)
//{
//	this->label = label;
//	this->length = length;
//
//	//not sure yet how to specify the position
//	field = new TextField(newwin(1, length, y, x));
//	this->data = data;
//}

//void FormField::saveField()
//{
//	TextField* field = (TextField*)ff->getField();
//	int id = ff->getId();
//	void* data = ff->getData();
//	string text = field->getText();
//
//	switch (id)
//	{
//	case AE_NAME:
//	{string* d = (string*)data;
//	*d = text; }
//	break;
//	case AE_SYMBOL:
//	{char* c = (char*)data;
//	*c = text[0]; }
//	break;
//	case AE_HP:
//	case AE_MP:
//	case AE_EXP:
//	case AE_MONEY:
//	{int* i = (int*)data;
//	*i = stoi(text); }
//	break;
//	case AE_LEVEL:
//	case AE_STRENGTH:
//	case AE_DEFENSE:
//	case AE_INTELLIGENCE:
//	case AE_WILL:
//	case AE_AGILITY:
//	{short* s = (short*)data;
//	*s = (short)stoi(text); }
//	break;
//	case AE_ACCURACY:
//	case AE_LUCK:
//	{float* f = (float*)data;
//	*f = (float)stof(text); }
//	break;
//	}
//}

void FormField::draw()
{
	label->draw();
	field->draw();
}

//void ActorEditor::drawLabel(int row, string lbl)
//{
//	//start from fieldMargin, back 2 beyond : and space, subtract length of string
//	int left = fieldMargin - 2 - lbl.length();
//	mvprintw(row, left, "%s:", lbl.c_str());
//}
//
//FormField::FormField()
//{
//	setPosition(0, 0);
//	clear();
//}
//
//FormField::FormField(unsigned short y, unsigned short x)
//{
//	setPosition(y, x)
//	clear();
//}
//
//void FormField::setPosition(unsigned short y, unsigned short x)
//{
//	yPos = y;
//	xPos = x;
//}
//
//void FormField::clear()
//{
//
//}
