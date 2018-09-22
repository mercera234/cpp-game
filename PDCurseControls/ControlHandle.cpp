#include "ControlHandle.h"
#include <assert.h>

void ControlHandle::draw()
{
	assert(control);

	control->draw();
}