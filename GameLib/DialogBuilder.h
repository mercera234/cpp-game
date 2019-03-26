#pragma once
#include "DialogWindow.h"
#include "Rectangle.h"
#include "ResourceManager.h"
#include "Actor.h"

class DialogBuilder
{
private:
	ResourceManager* rm;
	
	void setDialogControl(Controllable* control, DialogWindow& dWin, Rect& r);
	GridMenu* buildEquipmentMenu(Actor& actor);
public:
	DialogBuilder() {}

	void buildMainMenu(DialogWindow& dWin, Rect r);
	void buildPlayerMenu(DialogWindow& dWin, Rect r);
	void buildDesc(DialogWindow& dWin, Rect r);
	void buildMainMenuBody(DialogWindow& dWin, Rect r);
	
	void buildMainMenuStatus(DialogWindow& dWin, Rect r, Actor& actor);
	void buildInventory(DialogWindow& dWin, Rect r);
	void buildCenteredTextWin(DialogWindow& dWin, Rect r);
	void buildConfigMenu(DialogWindow& dWin, Rect r);
	void buildAutoMap(DialogWindow& dWin, Rect r);
	//void buildEquipDialog(DialogWindow& dWin, Rect r, Actor& actor);
	
	//setters/getters

	void setRm(ResourceManager* rmIn) { rm = rmIn; }

};

