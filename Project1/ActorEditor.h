#pragma once
#include "ControlManager.h"
#include "Actor.h"
#include "FormField.h"
#include "TextField.h"



class ActorEditor
{
private:
	ControlManager* cm;

	list<FormField*>* fields;

	int fieldMargin;

	Actor* actor;
	void textFieldDriver(TextField* field, int input);
	//void drawLabel(int row, string lbl);
	void processGlobalInput(int input);
	void saveFields();
	void saveField(FormField* ff);
	void setupFields();
	void setupField(string fieldName, int maxLen, int fieldY, int type, void* data);
public:
	ActorEditor();
	void draw();
	bool processInput(int input);
	
	static void globalCallback(void* caller, void* ptr, int input); //static
	static void textFieldCallback(void* caller, void* ptr, int input); //static
	
};