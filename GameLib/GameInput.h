#pragma once
#include "Input.h"

//Game Inputs
enum GameInput
{
	UNKNOWN = -1,
	OK_INPUT,
	CANCEL_INPUT,
	UP_INPUT,
	DOWN_INPUT,
	LEFT_INPUT,
	RIGHT_INPUT,
	CYCLE_LEFT_INPUT,
	CYCLE_RIGHT_INPUT,
	OPEN_MENU_INPUT,
	//Secret GameInput
	QUIT_INPUT,
	FIGHT_TRIGGER,
	TOGGLE_ENCOUNTERS
};


//
//const Input inputs[2] = {
//	Input("Ok", OK_INPUT, false),
//	Input("Cancel", CANCEL_INPUT, false)
//
//};