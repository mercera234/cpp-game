#pragma once
#include "MenuItem.h"
#include "GameItem.h"

class OwnedItemRecord : public MenuItem
{
private:
	Possession* possession;
public:
	OwnedItemRecord() {}
	OwnedItemRecord(Possession* possessionIn, int element);

	void draw();

	//setters/getters

	void setPossession(Possession* possessionIn) { possession = possessionIn; }
	Possession* getPossession() { return possession; }


};