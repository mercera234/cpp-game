#include "FormField.h"

FormField::FormField(TextLabel* label, FormControl* field, int type, void* data)
{
	this->label = label;
	this->field = field;
	this->type = type;
	this->data = data;
	focusable = field->isFocusable();
}

void FormField::setFocus()
{
	field->setFocus();
}

void FormField::setCursorFocus()
{
	field->setCursorFocus();
}


void FormField::draw()
{
	label->draw();
	field->draw();
}


