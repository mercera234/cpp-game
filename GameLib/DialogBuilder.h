#pragma once
#include "DialogWindow.h"
#include "Rectangle.h"
#include "ResourceManager.h"

class DialogBuilder
{
public:
	DialogBuilder() {}
	
	void buildMainMenu(DialogWindow& dWin, Rect r);
	void buildPlayerMenu(DialogWindow& dWin, Rect r);
	void buildDesc(DialogWindow& dWin, Rect r, ResourceManager& rm);
	void buildMainMenuBody(DialogWindow& dWin, Rect r, ResourceManager& rm);
	
};

