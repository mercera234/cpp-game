#pragma once
#include "MenuItem.h"
#include "GameItem.h"

class GameItemMenuRecord : public MenuItem
{
private:
	GameItem* item;
public:
	//setters/getters
	void setItem(GameItem* item) { this->item = item; }
	GameItem* getItem() { return item; }
	void draw();
};