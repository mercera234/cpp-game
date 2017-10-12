#pragma once

#include <list>
using namespace std;
#include "Actor.h"
#include "ControlManager.h"
#include "GraphMenu.h"
#include "GridMenu.h"
#include "TextLabel.h"
#include "TurnTracker.h"
#include "ActorCard.h"
#include "Frame.h"

class BattleProcessor : public Controllable
{
private:
	void driver(Controllable* control, int input);
	void advanceTurn();
	void processCPUTurn();
	void initTurnTracker();
	void initTargetMenu();
	void initSkillMenu();
	void initMessageDisplay();
	void initControlManager();
	
	void skillMenuDriver(int input);
	void targetMenuDriver(int input);
	void displayDriver(int input);

	void setMessage();

	void createActorCards(list<Actor*>& actors, int startNdx, list<MenuItem*>& cards);
	void addCardsToTargetMenu(list<MenuItem*>& cards);

	void attack(ActorCard * attacker, ActorCard * target);
	//checkForVictory
	//checkForDeath
	void setupVictory();
	void calcRewards(int& totalExp, int &totalMoney);
	void transferRewards(int totalExp, int totalMoney);

	void setupDeath();
public:
	list<MenuItem*> humanActors; //human combatants
	list<MenuItem*> cpuActors; //cpu combatants
	list<string> messages;
	
	GraphMenu* targetMenu;
	Frame* skillMenuFrame;
	GridMenu* skillMenu;
	TextLabel* msgDisplay;
	TurnTracker* turnTracker;
	ActorCard* turnHolder;

	BattleProcessor(WINDOW* win, list<Actor*>& players, list<Actor*>& enemies);
	void draw();
	bool processInput(int input);
	
	static void controlCallback(void* caller, void* ptr, int input); //static
};