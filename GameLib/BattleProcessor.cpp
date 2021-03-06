#include "BattleProcessor.h"
#include "LineItem.h"
#include <sstream>
#include "MenuItem.h"
#include "BattleAlgorithm.h"
#include "actor_helper.h"
#include <assert.h>
#include "GameInput.h"
#include "defaults.h"
#include "LineFormat.h"
#include "menu_drivers.h"
#include "GameAlgorithm.h"


void BattleProcessor::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);
	totalRows = getmaxy(win);//total row/col dimensions same as visible
	totalCols = getmaxx(win);
}


void BattleProcessor::addParticipants(std::list<Actor*>& players, std::list<Actor*>& enemies)
{
	createActorCards(players, 0, humanActors);
	createActorCards(enemies, humanActors.size(), cpuActors);

	//build environment
	initTurnTracker();
	initTargetMenu();
	initSkillMenu();
	initMessageDisplay();
	initControlManager();
}

void BattleProcessor::clearParticipants()
{
	humanActors.clear();
	cpuActors.clear();
}

void BattleProcessor::begin()
{
	if (inSession == false)
	{
		inSession = advanceTurn(); //in session if players/enemies were loaded and thus turn was advanced
	}
	//else begin was already called
}


BattleProcessor::BattleProcessor(WINDOW* win, std::list<Actor*>& players, std::list<Actor*>& enemies)
{
	setWindow(win);
	addParticipants(players, enemies);
}

/*
Must be done after actors have been setup
*/
void BattleProcessor::initTurnTracker()
{
	int totalPlayers = humanActors.size() + cpuActors.size();
	turnTracker = new TurnTracker();
	
	for each (ActorCard* card in humanActors)
	{
		PlayerActor* player = new PlayerActor();
		player->setActor(card->getActor());
		turnTracker->addPlayer(player);
	}

	for each (ActorCard* card in cpuActors)
	{
		PlayerActor* player = new PlayerActor();
		player->setActor(card->getActor());
		turnTracker->addPlayer(player);
	}
}


void BattleProcessor::initTargetMenu()
{
	//setup target menu
	int actorCardHeight = 4; //this should maybe be calculated
	int totalCapacity = humanActors.size() + cpuActors.size();

	targetMenu = new GraphMenu(newwin(actorCardHeight * 4, visibleCols - 2, 1, 1), totalCapacity);

	//link cards
	MenuItem::linkItemGroup(humanActors, Dir::DOWN);
	MenuItem::linkItemGroup(cpuActors, Dir::DOWN);

	MenuItem::linkItemGroups(humanActors, cpuActors, Dir::RIGHT);
	
	//position cards
	int pLeft = 0;
	int eLeft = pLeft + (visibleCols / 2); //midway across screen

	MenuItem::positionItemGroup(humanActors, 0, pLeft);
	MenuItem::positionItemGroup(cpuActors, 0, eLeft);

	//add cards to target menu
	addCardsToTargetMenu(humanActors);
	addCardsToTargetMenu(cpuActors);
}


void BattleProcessor::addCardsToTargetMenu(std::list<MenuItem*>& cards)
{
	for each (MenuItem* item in cards)
	{
		targetMenu->setItem(item);
	}
}

int BattleProcessor::attack(Actor * attacker, Actor * target)
{
	int attackerStrength = attacker->getStat(StatType::STRENGTH).getCurr();

	int damage = calcAttackDamage(attackerStrength, target->getStat(StatType::DEFENSE).getCurr());

	int agilityAdvantage = calcAgilityAdvantage(attacker->getStat(StatType::AGILITY).getCurr(),
		attackerStrength,
		target->getStat(StatType::AGILITY).getCurr()
	);

	damage += agilityAdvantage;

	damage += calcVariance(damage, 20);

	return damage;
}


void BattleProcessor::createActorCards(std::list<Actor*>& actors, int startNdx, std::list<MenuItem*>& cards)
{
	int i = 0;
	int index = startNdx;
	for each (Actor* actor in actors)
	{
		cards.push_back(new ActorCard(actor, index++, -1));
	}
}


void BattleProcessor::initSkillMenu()
{
	//setup skill menu
	int skillMenuRows = 4;
	skillMenu = new GridMenu(newwin(skillMenuRows, visibleCols - 2, visibleRows - 5, 1), skillMenuRows, 1);

	//we shouldn't initialize and post here, this should be done based on which human's turn it is
	skillMenu->setItem(new LineItem(ATTACK, 0, -1));
	skillMenu->setItem(new LineItem(MAGIC, 1, -1));
	skillMenu->setItem(new LineItem(S_ITEM, 2, -1));
	skillMenu->setItem(new LineItem(RUN, 3, -1));

	skillMenu->post(true);
	skillMenu->setCurrentItem(0);

	int frameHeight = 6;
	skillMenuFrame = new Frame(newwin(frameHeight, visibleCols, visibleRows - frameHeight, 0), skillMenu);
}

