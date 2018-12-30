#pragma once
#include "DialogWindow.h"
#include "Rectangle.h"
#include "ResourceManager.h"

class DialogBuilder
{
private:
	ResourceManager* rm;
	
public:
	DialogBuilder() {}
	std::map<std::string, Controllable*> controlCache;
	
	void buildMainMenu(DialogWindow& dWin, Rect r);
	void buildPlayerMenu(DialogWindow& dWin, Rect r);
	void buildDesc(DialogWindow& dWin, Rect r);
	void buildMainMenuBody(DialogWindow& dWin, Rect r);
	
	//void buildMainMenuStatus(DialogWindow& dWin, Rect r);

	//setters/getters

	void setRm(ResourceManager* rmIn) { rm = rmIn; }

};

