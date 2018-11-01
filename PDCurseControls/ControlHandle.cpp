#include "ControlHandle.h"
#include <assert.h>

void ControlHandle::draw()
{
	if (control == nullptr)
		return;

	control->draw();
}