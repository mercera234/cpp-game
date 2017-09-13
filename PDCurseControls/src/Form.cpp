#include "Form.h"

Form::Form(WINDOW* win, int capacity)
{
	setWindow(win);

	this->capacity = capacity;

	fields = new FormField*[capacity];
	for (int i = 0; i < capacity; i++)
	{
		fields[i] = NULL;
	}
}

void Form::draw()
{

	wnoutrefresh(win);
}