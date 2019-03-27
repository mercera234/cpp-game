#pragma once
#include "MenuItem.h"
#include "GameItem.h"

class EquippedItem : public MenuItem
{
private:
	GameItem* item = nullptr;
	EquipPart part;
	void init();
public:
	EquippedItem();
	EquippedItem(EquipPart partIn, GameItem* possessionIn, int element);
	
	void setItem(GameItem* itemIn) { item = itemIn; }
	GameItem* getItem() { return item; }

	void setPart(EquipPart partIn) { part = partIn; }
	EquipPart getPart() const { return part; }

	void draw();
};

