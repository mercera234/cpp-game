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
#include "SimpleControlCommand.h"
#include "ResourceManager.h"

class BattleProcessor : public Controllable
{
private:
	SimpleControlCommand<BattleProcessor> cmd;
	bool inSession = false;
	bool everyoneDied = false;
	ResourceManager* resourceManager;

	int driver(Controllable* control, int input);
	bool advanceTurn(); //returns true if the turn was advanced to the next player
	void processCPUTurn();
	void initTurnTracker();
	void initTargetMenu();
	void initSkillMenu();
	void initMessageDisplay();
	void initControlManager();
	
	int skillMenuDriver(int input);
	int targetMenuDriver(int input);
	int displayDriver(int input);

	void setMessage();

	void createActorCards(std::list<Actor*>& actors, int startNdx, std::list<MenuItem*>& cards);
	void addCardsToTargetMenu(std::list<MenuItem*>& cards);

	int attack(Actor * attacker, Actor * target);

	void setupVictory();
	void calcRewards(int& totalExp, int &totalMoney);
	void transferRewards(int totalExp, int totalMoney);
	bool checkIfDefeated(std::list<MenuItem*>& cards);

	void setupDeath();
	void stopBattle();

	void clearDisplayDamage();
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
	void clearParticipants();
	BattleProcessor(WINDOW* win, std::list<Actor*>& players, std::list<Actor*>& enemies);

	void begin();
	void draw();
	int processInput(int input);

	void setResourceManager(ResourceManager* rm) { resourceManager = rm; }
};