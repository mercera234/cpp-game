#pragma once
#include "TextLabel.h"
#include "GridMenu.h"
#include <iostream>
#include <list>
using namespace std;
#include "MapEditor.h"
#include "ActorEditor.h"

#define EDITOR_WIDTH 150
#define EDITOR_HEIGHT 30

class MasterEditor
{
private:
	ControlManager* cm;
	string fileName;
	string dialogDefPath;
	TextLabel* fileNameLbl;
	bool modified;
	GridMenu* masterMenu;
	list<Editor*> editors;
	Editor* curEditor;

	void processGlobalInput(int input);
	void processMenuInput(int input);
public:
	//void run();
	MasterEditor();
	void draw();
	bool processInput(int input);

	static void menuCallback(void* caller, void* ptr, int input); //static
	static void globalCallback(void* caller, void* ptr, int input); //static
};