void BattleProcessor::initMessageDisplay()
{
	//set message display
	int msgDisplayRows = 4;
	msgDisplay = new TextLabel(newwin(msgDisplayRows, visibleCols - 2, visibleRows - 5, 1), "");
	msgDisplay->setFormat(new LineFormat(0, Justf::CENTER));
}

void BattleProcessor::initControlManager()
{
	cmd.setReceiver(this);
	cmd.setAction(&BattleProcessor::driver);

	//setup controlmanager
	cm.setCaller(this);
	cm.registerControl(targetMenu, KEY_LISTENER, &cmd);
	cm.registerControl(skillMenuFrame, KEY_LISTENER, &cmd);
	cm.registerControl(msgDisplay, KEY_LISTENER, &cmd);
}

bool BattleProcessor::advanceTurn()
{
	turnHolder = (PlayerActor*)turnTracker->getNext();
	
	if (turnHolder == nullptr)
		return false;

	Actor* next = turnHolder->getActor();

	switch (next->getType())
	{
	case ActorType::HUMAN:
		skillMenuFrame->setText(next->name, 0, 4);
		cm.setFocusedControl(skillMenuFrame);
		break;
	case ActorType::CPU:
		processCPUTurn();
		break;
	}

	return true;
}


void BattleProcessor::draw()
{
	werase(win);
	wnoutrefresh(win);

	bool showMsgs = false;
	bool showSkillMenu = true;
	if (cm.getFocusedControl() == msgDisplay)
	{
		showMsgs = true;
		showSkillMenu = false;
	}
	msgDisplay->setShowing(showMsgs);
	skillMenu->setShowing(showSkillMenu);

	cm.draw();
}


void BattleProcessor::driver()
{
	GameInput input = (GameInput)cm.getInput();
	Controllable* control = cm.getFocusedControl();
	if (control == skillMenuFrame)
		skillMenuDriver(input);
	else if (control == targetMenu)
		targetMenuDriver(input);
	else if (control == msgDisplay)
		displayDriver(input);
	else
		cm.setExitCode(ExitCode::NOT_HANDLED);

}

void BattleProcessor::skillMenuDriver(int input)
{
	MenuItem* item = nullptr;
	item = menuDriver((GameInput)input, skillMenu);
	
	if (item != nullptr)
	{
		switch (item->index)
		{
		case 0: //attack
			cm.setFocusedControl(targetMenu);
			targetMenu->setCurrentItem(humanActors.size()); //current item is first enemy to start with
			break;
		case 3: //run
			stopBattle();
			messages.push_back("You ran away.");
			skillMenuFrame->setText("", 0, 4); //clear human's name from the frame
			cm.setFocusedControl(msgDisplay);
			setMessage();
			break;
		}
	}
}

void BattleProcessor::targetMenuDriver(int input)
{
	MenuItem* item = nullptr;

	switch (input)
	{
	case GameInput::DOWN_INPUT: targetMenu->driver(REQ_DOWN_ITEM); break;
	case GameInput::UP_INPUT: targetMenu->driver(REQ_UP_ITEM); break;
	case GameInput::LEFT_INPUT: targetMenu->driver(REQ_LEFT_ITEM); break;
	case GameInput::RIGHT_INPUT: targetMenu->driver(REQ_RIGHT_ITEM); break;
	case GameInput::OK_INPUT:
		item = targetMenu->getCurrentItem();
		break;
	case GameInput::CANCEL_INPUT: 
		cm.setFocusedControl(skillMenuFrame);
		break;
	default: break;
	}


	if (item != nullptr) //target was chosen, apply skill
	{
		//only skill right now is attack so use that
		ActorCard* target = (ActorCard*)item;
		
		int damage = attack(turnHolder->getActor(), target->getActor());

		target->applyDamage(damage);
		std::ostringstream attackMsg;
		attackMsg << turnHolder->getActor()->name << " hits " << target->getActor()->name;

		messages.push_back(attackMsg.str());

		//check if enemies are defeated

		if (checkIfDefeated(cpuActors))
		{
			setupVictory();
		}

		targetMenu->setCurrentItem(NO_CUR_ITEM);
		skillMenuFrame->setText("", 0, 4); //clear human's name from the frame
		cm.setFocusedControl(msgDisplay);
		setMessage();
	}
}

void BattleProcessor::stopBattle()
{
	//disable TurnTracker
	turnTracker->setActive(false);
}

void BattleProcessor::setupDeath()
{
	stopBattle();
	messages.push_back("You died.");
	everyoneDied = true;
}


