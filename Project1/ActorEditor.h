#pragma once
#include "Actor.h"
#include "FormField.h"
#include "TextField.h"
#include "Editor.h"



class ActorEditor : public Editor
{
private:
	//ControlManager* cm;

	list<FormField*>* fields;

	int fieldMargin;

	Actor* actor;
	void textFieldDriver(TextField* field, int input);
	void saveField(FormField* ff);
	void loadField(FormField* ff);
	void setupFields();
	void setupField(string fieldName, int maxLen, int fieldY, int type, void* data);
	void createNew();
	void load(string fileName);
	void save(string fileName);
public:
	ActorEditor();
	void draw();
	bool processInput(int input);
	
	//static void globalCallback(void* caller, void* ptr, int input); //static
	static void textFieldCallback(void* caller, void* ptr, int input); //static
	
};