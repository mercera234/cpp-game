#pragma once
#include "MenuItem.h"
#include "GameItem.h"

class OwnedItemRecord : public MenuItem
{
private:
	OwnedItem* possession;
public:
	OwnedItemRecord() {}
	OwnedItemRecord(OwnedItem* possessionIn, int element);

	void draw();

	//setters/getters

	void setPossession(OwnedItem* possessionIn) { possession = possessionIn; }
	OwnedItem* getPossession() { return possession; }


};