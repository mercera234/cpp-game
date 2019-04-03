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
#include "SimpleCommand.h"
#include "ResourceManager.h"

class BattleProcessor : public Controllable, public InputProcessor
{
private:
	ControlManager cm;
	SimpleCommand<BattleProcessor> cmd;
	bool inSession = false;
	bool everyoneDied = false;
	ResourceManager* resourceManager;

	void driver();
	bool advanceTurn(); //returns true if the turn was advanced to the next player
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

	BattleProcessor() {}
	BattleProcessor(WINDOW* win, std::list<Actor*>& players, std::list<Actor*>& enemies);

	void setWindow(WINDOW* win);
	void addParticipants(std::list<Actor*>& players, std::list<Actor*>& enemies);
	void clearParticipants();
	

	void begin();
	void draw();
	void processInput();

	void setResourceManager(ResourceManager* rm) { resourceManager = rm; }
};