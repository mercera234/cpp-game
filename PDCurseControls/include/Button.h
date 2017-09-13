#pragma once
#include <string>
#include "Controllable.h"

//Do I need this class or can I just use a text label as a key listener?
class Button : public Controllable
{
private:
	std::string caption;
public:
	Button(WINDOW* win, std::string& captionIn);
	void draw();
};