void BattleProcessor::setupVictory()
{
	stopBattle();

	//add messages
	messages.push_back("You killed the enemies.");

	resourceManager->getData().alterIntData(BATTLES_WON, 1);
	resourceManager->getData().alterIntData(ENEMIES_KILLED, cpuActors.size());

	//calculate experience and money earned
	int totalExp, totalMoney;
	calcRewards(totalExp, totalMoney);
	transferRewards(totalExp, totalMoney);
}


void BattleProcessor::calcRewards(int& totalExp, int &totalMoney)
{
	totalExp = 0;
	totalMoney = 0;

	for each (MenuItem* item in cpuActors)
	{
		ActorCard* card = (ActorCard*)item;
		Actor* enemy = card->getActor();

		totalExp += enemy->getStat(StatType::EXP).getCurr();
		totalMoney += enemy->getMoney().getCurr();
	}

	std::ostringstream rewardMsg;
	rewardMsg << "Gained " << totalExp << " exp. Earned " << totalMoney << " gold $.";
	messages.push_back(rewardMsg.str());
}

void BattleProcessor::transferRewards(int totalExp, int totalMoney)
{
	//transfer gold
	resourceManager->getData().alterIntData(GOLD$, totalMoney);
	
	//divided total gained by number of players
	int expPerPlayer = totalExp / humanActors.size();
	if (expPerPlayer <= 0)
		expPerPlayer = 1;

	//transfer exp and process level gains
	for each (MenuItem* item in humanActors)
	{
		ActorCard* card = (ActorCard*)item;
		Actor* player = card->getActor();

		//player->getStat(StatType::EXP).alterCurr(expPerPlayer);

		//check for level up here as well (not fully implemented yet)
		int levelsGained = alg::gainExp(*player, expPerPlayer);

		//setup message for level ups
		if (levelsGained > 0)
		{
			std::ostringstream gainLevelMsg;
			gainLevelMsg << player->name;

			levelsGained == 1 ?
				gainLevelMsg << " gained a level." :
				gainLevelMsg << " gained " << levelsGained << " levels.";

			messages.push_back(gainLevelMsg.str());
		}
	}
}

void BattleProcessor::displayDriver(int input)
{
	//input can only come from the acknowledgement key(we'll do others later)
	if (input != GameInput::OK_INPUT)
		return;

	clearDisplayDamage();

	//advance acknowledgements until exhausted
	setMessage();
	
	if(messages.size() <= 0 && msgDisplay->getText().compare("") == 0)  //no more messages left
	{
		if (advanceTurn() == false) //no more messages and turntracker is turned off
		{
			inSession = false;
			if (everyoneDied)
				cm.setExitCode(QUIT_TO_TITLE);
			else
				cm.setExitCode(ExitCode::GO_BACK);
				
		}
			
	}
}

void BattleProcessor::clearDisplayDamage()
{
	//clear actor cards from displaying damage
	for each (MenuItem* item in humanActors)
	{
		ActorCard* card = (ActorCard*)item;
		card->setDisplayStatChanges(false);
	}

	for each (MenuItem* item in cpuActors)
	{
		ActorCard* card = (ActorCard*)item;
		card->setDisplayStatChanges(false);
	}
}

/*
Return true if there was a message to print
*/
void BattleProcessor::setMessage()
{
	if (messages.size() <= 0)//verify there is a message to print
	{
		msgDisplay->setText("");
		return;
	}
		
		
	msgDisplay->setText(messages.front());
	messages.pop_front();
}


void BattleProcessor::processInput()
{
	cm.setExitCode(HANDLED);
	if (inSession == false)
	{
		exitCode = ExitCode::GO_BACK;
		return;
	}

	exitCode = ::processInput(cm, input);
}

void BattleProcessor::processCPUTurn()
{
	//choose random human target
	int targetId = rand() % humanActors.size();
	int seq = 0;
	ActorCard* target = nullptr;

	for each (MenuItem* item in humanActors)
	{
		ActorCard* card = (ActorCard*)item;
		if (seq++ == targetId)
		{
			target = card;
		}
	}
	
	//apply skill (just attack for now)
	int damage = attack(turnHolder->getActor(), target->getActor());
	target->applyDamage(damage);

	std::ostringstream attackMsg;
	attackMsg << turnHolder->getActor()->name << " hits " << target->getActor()->name;

	messages.push_back(attackMsg.str());

	
	if(checkIfDefeated(humanActors))
	{
		setupDeath();
	}

	cm.setFocusedControl(msgDisplay);
	setMessage();
}

bool BattleProcessor::checkIfDefeated(std::list<MenuItem*>& cards)
{
	bool allDead = true;

	for each (MenuItem* item in cards)
	{
		ActorCard* card = (ActorCard*)item;
		Actor* actor = card->getActor();

		if (isAlive(*actor))
		{
			allDead = false; break;
		}
	}

	return allDead;
}