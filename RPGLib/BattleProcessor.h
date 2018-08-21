#pragma once

#include <list>
#include "Actor.h"
#include "ControlManager.h"
#include "GraphMenu.h"
#include "GridMenu.h"
#include "TextLabel.h"
#include "TurnTracker.h"
#include "ActorCard.h"
#include "Frame.h"
#include "PlayerActor.h"

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

	void createActorCards(std::list<Actor*>& actors, int startNdx, std::list<MenuItem*>& cards);
	void addCardsToTargetMenu(std::list<MenuItem*>& cards);

	void attack(Actor * attacker, Actor * target);
	//checkForVictory
	//checkForDeath
	void setupVictory();
	void calcRewards(int& totalExp, int &totalMoney);
	void transferRewards(int totalExp, int totalMoney);
	bool checkIfDefeated(std::list<MenuItem*>& cards);

	void setupDeath();
public:
	std::list<MenuItem*> humanActors; //human combatants
	std::list<MenuItem*> cpuActors; //cpu combatants
	std::list<std::string> messages;
	
	//display elements
	GraphMenu* targetMenu;
	Frame* skillMenuFrame;
	GridMenu* skillMenu;
	TextLabel* msgDisplay;

	//turntracking elements
	TurnTracker* turnTracker;
	PlayerActor* turnHolder;

	BattleProcessor();
	void setWindow(WINDOW* win);
	void addParticipants(std::list<Actor*>& players, std::list<Actor*>& enemies);
	BattleProcessor(WINDOW* win, std::list<Actor*>& players, std::list<Actor*>& enemies);
	void draw();
	bool processInput(int input);
	
	static void controlCallback(void* caller, void* ptr, int input); //static
};