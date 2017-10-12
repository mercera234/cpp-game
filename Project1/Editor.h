#pragma once
#include "ControlManager.h"
#include "Frame.h"
#include "TextLabel.h"
#include "FileChooser.h"

#define DEF_FILENAME "<Untitled>" //will add open/close arrows later

#define E_NEW 0
#define E_OPEN 1
#define E_SAVE 2
#define E_QUIT 3

class Editor
{
protected:
	ControlManager* cm;

	//file data (this should be moved to Master Editor)
	string fileName;
	string dialogDefPath;
	TextLabel* fileNameLbl;
	bool modified = false;
	string extensionFilter;

	void updateFileNameLabel();
	void processGlobalInput(int input);
	Frame* createConfirmDialog(string confirmMsg);
	void confirmDialogDriver(Controllable* dialog, int input, int confirmMethod);
	void setupConfirmDialog(void(*callback) (void*, void*, int));
	void fileDialogDriver(Controllable* dialog, int input);
	void setupFileDialog(FileDialogType dialogType);
	virtual void load(string fileName) = 0;
	virtual void save(string fileName) = 0;
	virtual void createNew() = 0; //for creating a new instance of whatever is being edited
	void setModified(bool modified); //all routines to perform when current map has been modified from original state
public:
	virtual void draw() = 0;
	virtual bool processInput(int input) = 0;
	ControlManager* getControlManager() { return cm; }
	static void globalCallback(void* caller, void* ptr, int input); 
	static void fileDialogCallback(void* caller, void* ptr, int input);
	static void confirmNewCallback(void* caller, void* ptr, int input); 
	static void confirmOpenCallback(void* caller, void* ptr, int input); 
	static void confirmQuitCallback(void* caller, void* ptr, int input); 

};